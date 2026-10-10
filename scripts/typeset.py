"""Print-focused C++ highlighting and explicit Markdown math support."""
import re
from pygments import format as pygments_format
from pygments.lexers import CppLexer
from pygments.formatters import LatexFormatter
from pygments.style import Style
from pygments.token import Token,Keyword,Comment,String,Number,Name,Operator

class PrintStyle(Style):
    background_color='#ffffff'
    styles={Token:'#111111',Keyword:'bold #18354d',Comment:'italic #505050',
            Comment.Preproc:'#35503b',String:'#703728',Number:'#403468',
            Name.Function:'bold #18354d',Operator:'#111111'}

FORMATTER=LatexFormatter(style=PrintStyle,commandprefix='BL',nowrap=True)

def highlighted_code(body):
    # expandtabs(4) makes source alignment independent of the PDF verbatim engine.
    expanded=body.expandtabs(4)
    tokens=list(CppLexer(stripnl=False,ensurenl=False).get_tokens(expanded))
    if ''.join(value for _,value in tokens)!=expanded:
        raise ValueError('Highlighting lexer altered the printed source')
    return pygments_format(tokens,FORMATTER)

def style_definitions():return FORMATTER.get_style_defs()

def tex(s):
    d={'\\':r'\textbackslash{}','{':r'\{','}':r'\}',
       '$':r'\$','&':r'\&','#':r'\#','%':r'\%',
       '_':r'\_','~':r'\textasciitilde{}','^':r'\textasciicircum{}'}
    return ''.join(d.get(c,c) for c in str(s))

def prose(text):
    """$inline math$, $$display math$$, and `code`; escape ordinary text."""
    chunks=re.split(r'(\$\$[^\n]*?\$\$|\$(?!\$)[^$\n]+?\$|`[^`\n]+`)',text)
    result=[]
    for part in chunks:
        if part.startswith('$$') and part.endswith('$$'):result.append(r'\['+part[2:-2]+r'\]')
        elif part.startswith('$') and part.endswith('$'):result.append(part)
        elif part.startswith('`') and part.endswith('`'):
            body=''.join(tex(c)+(r'\allowbreak{}' if c in '(),;[]{}=<>:' else '') for c in part[1:-1])
            result.append(r'\texttt{'+body+'}')
        else:
            result.append(''.join(tex(c)+(r'\allowbreak{}' if c in '_,();[]' else '') for c in part))
    return ''.join(result)

def complexity(text):
    """Render balanced O(...) expressions, preserving surrounding Chinese prose."""
    parts=[];at=0
    while True:
        begin=text.find('O(',at)
        if begin<0:parts.append(tex(text[at:]));break
        parts.append(tex(text[at:begin]));end=begin+2;depth=1
        while end<len(text) and depth:
            depth+=(text[end]=='(')-(text[end]==')');end+=1
        if depth:parts.append(tex(text[begin:]));break
        expr=text[begin+2:end-1]
        expr=re.sub(r'\^\(([^()]*)\)',r'^{\1}',expr)
        expr=re.sub(r'\^([a-zA-Z0-9]+)',r'^{\1}',expr)
        expr=re.sub(r'\bsqrt\(([^()]*)\)',r'\\sqrt{\1}',expr)
        expr=re.sub(r'\bsqrt\s+([a-zA-Z])',r'\\sqrt{\1}',expr)
        expr=re.sub(r'\b(log|ln|exp|alpha|min|max)\b',r'\\\1',expr)
        expr=re.sub(r'\b(ceil|depth|modulus|pivot|mod)\b',r'\\operatorname{\1}',expr)
        expr=re.sub(r'[\u4e00-\u9fff]+',lambda m:r'\text{'+m[0]+'}',expr)
        expr=expr.replace('*',r'\cdot ')
        parts.append(r'$\mathcal{O}\bigl('+expr+r'\bigr)$');at=end
    return ''.join(parts)
