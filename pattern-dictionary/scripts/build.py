#!/usr/bin/env python3
"""Build the standalone pattern dictionary from its single maintained JSON source."""
from pathlib import Path
import argparse,json,re,shutil,subprocess
R=Path(__file__).resolve().parents[1]
def plain(s):
 e={'\\':r'\textbackslash{}','&':r'\&','%':r'\%','$':r'\$','#':r'\#','_':r'\_','{':r'\{','}':r'\}','~':r'\textasciitilde{}','^':r'\textasciicircum{}'}
 return ''.join(e.get(c,c)for c in s)
def rich(s,highlights=(),md=False):
 pat='|'.join(re.escape(x)for x in sorted(highlights,key=len,reverse=True)if '$'not in x)
 out=[]
 for part in re.split(r'(\$[^$]+\$)',s):
  if part.startswith('$')and part.endswith('$'):out.append(part if md else r'\('+part[1:-1]+r'\)');continue
  if not pat:out.append(part if md else plain(part));continue
  last=0
  for m in re.finditer(pat,part):
   out.append(part[last:m.start()] if md else plain(part[last:m.start()]));out.append('**'+m[0]+'**' if md else r'\textbf{'+plain(m[0])+'}');last=m.end()
  out.append(part[last:] if md else plain(part[last:]))
 return ''.join(out)
def linked(cid):return r'\hyperref['+cid+']{'+cid+r'\,(\pageref*{'+cid+r'})}'
def make_tables(d):
 t=d['print_tables'];out=[r'\begingroup\fontsize{9.4}{13.5}\selectfont\setlength{\tabcolsep}{5pt}\renewcommand{\arraystretch}{1.55}'];md=[]
 for key,spec in [('twelvefold',r'll>{\raggedright\arraybackslash}X>{\raggedright\arraybackslash}X>{\raggedright\arraybackslash}X'),('stirling',r'>{\raggedright\arraybackslash}p{28mm}>{\raggedright\arraybackslash}X >{\raggedright\arraybackslash}p{40mm}')]:
  x=t[key];out+=[r'\textbf{\color{Accent}'+rich(x['title'])+r'}\par\vspace{5pt}',rich(x['notation'])+r'\par\vspace{4pt}',r'\begin{tabularx}{\linewidth}{'+spec+r'}\toprule',' & '.join(rich(c)for c in x['columns'])+r'\\\midrule']
  out+=[' & '.join(rich(c)for c in row)+r'\\'for row in x['rows']]
  out+=[r'\bottomrule\end{tabularx}\par\vspace{5pt}',rich(x['boundary'])+r'\par\vspace{5pt}']
  if x.get('algorithm'):out+=[rich(x['algorithm'])+r'\par\vspace{8pt}']
  md+=['### '+x['title'],x['notation'],'\n'.join(['| '+' | '.join(x['columns'])+' |','| '+' | '.join(['---']*len(x['columns']))+' |']+['| '+' | '.join(row)+' |'for row in x['rows']]),x['boundary'],x.get('algorithm','')]
 c=t['chain_detail'];out+=[r'\vspace{5pt}\textbf{\color{Accent}任意定向链：可直接还原的卷积递推}\par',rich(c['definition'])+r'\par',rich(c['recurrence'])+r'\par',rich(c['cdq']),r'\endgroup'];md+=['### 任意定向链：卷积递推',c['definition'],c['recurrence'],c['cdq']]
 (R/'source/tables.tex').write_text('\n'.join(out)+'\n');(R/'source/tables.md').write_text('\n\n'.join(md)+'\n')

