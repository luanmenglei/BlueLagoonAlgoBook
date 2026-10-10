"""Check catalog, dependency declarations and printability before compiling."""
from pathlib import Path
import json,re
from metadata import entries as read_entries
ROOT=Path(__file__).resolve().parents[1]

def validate():
    book=json.loads((ROOT/'book.json').read_text(encoding='utf-8'))
    entries=read_entries()
    known={e['path']:e for e in entries}
    assert len(known)==len(entries),'Duplicate paths'
    for e in entries:
        p=ROOT/'content'/e['path'];assert p.is_file(),f'Missing {p}'
        if e['kind']!='code':continue
        for field in ('title','purpose','note','complexity','source','author','license','dependencies'):assert field in e,(e['path'],field)
        text=p.read_text(encoding='utf-8-sig')
        includes={str((p.parent/i).resolve().relative_to((ROOT/'content').resolve())).replace('\\','/')
                  for i in re.findall(r'^\s*#include "([^\n]+)"',text,re.M)}
        expected=set(e['dependencies'])
        if e['path']!='contest/Base.h':expected.add('contest/Base.h')
        assert includes==expected,(e['path'],includes,expected)
        for dep in expected:assert dep in known,(e['path'],dep)
    code={e['path'] for e in entries if e['kind']=='code'}
    files={p.relative_to(ROOT/'content').as_posix() for p in (ROOT/'content').rglob('*.h')}
    assert code==files,('Unlisted or missing headers',code^files)
    seen=set();active=set()
    def visit(p):
        assert p not in active,('Dependency cycle',p)
        if p in seen:return
        active.add(p)
        for d in known[p].get('dependencies',[]):visit(d)
        active.remove(p);seen.add(p)
    for p in code:visit(p)
    return len(code),len(entries)

if __name__=='__main__':print('Catalog OK:',validate())
