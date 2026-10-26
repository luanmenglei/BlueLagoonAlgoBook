"""Package a portable notebook, including PDF, maintained source and tests."""
from pathlib import Path
import zipfile,json,hashlib
from manifest import print_manifest
from verification import publication_coverage
ROOT=Path(__file__).resolve().parents[1]

def main():
    try:publication_coverage()
    except ValueError as ex:raise SystemExit(str(ex))
    stamp=json.loads((ROOT/'output/build.json').read_text(encoding='utf-8'))
    if stamp['inputs']!=print_manifest():raise SystemExit('Print inputs changed: rebuild PDF first.')
    if stamp['pdf_sha256']!=hashlib.sha256((ROOT/'output/BlueLagoon.pdf').read_bytes()).hexdigest():raise SystemExit('PDF differs from last build.')
    target=ROOT.parent/'BlueLagoon.zip'
    with zipfile.ZipFile(target,'w',zipfile.ZIP_DEFLATED) as z:
        for p in sorted(ROOT.rglob('*')):
            if not p.is_file():continue
            rel=p.relative_to(ROOT)
            if any(part in ('build','__pycache__','.git') for part in rel.parts):continue
            if rel.parts[0]=='scripts' and (p.name.startswith('prepare') or p.name in ('catalog.py','record_sources.py')):continue
            if p.suffix in ('.exe','.o','.pyc') or rel.as_posix()=='tests/all.cpp':continue
            z.write(p,Path('BlueLagoon')/rel)
    print(target)

if __name__=='__main__':main()
