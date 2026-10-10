"""Read literal chapter/import commands from the maintained LaTeX files."""
from pathlib import Path,PurePosixPath
import re

ROOT=Path(__file__).resolve().parents[1]

def uncomment(text):
    return re.sub(r'(?<!\\)%[^\n]*','',text)

def chapter_order(text):
    text=uncomment(text)
    names=re.findall(r'\\blchapter\s*\{([^{}\n]+)\}',text)
    if len(names)!=len(re.findall(r'\\blchapter\b',text)):
        raise ValueError('Use literal \\blchapter{directory} commands')
    if not names:raise ValueError('No selected chapters in BlueLagoon.tex')
    if len(set(names))!=len(names):raise ValueError('Duplicate chapter')
    if any(not re.fullmatch(r'[A-Za-z0-9_-]+',n) for n in names):
        raise ValueError('Invalid chapter directory')
    return names

def entry_path(chapter,filename,suffix):
    p=PurePosixPath(filename)
    if p.is_absolute() or '..' in p.parts or '\\' in filename or p.suffix!=suffix:
        raise ValueError(f'{chapter}: invalid import {filename}')
    return chapter+'/'+p.as_posix()

def parse_chapter(text,name):
    text=uncomment(text)
    heading=re.search(r'\\section\s*\{([^{}\n]+)\}',text)
    if not heading:raise ValueError(f'{name}: missing \\section{{chapter title}}')
    pattern=r'\\blimport\s*\{([^{}\n]+)\}|\\blnote\s*\{([^{}\n]+)\}\s*\{([^{}\n]+)\}'
    matches=list(re.finditer(pattern,text))
    if len(matches)!=len(re.findall(r'\\bl(?:import|note)\b',text)):
        raise ValueError(f'{name}: use literal \\blimport{{file.h}} or \\blnote{{file.md}}{{title}}')
    es=[]
    for m in matches:
        if m[1]:es.append(dict(path=entry_path(name,m[1],'.h'),kind='code',print=True))
        else:es.append(dict(path=entry_path(name,m[2],'.md'),kind='note',title=m[3],print=True))
    if len({e['path'] for e in es})!=len(es):raise ValueError(f'{name}: duplicate import')
    return dict(name=name,title=heading[1],entries=es)

def load_selection():
    chapters=[]
    for name in chapter_order((ROOT/'BlueLagoon.tex').read_text(encoding='utf-8')):
        chapter=parse_chapter((ROOT/'content'/name/'chapter.tex').read_text(encoding='utf-8'),name)
        for e in chapter['entries']:
            if not (ROOT/'content'/e['path']).is_file():raise ValueError(f'Missing import: {e["path"]}')
        chapters.append(chapter)
    paths=[e['path'] for c in chapters for e in c['entries']]
    if len(set(paths))!=len(paths):raise ValueError('Duplicate import across chapters')
    return chapters
