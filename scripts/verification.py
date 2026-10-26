"""Require direct test ownership and a complete, current publication report."""
from pathlib import Path
import json
import re
from manifest import ROOT, source_manifest


def coverage_map(root=ROOT):
    root = Path(root).resolve()
    content = root / 'content'
    covered = {p.relative_to(content).as_posix(): []
               for p in sorted(content.rglob('*.h'))}
    for suite in sorted((root / 'tests').glob('test_*.cpp')):
        text = suite.read_text(encoding='utf-8')
        # Remove comments, preserving quoted strings (including include paths).
        text = re.sub(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|//[^\n]*|/\*.*?\*/',
                      lambda m: re.sub(r'[^\n]', ' ', m[0]) if m[0].startswith('/') else m[0],
                      text, flags=re.S)
        for include in re.findall(r'^\s*#\s*include\s*"([^"\n]+)"', text, re.M):
            path = (suite.parent / include).resolve()
            if not path.is_relative_to(content):
                continue
            name = path.relative_to(content).as_posix()
            if name not in covered:
                raise ValueError(f'{suite.name}: unknown code header {name}')
            if suite.stem not in covered[name]:
                covered[name].append(suite.stem)
    missing = [name for name, suites in covered.items() if not suites]
    if missing:
        raise ValueError('Headers without direct runtime tests:\n  ' + '\n  '.join(missing))
    return covered


def assertion_count(output):
    counts = re.findall(r'^.+: ([0-9]+) assertions; seed 712367\s*$', output, re.M)
    return int(counts[0]) if len(counts) == 1 else 0


def validate_report(report, manifest, covered, suites):
    if not isinstance(report, dict):
        raise ValueError('Invalid verification report.')
    if report.get('schema') != 1 or report.get('complete') is not True:
        raise ValueError('A full verification report is required; run scripts/verify.py.')
    if report.get('source_manifest') != manifest:
        raise ValueError('Sources/tests changed since verification; run scripts/verify.py.')
    if report.get('tool_checks') is not True:
        raise ValueError('Maintenance checks did not pass.')
    if report.get('coverage') != covered:
        raise ValueError('Test ownership changed; run scripts/verify.py.')
    for field, key, expected in [('headers', 'path', set(covered)),
                                 ('suites', 'name', set(suites))]:
        rows = report.get(field)
        if not isinstance(rows, list) or any(not isinstance(r, dict) for r in rows):
            raise ValueError(f'Invalid {field} results.')
        names = [row.get(key) for row in rows]
        if any(not isinstance(name, str) for name in names):
            raise ValueError(f'Invalid {field} names.')
        if len(names) != len(set(names)) or set(names) != expected:
            raise ValueError(f'Incomplete or duplicate {field} results; run full verification.')
        if any(row.get('passed') is not True for row in rows):
            raise ValueError(f'Failed {field}; publication refused.')
        if field == 'suites':
            for row in rows:
                count = assertion_count(row.get('output', ''))
                if count <= 0 or row.get('assertions') != count:
                    raise ValueError(f'{row["name"]}: no successful runtime assertions.')
    return covered


def publication_coverage():
    covered = coverage_map()
    try:
        report = json.loads((ROOT / 'tests/report.json').read_text(encoding='utf-8'))
    except (FileNotFoundError, json.JSONDecodeError) as ex:
        raise ValueError('Missing/invalid report; run scripts/verify.py first.') from ex
    suites = [p.stem for p in (ROOT / 'tests').glob('test_*.cpp')]
    return validate_report(report, source_manifest(), covered, suites)


if __name__ == '__main__':
    try:
        result = coverage_map()
    except ValueError as ex:
        raise SystemExit(str(ex))
    print(f'Direct test coverage: {len(result)}/{len(result)} headers')
