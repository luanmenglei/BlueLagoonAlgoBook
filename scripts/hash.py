"""Hash the exact printed body, ignoring comments and non-string whitespace."""
from pathlib import Path
import hashlib,re,sys

def printed_body(text):
    text=re.sub(r'\A\s*/\*\*.*?\*/\s*','',text,flags=re.S)
    text=re.sub(r'^\s*#pragma once\s*\n','',text,flags=re.M)
    text=re.sub(r'^\s*#include "[^\n]+"\s*\n','',text,flags=re.M)
    return text.strip()+'\n'

TOKEN=re.compile(r'R"(?P<delim>[^ ()\\\t\r\n]{0,16})\(.*?\)(?P=delim)"|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|//[^\n]*|/\*.*?\*/|[A-Za-z_]\w*|(?:\d|\.\d)(?:[eEpP][+-]|[\w.\'])*|>>=|<<=|->\*|\.\.\.|\+\+|--|->|::|<=|>=|==|!=|&&|\|\||<<|>>|\+=|-=|\*=|/=|%=|&=|\|=|\^=|[^\s]',re.S)
def code_hash(text):
    tokens=[m[0] for m in TOKEN.finditer(printed_body(text)) if not m[0].startswith(('//','/*'))]
    return hashlib.sha256('\0'.join(tokens).encode()).hexdigest()[:6]

if __name__=='__main__':
    s=Path(sys.argv[1]).read_text(encoding='utf-8-sig') if len(sys.argv)>1 else sys.stdin.read()
    print(code_hash(s))
