"""Small checks for hashes and dependency expansion, run alongside algorithm tests."""
from hash import code_hash
from bundle import bundle
from metadata import parse_header
from selection import chapter_order,parse_chapter

def check_tools():
    assert chapter_order('% \\blchapter{excluded}\n\\blchapter{flow}\n\\blchapter{graph}')==['flow','graph']
    selected=parse_chapter('\\section{匹配}\n% \\blimport{Hidden.h}\n\\blimport{First.h}\n\\columnbreak\n\\blnote{Guide.md}{说明}\n\\blimport{Second.h}','flow')
    assert [e['path'] for e in selected['entries']]==['flow/First.h','flow/Guide.md','flow/Second.h']
    assert selected['entries'][1]['title']=='说明'
    for broken in ('\\section{图}\\blimport{../Hidden.h}','\\section{图}\\blimport{A.h}\\blimport{A.h}','\\section{图}\\blimport{A.h'):
        try:parse_chapter(broken,'flow')
        except ValueError:pass
        else:raise AssertionError('Invalid import accepted')
    sample='''/**
 * @title Example
 * @brief Solve a problem.
 *   Continued description.
 * @note $x^2$ and `a[i]` remain literal.
 * @time O(n)
 * @depends contest/Base.h, algebra/NTT.h
 * @author BlueLagoon
 * @source Local
 * @license CC0-1.0
 */
int main() {}'''
    info=parse_header(sample)
    assert info['purpose']=='Solve a problem. Continued description.'
    assert info['dependencies']==['contest/Base.h','algebra/NTT.h']
    assert info['note']=='$x^2$ and `a[i]` remain literal.'
    assert parse_header(sample.replace('@source Local','@source\n * Local'))['source']=='Local'
    for broken in (sample.replace('@brief','@typo'),sample.replace(' * @time O(n)\n',''),sample.replace('@title Example','@title Example\n * @title Duplicate'),sample.replace('@source Local','@source')):
        try:parse_header(broken)
        except ValueError:pass
        else:raise AssertionError('Invalid annotation accepted')
    assert code_hash('int a=1; // comment\na ++;')==code_hash('int a = 1; /* x */ a++;')
    for a,b in [('a++','a + +'),('int ab;','int a b;'),('"a b"','"ab"'),
                ('R"x(a b)x"','R"x(ab)x"'),('0x10','0 x10'),('1e+2','1 e + 2')]:
        assert code_hash(a)!=code_hash(b),(a,b)
    source=bundle(['algebra/FPS.h','algebra/NTT.h'])
    assert source.count('// contest/Base.h\n')==1
    assert source.count('// algebra/NTT.h\n')==1
    assert '#pragma once' not in source
    assert '#include "' not in source
    assert '#include<bits/stdc++.h>' in source
    return True

if __name__=='__main__':check_tools();print('Maintenance tools OK')
