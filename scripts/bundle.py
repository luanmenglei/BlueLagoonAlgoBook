"""Expand quoted local includes once, keeping system includes and source notices."""
from pathlib import Path
import argparse,re,sys
ROOT=Path(__file__).resolve().parents[1]
CONTENT=(ROOT/'content').resolve()

def bundle(paths):
    seen=set();active=set();parts=[]
    def visit(path):
        path=path.resolve()
        if not path.is_relative_to(CONTENT):raise ValueError(f'Outside content: {path}')
        if path in active:raise ValueError(f'Circular dependency: {path}')
        if path in seen:return
        active.add(path)
        text=path.read_text(encoding='utf-8-sig')
        for inc in re.findall(r'^\s*#include "([^\n]+)"',text,re.M):visit(path.parent/inc)
        active.remove(path);seen.add(path)
        text=re.sub(r'^\s*#pragma once\s*\n','',text,flags=re.M)
        text=re.sub(r'^\s*#include "[^\n]+"\s*\n','',text,flags=re.M)
        parts.append('// '+path.relative_to(CONTENT).as_posix()+'\n'+text.strip()+'\n')
    for p in paths:visit(CONTENT/p)
    return '\n'.join(parts)

if __name__=='__main__':
    ap=argparse.ArgumentParser();ap.add_argument('headers',nargs='+');ap.add_argument('--output',type=Path);a=ap.parse_args()
    result=bundle(a.headers)
    if a.output:a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(result,encoding='utf-8')
    else:sys.stdout.buffer.write(result.encode('utf-8'))
