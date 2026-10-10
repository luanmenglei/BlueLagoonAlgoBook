from pathlib import Path
import re, shutil, subprocess
ROOT=Path(__file__).resolve().parents[1]
exe=shutil.which('clang-format')
if not exe: raise SystemExit('clang-format is required for formatting only')
for p in (ROOT/'content').rglob('*.h'):
    subprocess.run([exe,'-i',str(p)],check=True)
    s=p.read_text(encoding='utf-8')
    # Protect literals, comments, and system include names before spacing postfix operators.
    pattern=r'R"([^ ()\\\t\r\n]{0,16})\(.*?\)\1"|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|//[^\n]*|/\*.*?\*/|^[ \t]*#include[^\n]*|(?<=[\w\]\)])(\+\+|--)'
    s=re.sub(pattern,lambda m:' '+m[2] if m[2] else m[0],s,flags=re.S|re.M)
    s=s.replace('#include <bits/stdc++.h>','#include<bits/stdc++.h>')
    p.write_text(s,encoding='utf-8')
print('Formatted all headers')
