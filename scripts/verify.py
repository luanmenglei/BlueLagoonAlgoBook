"""Compile every header independently and run deterministic regression/stress suites."""
from pathlib import Path
import argparse, concurrent.futures, json, os, re, shutil, subprocess, sys, time
from manifest import source_manifest
from validate import validate
from check_tools import check_tools
from metadata import entries
from verification import coverage_map, assertion_count, validate_report
ROOT=Path(__file__).resolve().parents[1]

def run(args,timeout=120):
    try:
        p=subprocess.run(args,cwd=ROOT,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,
                         encoding='utf-8',errors='replace',timeout=timeout)
    except subprocess.TimeoutExpired:
        return 124,f'Timeout after {timeout}s: {args[0]}\n'
    return p.returncode,p.stdout

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--compiler',default=os.environ.get('CXX','g++'))
    ap.add_argument('--jobs',type=int,default=4)
    ap.add_argument('--skip-headers',action='store_true')
    ap.add_argument('--suite')
    a=ap.parse_args(); cxx=shutil.which(a.compiler) or a.compiler
    if a.jobs < 1:ap.error('--jobs must be positive')
    suite_files=sorted((ROOT/'tests').glob('test_*.cpp'))
    all_suites=[p.stem for p in suite_files]
    if a.suite is not None:
        suite_files=[p for p in suite_files if a.suite in p.stem]
        if not suite_files:ap.error(f'No test suites match {a.suite!r}')
    full=not a.skip_headers and a.suite is None
    old=ROOT/'tests/report.json'
    if full:old.unlink(missing_ok=True)
    before=source_manifest()
    validate()
    try:covered=coverage_map()
    except ValueError as ex:raise SystemExit(str(ex))
    print(f'Direct test coverage: {len(covered)}/{len(covered)} headers',flush=True)
    check_tools()
    code,log=run([sys.executable,'-m','unittest','discover','-s','tests','-p','test_*.py'])
    print(log,flush=True)
    if code or not re.search(r'Ran [1-9][0-9]* tests?',log):
        old.unlink(missing_ok=True)
        raise SystemExit('Maintenance regression tests failed or no tests ran.')
    build=ROOT/'build/tests'; build.mkdir(parents=True,exist_ok=True)
    es=entries(); report=dict(schema=1,complete=full,tool_checks=True,coverage=covered,
        standard='GNU C++20',seed=712367,
        compiler=run([cxx,'--version'])[1].splitlines()[0],headers=[],suites=[])
    flags=['-std=c++20','-O2','-Wall','-Wextra','-I',str(ROOT/'content')]
    def compile_header(e):
        name=e['path'].replace('/','_').replace('.h','')
        src=build/(name+'.cpp')
        text=f'#include "{e["path"]}"\n'
        if e['path']!='contest/Template.h': text+='int main() { return 0; }\n'
        src.write_text(text,encoding='utf-8')
        code,out=run([cxx,*flags,'-fsyntax-only',str(src)])
        if code: (build/(name+'.log')).write_text(out,encoding='utf-8')
        return dict(path=e['path'],passed=code==0,diagnostics=out if code else '')
    if not a.skip_headers:
        with concurrent.futures.ThreadPoolExecutor(a.jobs) as pool:
            report['headers']=list(pool.map(compile_header,[e for e in es if e['kind']=='code']))
        bad=[x for x in report['headers'] if not x['passed']]
        print(f'Headers: {len(report["headers"])-len(bad)}/{len(report["headers"])} passed',flush=True)
        for x in bad: print(x['path'],x['diagnostics'],flush=True)
    def test(src):
        out=build/(src.stem+('.exe' if os.name=='nt' else ''))
        start=time.time(); code,log=run([cxx,*flags,str(src),'-o',str(out)])
        if not code:
            code,log=run([str(out)],timeout=120)
        count=assertion_count(log) if not code else 0
        if not code and not count:
            code=1
            log+='\nNo positive runtime assertion summary; use CHECK(...) and done(...).\n'
        (build/(src.stem+'.log')).write_text(log,encoding='utf-8')
        return dict(name=src.stem,passed=code==0,assertions=count,
                    seconds=round(time.time()-start,2),output=log)
    with concurrent.futures.ThreadPoolExecutor(a.jobs) as pool:
        for item in pool.map(test,suite_files):
            print(item['name'], 'PASS' if item['passed'] else 'FAIL',item['output'],flush=True)
            report['suites'].append(item)
    after=source_manifest()
    if before != after: raise SystemExit('Sources changed during verification; rerun.')
    report['source_manifest']=after
    target=old if full else build/'report.partial.json'
    target.write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    failed=[x for key in ('headers','suites') for x in report[key] if not x['passed']]
    if failed and not full:old.unlink(missing_ok=True)
    if full and not failed:validate_report(report,after,covered,all_suites)
    print('Full report: tests/report.json' if full else
          'Partial report: build/tests/report.partial.json (not valid for publication)',flush=True)
    raise SystemExit(bool(failed))

if __name__=='__main__': main()
