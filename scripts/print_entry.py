"""Prepare one generated entry; layout belongs in tex/commands.tex."""
from pathlib import Path
from hash import printed_body,code_hash
from typeset import highlighted_code,prose,complexity,tex

def render_entry(root,e,labels,tested):
    lines=[r'\blentry{'+tex(e['title'])+'}{'+labels[e['path']]+'}']
    source=root/'content'/e['path']
    if e['kind']=='note':
        lines += [r'\noindent '+prose(line)+r'\par\smallskip'
                  for line in source.read_text(encoding='utf-8').splitlines() if line.strip()]
        return '\n'.join(lines)+'\n',None
    body=printed_body(source.read_text(encoding='utf-8-sig'));h=code_hash(body)
    dest=root/'build/print'/e['path'];dest.parent.mkdir(parents=True,exist_ok=True)
    dest.write_text(body,encoding='utf-8')
    colored=dest.with_suffix('.tex');colored.write_text(highlighted_code(body),encoding='utf-8')
    deps=[d for d in e['dependencies'] if d!='contest/Base.h']
    if e['path']!='contest/Base.h':deps=['contest/Base.h']+deps
    for d in deps:
        if d not in labels:raise ValueError(f'{e["path"]}: dependency is not printed: {d}')
    for caption,field in [('功能','purpose'),('约定','note'),('调用','usage')]:
        if e.get(field):lines.append(r'\blfield{'+caption+'}{'+prose(e[field])+'}')
    lines.append(r'\blfield{复杂度}{'+complexity(e['complexity'])+'}')
    if deps:lines.append(r'\blfield{依赖}{'+', '.join(r'\S\ref{'+labels[d]+'}' for d in deps)+r'（递归收齐依赖）。}')
    tag='本地回归／对拍' if e['path'] in tested else '独立编译'
    repo=e.get('upstream')
    if repo:origin='KACTL' if repo['repository']=='kactl' else repo['repository']
    else:
        refs=[name for name in ('cp-book','koosaga') if name in e['source']]
        origin='BlueLagoon'+('（参考 '+', '.join(refs)+'）' if refs else '')
    lines.append(r'\blprovenance{'+tex(origin+' / '+e['license']+' / '+tag)+'}{'+h+'}')
    lines.append(r'\blcode{'+colored.relative_to(root).as_posix()+'}')
    return '\n'.join(lines)+'\n',h
