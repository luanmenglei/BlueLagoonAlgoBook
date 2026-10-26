"""Regression tests for test ownership and the publication gate."""
from contextlib import redirect_stdout
from copy import deepcopy
from pathlib import Path
import io
import json
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'scripts'))
import verify
from verification import coverage_map, assertion_count, validate_report


class CoverageTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        (self.root / 'content').mkdir()
        (self.root / 'tests').mkdir()
        (self.root / 'content/A.h').write_text('// algorithm\n')
        self.suite = self.root / 'tests/test_a.cpp'

    def test_direct_include(self):
        self.suite.write_text('# include "../content/A.h"\n')
        self.assertEqual(coverage_map(self.root), {'A.h': ['test_a']})

    def test_missing_test_rejected(self):
        with self.assertRaisesRegex(ValueError, 'A.h'):
            coverage_map(self.root)

    def test_comments_are_not_coverage(self):
        self.suite.write_text('// #include "../content/A.h"\n'
                              '/*\n#include "../content/A.h"\n*/\n')
        with self.assertRaisesRegex(ValueError, 'A.h'):
            coverage_map(self.root)

    def test_indirect_dependency_is_not_ownership(self):
        (self.root / 'content/B.h').write_text('#include "A.h"\n')
        self.suite.write_text('#include "../content/B.h"\n')
        with self.assertRaisesRegex(ValueError, 'A.h'):
            coverage_map(self.root)

    def test_unknown_header_rejected(self):
        self.suite.write_text('#include "../content/Typo.h"\n')
        with self.assertRaisesRegex(ValueError, 'unknown code header'):
            coverage_map(self.root)

    def test_multiple_suites_and_duplicate_include(self):
        self.suite.write_text('#include "../content/A.h"\n' * 2)
        (self.root / 'tests/test_b.cpp').write_text('#include "../content/A.h"\n')
        self.assertEqual(coverage_map(self.root), {'A.h': ['test_a', 'test_b']})


class ReportTests(unittest.TestCase):
    def setUp(self):
        self.manifest = {'content/A.h': 'source-hash', 'tests/test_a.cpp': 'test-hash'}
        self.covered = {'A.h': ['test_a']}
        self.report = dict(schema=1, complete=True, tool_checks=True,
                           source_manifest=self.manifest.copy(), coverage=self.covered.copy(),
                           headers=[dict(path='A.h', passed=True)],
                           suites=[dict(name='test_a', passed=True, assertions=3,
                                        output='example: 3 assertions; seed 712367\n')])

    def validate(self, report):
        return validate_report(report, self.manifest, self.covered, ['test_a'])

    def test_complete_current_report(self):
        self.assertEqual(self.validate(self.report), self.covered)

    def test_empty_missing_duplicate_and_extra_results_rejected(self):
        for field in ['headers', 'suites']:
            for rows in [[], None, self.report[field] * 2,
                         self.report[field] + [dict(path='extra', name='extra', passed=True)]]:
                with self.subTest(field=field, rows=rows):
                    report = deepcopy(self.report)
                    report[field] = rows
                    with self.assertRaises(ValueError):
                        self.validate(report)

    def test_failed_results_rejected(self):
        for field in ['headers', 'suites']:
            report = deepcopy(self.report)
            report[field][0]['passed'] = False
            with self.assertRaises(ValueError):
                self.validate(report)

    def test_stale_code_and_tests_rejected(self):
        for path in self.manifest:
            report = deepcopy(self.report)
            report['source_manifest'][path] = 'old'
            with self.assertRaisesRegex(ValueError, 'changed'):
                self.validate(report)

    def test_partial_old_schema_and_missing_tool_checks_rejected(self):
        for key, value in [('complete', False), ('schema', 0), ('tool_checks', False)]:
            report = deepcopy(self.report)
            report[key] = value
            with self.assertRaises(ValueError):
                self.validate(report)

    def test_changed_ownership_rejected(self):
        report = deepcopy(self.report)
        report['coverage'] = {'A.h': ['test_b']}
        with self.assertRaisesRegex(ValueError, 'ownership'):
            self.validate(report)

    def test_zero_or_missing_runtime_assertions_rejected(self):
        for output in ['', 'example: 0 assertions; seed 712367\n',
                       'example: 3 assertions; seed 712367\n' * 2]:
            report = deepcopy(self.report)
            report['suites'][0]['output'] = output
            with self.assertRaisesRegex(ValueError, 'runtime assertions'):
                self.validate(report)

    def test_mismatched_assertion_count_rejected(self):
        report = deepcopy(self.report)
        report['suites'][0]['assertions'] = 4
        with self.assertRaises(ValueError):
            self.validate(report)

    def test_assertion_summary(self):
        self.assertEqual(assertion_count('CPU AVX2: yes\nexample: 15 assertions; seed 712367\n'), 15)
        self.assertEqual(assertion_count('PASS\n'), 0)


class RunnerTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        (self.root / 'tests').mkdir()
        (self.root / 'tests/test_a.cpp').write_text('// fixture\n')
        self.output = 'fixture: 2 assertions; seed 712367\n'
        self.report_path = self.root / 'tests/report.json'
        for name, value in [('ROOT', self.root), ('validate', lambda: None),
                            ('check_tools', lambda: None),
                            ('source_manifest', lambda: {'fixture': 'hash'}),
                            ('coverage_map', lambda: {'A.h': ['test_a']}),
                            ('entries', lambda: [dict(path='A.h', kind='code')]),
                            ('run', self.run_command)]:
            patcher = patch.object(verify, name, value)
            patcher.start()
            self.addCleanup(patcher.stop)

    def run_command(self, args, timeout=120):
        if '--version' in args:
            return 0, 'fixture compiler\n'
        if 'unittest' in args:
            return 0, 'Ran 1 test\nOK\n'
        if len(args) == 1:
            return 0, self.output
        return 0, ''

    def run_verify(self, *args):
        with patch.object(sys, 'argv', ['verify.py', *args]), redirect_stdout(io.StringIO()):
            with self.assertRaises(SystemExit) as result:
                verify.main()
        return result.exception.code

    def test_full_run_writes_publishable_report(self):
        self.assertEqual(self.run_verify(), 0)
        report = json.loads(self.report_path.read_text())
        validate_report(report, {'fixture': 'hash'}, {'A.h': ['test_a']}, ['test_a'])

    def test_partial_run_does_not_create_publication_report(self):
        self.assertEqual(self.run_verify('--suite', 'a', '--skip-headers'), 0)
        self.assertFalse(self.report_path.exists())
        report = json.loads((self.root / 'build/tests/report.partial.json').read_text())
        self.assertFalse(report['complete'])

    def test_partial_run_preserves_prior_full_report(self):
        self.assertEqual(self.run_verify(), 0)
        previous = self.report_path.read_bytes()
        self.assertEqual(self.run_verify('--suite', 'a'), 0)
        self.assertEqual(self.report_path.read_bytes(), previous)

    def test_empty_runtime_test_fails(self):
        self.output = ''
        self.assertEqual(self.run_verify(), 1)
        report = json.loads(self.report_path.read_text())
        with self.assertRaises(ValueError):
            validate_report(report, {'fixture': 'hash'}, {'A.h': ['test_a']}, ['test_a'])

    def test_failed_partial_run_invalidates_full_report(self):
        self.assertEqual(self.run_verify(), 0)
        self.output = ''
        self.assertEqual(self.run_verify('--skip-headers'), 1)
        self.assertFalse(self.report_path.exists())

    def test_unknown_suite_is_an_error(self):
        with patch.object(sys, 'stderr', io.StringIO()):
            self.assertEqual(self.run_verify('--suite', 'missing'), 2)
        self.assertFalse(self.report_path.exists())


if __name__ == '__main__':
    unittest.main()
