"""Compile every header independently and run deterministic regression/stress suites."""
from pathlib import Path
import argparse, concurrent.futures, json, os, shutil, subprocess, time
from manifest import source_manifest
from validate import validate
from check_tools import check_tools
from metadata import entries
ROOT=Path(__file__).resolve().parents[1]

def run(args,timeout=120):
    p=subprocess.run(args,cwd=ROOT,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,
                     encoding='utf-8',errors='replace',timeout=timeout)
    return p.returncode,p.stdout

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--compiler',default=os.environ.get('CXX','g++'))
    ap.add_argument('--jobs',type=int,default=4)
    ap.add_argument('--skip-headers',action='store_true')
    ap.add_argument('--suite')
    a=ap.parse_args(); cxx=shutil.which(a.compiler) or a.compiler
    validate()
    check_tools()
    build=ROOT/'build/tests'; build.mkdir(parents=True,exist_ok=True)
    before=source_manifest()
    es=entries(); report=dict(standard='GNU C++20',seed=712367,
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
    suite_files=list((ROOT/'tests').glob('test_*.cpp'))
    if a.suite: suite_files=[p for p in suite_files if a.suite in p.stem]
    def test(src):
        out=build/(src.stem+('.exe' if os.name=='nt' else ''))
        start=time.time(); code,log=run([cxx,*flags,str(src),'-o',str(out)])
        if not code:
            code,log=run([str(out)],timeout=120)
        (build/(src.stem+'.log')).write_text(log,encoding='utf-8')
        return dict(name=src.stem,passed=code==0,seconds=round(time.time()-start,2),output=log)
    with concurrent.futures.ThreadPoolExecutor(a.jobs) as pool:
        for item in pool.map(test,suite_files):
            print(item['name'], 'PASS' if item['passed'] else 'FAIL',item['output'],flush=True)
            report['suites'].append(item)
    old=ROOT/'tests/report.json'
    previous=json.loads(old.read_text(encoding='utf-8')) if old.exists() else {}
    if a.skip_headers and old.exists(): report['headers']=json.loads(old.read_text(encoding='utf-8'))['headers']
    if a.suite and old.exists():
        seen={x['name'] for x in report['suites']}
        report['suites']=[x for x in json.loads(old.read_text(encoding='utf-8'))['suites'] if x['name'] not in seen]+report['suites']
    after=source_manifest()
    if before != after: raise SystemExit('Sources changed during verification; rerun.')
    if not a.skip_headers and not a.suite:
        report['source_manifest']=after
    elif previous.get('source_manifest') == after:
        report['source_manifest']=after
    old.write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    failed=[x for key in ('headers','suites') for x in report[key] if not x['passed']]
    raise SystemExit(bool(failed))

if __name__=='__main__': main()
