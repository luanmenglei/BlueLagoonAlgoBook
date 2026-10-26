"""Prepare import fragments and compile the maintained BlueLagoon.tex entry point."""
from pathlib import Path
import argparse,hashlib,json,re,shutil,subprocess
from manifest import print_manifest
from typeset import style_definitions,tex
from metadata import load_chapters
from print_entry import render_entry
from verification import publication_coverage
ROOT=Path(__file__).resolve().parents[1]

def write_json(path,value):
    path.write_text(json.dumps(value,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')

def prepare():
    try:tested=publication_coverage()
    except ValueError as ex:raise SystemExit(str(ex))
    chapters=load_chapters();es=[e for c in chapters for e in c['entries']]
    labels={e['path']:'code:'+re.sub(r'[^a-zA-Z0-9]','-',e['path']) for e in es}
    built=ROOT/'build';out=ROOT/'output'
    built.mkdir(exist_ok=True);out.mkdir(exist_ok=True)
    book=json.loads((ROOT/'book.json').read_text(encoding='utf-8'))
    settings=[r'\newcommand{\bl'+name+'}{'+tex(book[key])+'}'
              for name,key in [('team','team'),('standard','standard'),('date','date')]]
    (built/'book-settings.tex').write_text('\n'.join(settings)+'\n',encoding='utf-8')
    (built/'highlight-style.tex').write_text(style_definitions(),encoding='utf-8')
    hashes={}
    for e in es:
        fragment,h=render_entry(ROOT,e,labels,tested)
        target=built/'entries'/(e['path']+'.tex');target.parent.mkdir(parents=True,exist_ok=True)
        target.write_text(fragment,encoding='utf-8')
        if h:hashes[e['path']]=h
    write_json(out/'catalog.json',chapters)
    write_json(out/'hashes.json',hashes)
    write_json(out/'coverage.json',[dict(path=e['path'],local_suites=tested.get(e['path'],[]),
        hash=hashes[e['path']],status='compile + runtime' if e['path'] in tested else 'compile only')
        for e in es if e['kind']=='code'])
    return len(es),len(hashes)

def compile_pdf():
    exe=shutil.which('xelatex')
    if not exe:raise SystemExit('XeLaTeX required: install TeX Live, or prepare with --tex-only')
    for _ in range(3):
        p=subprocess.run([exe,'-interaction=nonstopmode','-halt-on-error','-output-directory=build','BlueLagoon.tex'],cwd=ROOT,
            stdout=subprocess.PIPE,stderr=subprocess.STDOUT,encoding='utf-8',errors='replace')
        (ROOT/'build/xelatex-console.log').write_text(p.stdout,encoding='utf-8')
        if p.returncode:raise SystemExit(p.stdout[-5000:])
    pdf=ROOT/'output/BlueLagoon.pdf';shutil.copyfile(ROOT/'build/BlueLagoon.pdf',pdf)
    write_json(ROOT/'output/build.json',dict(inputs=print_manifest(),pdf_sha256=hashlib.sha256(pdf.read_bytes()).hexdigest()))

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--tex-only',action='store_true');args=ap.parse_args()
    count,headers=prepare()
    if args.tex_only:
        print(f'Prepared {count} imports; maintained TeX files were not overwritten')
        return
    compile_pdf()
    print(f'Built {ROOT/"output/BlueLagoon.pdf"}: {count} entries, {headers} code files')

if __name__=='__main__':main()
