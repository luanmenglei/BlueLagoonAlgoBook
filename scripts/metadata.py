"""Read authoritative notebook annotations from each C++ header."""
from pathlib import Path
import json,re
from selection import load_selection

ROOT=Path(__file__).resolve().parents[1]
TAGS={'title':'title','brief':'purpose','note':'note','usage':'usage',
      'time':'complexity','depends':'dependencies','author':'author',
      'source':'source','license':'license','status':'status'}
REQUIRED={'title','brief','note','time','depends','author','source','license'}

def parse_header(text):
    match=re.match(r'\s*/\*\*(.*?)\*/',text,re.S)
    if not match:raise ValueError('Missing opening notebook annotation block')
    values={};current=None
    for raw in match[1].splitlines():
        line=re.sub(r'^\s*\* ?', '', raw).strip()
        if not line:continue
        if line.startswith('@'):
            parts=line[1:].split(maxsplit=1);tag=parts[0];value=parts[1] if len(parts)>1 else ''
            if tag not in TAGS:raise ValueError(f'Unknown annotation @{tag}')
            if tag in values:raise ValueError(f'Duplicate annotation @{tag}')
            values[tag]=value.strip();current=tag
        elif current:
            values[current]=(values[current]+' '+line).strip()
        else:raise ValueError('Text before first notebook annotation')
    missing=REQUIRED-values.keys()
    if missing:raise ValueError(f'Missing annotations: {sorted(missing)}')
    empty={k for k,v in values.items() if not v}
    if empty:raise ValueError(f'Empty annotations: {sorted(empty)}')
    result={TAGS[k]:v for k,v in values.items()}
    deps=result['dependencies']
    result['dependencies']=[] if deps=='-' else [p.strip() for p in deps.split(',')]
    if any(not p for p in result['dependencies']):raise ValueError('Empty dependency')
    return result

def enrich(chapters):
    attribution=json.loads((ROOT/'third_party/attribution.json').read_text(encoding='utf-8'))
    for c in chapters:
        for e in c['entries']:
            if e['kind']!='code':continue
            try:e.update(parse_header((ROOT/'content'/e['path']).read_text(encoding='utf-8-sig')))
            except ValueError as ex:raise ValueError(f'{e["path"]}: {ex}') from ex
            e['upstream']=attribution.get(e['path'])
    return chapters

def load_chapters():
    return enrich(load_selection())

def entries():
    selected=[e for c in load_chapters() for e in c['entries']]
    known={e['path'] for e in selected}
    excluded=[dict(path=p.relative_to(ROOT/'content').as_posix(),kind='code',print=False)
              for p in sorted((ROOT/'content').rglob('*.h'))
              if p.relative_to(ROOT/'content').as_posix() not in known]
    return selected+enrich([dict(entries=excluded)])[0]['entries']

if __name__=='__main__':print('Header annotations OK:',sum(e['kind']=='code' for e in entries()))
