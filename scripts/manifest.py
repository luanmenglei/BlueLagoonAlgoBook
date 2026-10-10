"""Bind successful verification to the exact code, catalog and test inputs."""
from pathlib import Path
import hashlib
ROOT = Path(__file__).resolve().parents[1]

def source_manifest():
    paths = []
    paths += list((ROOT / 'content').rglob('*.h'))
    paths += list((ROOT / 'tests').glob('test_*.cpp'))
    paths += [ROOT / 'tests/Test.h']
    paths += [ROOT/'scripts'/p for p in ('metadata.py','selection.py','validate.py','check_tools.py','verify.py')]
    return {p.relative_to(ROOT).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
            for p in sorted(paths)}

def print_manifest():
    paths=[ROOT/'BlueLagoon.tex',ROOT/'book.json',ROOT/'third_party/attribution.json',ROOT/'requirements.txt']
    paths+=list((ROOT/'tex').glob('*.tex'))
    paths+=[ROOT/'scripts'/p for p in ('build.py','hash.py','typeset.py','metadata.py','selection.py','print_entry.py')]
    paths+=list((ROOT/'content').rglob('*'))
    return {p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest()
            for p in sorted(paths) if p.is_file()}