def build():
 d=json.loads((R/'source/entries.json').read_text());cards=d['cards'];by={c['id']:c for c in cards};indexes=d['shape_index'];order='MDNCQI';make_tables(d)
 assert len(by)==len(cards)
 for c in cards:
  assert 3<=len(c['keywords'])<=5 and all(x in c['text']for x in c['highlights']),c['id']
  assert not re.search(r'\b(?:AGC|ABC|ARC|CF|QOJ|LOJ)\s*\d|\bP\d{4}',c['text']+' '+c['title'])
 for g in indexes:
  for _,ids in g['items']:assert set(ids)<=set(by)
 pre=r'''\documentclass[10pt,a4paper]{article}
\usepackage{fontspec,xeCJK,amsmath,amssymb,unicode-math}
\setmainfont{texgyretermes-regular.otf}[BoldFont=texgyretermes-bold.otf]
\setmathfont{texgyretermes-math.otf}
\IfFontExistsTF{SimSun}{\setCJKmainfont[AutoFakeBold=2]{SimSun}}{\IfFontExistsTF{Noto Serif CJK SC}{\setCJKmainfont[AutoFakeBold=2]{Noto Serif CJK SC}}{\IfFontExistsTF{Songti SC}{\setCJKmainfont{Songti SC}[BoldFont=Songti SC Bold]}{\setCJKmainfont{FandolSong-Regular.otf}[BoldFont=FandolSong-Bold.otf]}}}
\usepackage[top=14mm,bottom=13mm,left=10mm,right=10mm,headheight=12pt,headsep=3mm,footskip=7mm]{geometry}
\usepackage{multicol,fancyhdr,lastpage,xcolor,needspace,hyperref,booktabs,tabularx,array}
\definecolor{Accent}{HTML}{244A53}\colorlet{Teal}{Accent}\definecolor{Muted}{HTML}{4B5960}\definecolor{Rule}{HTML}{CBD3D6}
\hypersetup{hidelinks,pdftitle={TITLE},pdfauthor={BlueLagoon}}
\pagestyle{fancy}\fancyhf{}
\fancyhead[L]{\scriptsize BlueLagooon 常见题目模式字典}
\fancyhead[R]{\scriptsize 图灵奖速查表 · 题面检索版}
\fancyfoot[L]{\scriptsize DATE · COUNT 条 · M D N C Q I}
\fancyfoot[R]{\scriptsize\thepage\,/\,\pageref*{LastPage}}
\renewcommand{\headrulewidth}{.35pt}
\setlength{\columnsep}{6mm}\setlength{\columnseprule}{.25pt}
\setlength{\parindent}{0pt}\setlength{\parskip}{0pt}\setlength{\emergencystretch}{3em}\tolerance=3000
\newcommand{\grouphead}[1]{\needspace{7\baselineskip}\par\vspace{7pt}{\large\bfseries #1}\par\vspace{5pt}}
\newcommand{\entry}[4]{\begin{minipage}{\linewidth}{\fontsize{8.7}{10.5}\selectfont\bfseries\color{Accent}#3}\hfill{\bfseries #1}\par\vspace{2pt}\textbf{#2}\quad#4\end{minipage}\par\vspace{3pt}{\color{Rule}\hrule}\vspace{3pt}}
\begin{document}\fontsize{9.25}{12}\selectfont
'''.replace('TITLE',d['title']).replace('DATE',d['date']).replace('COUNT',str(len(cards)))
 parts=[pre,r'{\fontsize{18}{23}\selectfont\bfseries BlueLagooon常见题目模式字典\\+图灵奖速查表}\par\vspace{8pt}',r'\textbf{从模糊题面找到精确模型。}先按下方关键词找编号，再核对正文中加粗的约束与区别；括号内为页码。关键词仅作入口，不能代替适用条件。\par\vspace{5pt}',r'复杂度按赛场常用精确实现，数值按机器字／模数运算；$w$ 为机器字长。全文不依赖外部题号。\par\vspace{7pt}']
 sections={s['code']:s for s in d['sections']}
 parts.append(r'\textbf{分区：}\quad'+r'\quad'.join(r'\hyperref[sec'+s+']{'+s+' '+{'M':'矩阵','D':'数据结构','N':'一般难题','C':'计数','Q':'背包与瓶颈','I':'信息论'}[s]+r'\,(\pageref*{sec'+s+r'})}'for s in order)+r'\par')
 parts.append(r'\begin{multicols}{2}\raggedcolumns')
 for g in indexes:
  parts+=[r'\grouphead{'+plain(g['group'])+'}']
  for title,ids in g['items']:
   parts+=[r'\begin{minipage}{\linewidth}\textbf{'+plain(title)+r'}\par{\fontsize{8.5}{11}\selectfont '+r'\hspace{.5em}\allowbreak '.join(linked(cid)for cid in ids)+r'}\end{minipage}\par\vspace{5pt}']
 parts+=[r'\end{multicols}\clearpage',r'\pdfbookmark[0]{完整条目目录}{catalog}\grouphead{完整条目目录}',r'内部编号在各版本间保持稳定；先看条目名，再按页码定位。十二重计数、斯特林行列与链递推见第\pageref*{tables}页。\par',r'\begin{multicols}{3}\fontsize{8.2}{10.4}\selectfont\raggedcolumns']
 for s in order:
  parts+=[r'\needspace{5\baselineskip}\par\vspace{6pt}\textbf{'+plain(s+' '+sections[s]['title'])+r'}\par\vspace{3pt}']
  ordered=[c for c in cards if c['section']==s]
  if s=='C':ordered=[by[cid]for group in d['counting_parts']for cid in group['ids']]
  for c in ordered:
   if s=='C':
    for group in d['counting_parts']:
     if c['id']==group['ids'][0]:parts+=[r'\par\vspace{3pt}\textbf{'+plain(group['title'])+r'}\par']
   parts+=[r'\noindent\hyperref['+c['id']+']{'+c['id']+' '+plain(c['title'])+r'\nobreak\hfill\pageref*{'+c['id']+r'}}\par\vspace{1.5pt}']
 parts+=[r'\end{multicols}\clearpage',r'\begin{multicols}{2}\raggedcolumns']
 for s in order:
  if s=='C':parts.append(r'\needspace{16\baselineskip}')
  sec=sections[s];parts+=[r'\phantomsection\label{sec'+s+r'}\pdfbookmark[0]{'+plain(s+' '+sec['title'])+'}{sec'+s+'}',r'\grouphead{'+plain(s+' '+sec['title'])+'}',r'{\small\color{Muted}'+rich(sec['subtitle'])+r'}\par\vspace{6pt}']
  section_cards=[c for c in cards if c['section']==s]
  if s=='C':section_cards=[by[cid]for group in d['counting_parts']for cid in group['ids']]
  for c in section_cards:
   if s=='C':
    for group in d['counting_parts']:
     if c['id']==group['ids'][0]:
      parts+=[r'\grouphead{'+plain(group['title'])+'}']
      if group['id']=='feasibility':
       for row in d['counting_overview']:
        parts+=[r'\begin{minipage}{\linewidth}\textbf{'+rich(row['model'])+'}：'+rich(row['structure'])+'；'+rich(row['complexity'])+r'\end{minipage}\par\vspace{4pt}']
   parts+=[r'\pdfbookmark[1]{'+plain(c['id']+' '+c['title'])+'}{'+c['id']+'}',r'\entry{\phantomsection\label{'+c['id']+'}'+c['id']+'}{'+plain(c['title'])+'}{'+plain(' · '.join(c['keywords']))+'}{'+rich(c['text'],c['highlights'])+'}']
  if s=='C':
   parts+=[r'\end{multicols}\clearpage\phantomsection\label{tables}\pdfbookmark[0]{计数计算表}{tables}\grouphead{C 计数计算表与链递推}',(R/'source/tables.tex').read_text(),r'\clearpage\begin{multicols}{2}\raggedcolumns']
 parts+=[r'\end{multicols}\end{document}']
 (R/'dictionary.tex').write_text('\n'.join(parts)+'\n')
 md=['# '+d['title'],str(len(cards))+' 条 · 题面检索版 · '+d['date'],'先用检索词定位，再核对正文的精确条件。**检索词可以宽泛，算法适用条件不能宽泛。**','## 从题面找模型']
 for g in indexes:
  md+=['### '+g['group']]+['- **'+title+'**：'+'、'.join('['+cid+' '+by[cid]['title']+'](#'+cid.lower()+')'for cid in ids)for title,ids in g['items']]
 md+=['## 完整目录']
 for s in order:
  md+=['### '+s+' '+sections[s]['title']]
  ordered=[c for c in cards if c['section']==s]
  if s=='C':ordered=[by[cid]for group in d['counting_parts']for cid in group['ids']]
  md+=['- ['+c['id']+' '+c['title']+'](#'+c['id'].lower()+')'for c in ordered]
 for s in order:
  md+=['## '+s+' '+sections[s]['title'],sections[s]['subtitle']]
  section_cards=[c for c in cards if c['section']==s]
  if s=='C':section_cards=[by[cid]for group in d['counting_parts']for cid in group['ids']]
  for c in section_cards:
   if s=='C':
    for group in d['counting_parts']:
     if c['id']==group['ids'][0]:
      md+=['### '+group['title']]
      if group['id']=='feasibility':md+=['- **'+row['model']+'**：'+row['structure']+'；'+row['complexity']for row in d['counting_overview']]
   md+=['<a id="'+c['id'].lower()+'"></a>','**检索：'+' · '.join(c['keywords'])+'**','### '+c['id']+' '+c['title'],rich(c['text'],c['highlights'],md=True)]
  if s=='C':md+=[(R/'source/tables.md').read_text()]
 (R/'dictionary.md').write_text('\n\n'.join(md)+'\n')
 deriv=['# 计数条目的公式与推导','正文以对象、转化和操作步骤为主；本文件保留长公式供维护时核对。']
 for c in cards:
  if c.get('derivation'):deriv+=['## '+c['id']+' '+c['title'],c['derivation']]
 (R/'research/derivations.md').write_text('\n\n'.join(deriv)+'\n')
 refs=['# 来源记录','外部题号与链接只在此记录及网页来源折叠栏中保留。']
 for r in d['references']:refs+=['- '+r['id']+' '+('['+r['title']+']('+r['url']+')'if r.get('url')else r['title'])+'：'+r.get('detail','')]
 refs+=['## 条目对应来源']+['- '+c['id']+' '+c['title']+'：'+', '.join(c['sources'])for c in cards]
 (R/'research/sources.md').write_text('\n\n'.join(refs)+'\n')
 print(f'Generated dictionary.md, dictionary.tex and research notes; {len(cards)} entries.')
if __name__=='__main__':
 ap=argparse.ArgumentParser();ap.add_argument('--pdf',action='store_true');args=ap.parse_args();build()
 if args.pdf:
  if shutil.which('xelatex'):
   for _ in range(3):subprocess.run(['xelatex','-interaction=nonstopmode','-halt-on-error','dictionary.tex'],cwd=R,check=True,stdout=subprocess.DEVNULL)
  elif shutil.which('tectonic'):subprocess.run(['tectonic','dictionary.tex'],cwd=R,check=True)
  else:raise SystemExit('Install XeLaTeX or Tectonic to build PDF; the Markdown and TeX have been generated.')
