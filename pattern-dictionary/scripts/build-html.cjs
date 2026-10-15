#!/usr/bin/env node
'use strict';
const fs = require('node:fs');
const path = require('node:path');
const ROOT = path.resolve(__dirname, '..');
const ORDER = 'MDNCQI'.split('');
const esc = value => String(value ?? '').replace(/[&<>"']/g, ch => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[ch]));
const SYNONYMS = [
  ['树链','树上路径','树上链'],
  ['不同颜色','不同颜色数','颜色种类','颜色种数','不同色数','颜色数','数颜色','distinct'],
  ['双子树','两子树','两棵树子树交','子树交'],
  ['压位','位集','bitset'],
  ['无标号','无标签','不可区分'],
  ['有标号','有标签','可区分'],
  ['逆序对','逆序数'],
  ['拓扑序','拓扑排序'],
  ['斯特林','stirling'],
  ['最大公约数','gcd'],
  ['可行性背包','背包可行性'],
  ['第k小','kth']
];
function normalize(value) {
  let text = String(value ?? '').normalize('NFKC').toLowerCase();
  const aliases = SYNONYMS.flatMap(group => group.map(alias => [alias.toLowerCase(), group[0].toLowerCase()])).sort((a,b) => b[0].length-a[0].length);
  const mapping = new Map(aliases);
  const regex = new RegExp(aliases.map(([alias]) => alias.replace(/[.*+?^${}()|[\]\\]/g,'\\$&')).join('|'),'g');
  return text.replace(regex, alias => mapping.get(alias)).replace(/第\s*k\s*小/g,'第k小').replace(/\s+/g,' ').trim();
}
function bigrams(value) {
  const out = new Set();
  for (const word of normalize(value).split(/[^\p{L}\p{N}]+/u)) {
    const chars = [...word];
    for (let i=0;i+1<chars.length;i++) out.add(chars[i]+chars[i+1]);
  }
  return out;
}
function selectMatches(records, query, section='') {
  const terms = normalize(query).split(/\s+/).filter(Boolean);
  const available = records.filter(record => !section || record.section===section);
  const direct = available.filter(record => terms.every(term => record.search.includes(term)));
  if (direct.length || !terms.length) return {ids:direct.map(record=>record.id),approximate:false};
  const scored = [];
  for (const record of available) {
    const candidate = bigrams(record.fuzzy);
    let score=1,valid=true;
    for (const term of terms) {
      if (record.search.includes(term)) continue;
      const pairs = bigrams(term);
      if (pairs.size<3) {valid=false;break;}
      let hits=0;
      for (const pair of pairs) if (candidate.has(pair)) hits++;
      const ratio=hits/pairs.size;
      if (hits<3 || ratio<0.75) {valid=false;break;}
      score=Math.min(score,ratio);
    }
    if (valid) scored.push({id:record.id,score});
  }
  const ids=scored.sort((a,b)=>b.score-a.score).slice(0,12).map(record=>record.id);
  return {ids,approximate:ids.length>0};
}
function runSearchTests(records=[]) {
  const assert=require('node:assert/strict');
  const sample=[
    {id:'D01',section:'D',search:normalize('树链 不同颜色 静态 精确 祖先'),fuzzy:'树链的不同颜色数 祖先链'},
    {id:'D02',section:'D',search:normalize('两棵树子树交 最大公约数 线段树合并'),fuzzy:'两棵树子树交 最大公约数'},
    {id:'C01',section:'C',search:normalize('无标号 斯特林 集合划分'),fuzzy:'无标号集合划分'},
    {id:'Q01',section:'Q',search:normalize('可行性背包 压位 总体积'),fuzzy:'可行性背包 压位'}
  ];
  assert.deepEqual(selectMatches(sample,'树上路径 颜色种类').ids,['D01']);
  assert.deepEqual(selectMatches(sample,'双子树 gcd').ids,['D02']);
  assert.deepEqual(selectMatches(sample,'无标签 stirling').ids,['C01']);
  assert.deepEqual(selectMatches(sample,'bitset').ids,['Q01']);
  assert.deepEqual(selectMatches(sample,'树链','C').ids,[]);
  assert.deepEqual(selectMatches(sample,'不存在的宇宙飞船').ids,[]);
  assert.equal(selectMatches(sample,'颜色').approximate,false);
  const approximate=selectMatches(sample,'树链不同颜色');
  assert.equal(approximate.approximate,true);
  assert.deepEqual(approximate.ids,['D01']);
  assert.equal(selectMatches(sample,'树恋').approximate,false);
  assert.equal(normalize('不可区分 可区分'),'无标号 有标号');
  if(records.length) {
    for(const section of ORDER) assert(selectMatches(records,'',section).ids.every(id=>records.find(r=>r.id===id).section===section));
    assert.deepEqual(selectMatches(records,'zzzz不存在的宇宙飞船zzzz').ids,[]);
    assert(selectMatches(records,'', '').ids.length===records.length);
  }
  return 12;
}
function build() {
  const data = JSON.parse(fs.readFileSync(path.join(ROOT,'source/entries.json'),'utf8'));
  const katex = require(path.join(ROOT,'assets/katex/katex.min.js'));
  const byId = new Map(data.cards.map(card=>[card.id,card]));
  if(byId.size!==data.cards.length) throw new Error('Duplicate card IDs');
  const refs = new Map((data.references||[]).map(ref=>[ref.id,ref]));
  const sections=ORDER.map(code=>data.sections.find(section=>section.code===code));
  if(sections.some(section=>!section)) throw new Error('Missing MDNCQI section');
  let formulaCount=0,highlightCount=0;
  const records=[];
  const anchors=new Set();
  function anchor(id) {if(anchors.has(id))throw new Error('Duplicate anchor: '+id);anchors.add(id);return esc(id);}
  function math(formula,displayMode=false) {
    formulaCount++;
    return '<span class="'+(displayMode?'formula-block':'formula-inline')+'">'+katex.renderToString(formula,{displayMode,throwOnError:true,strict:code=>['unicodeTextInMathMode','unknownSymbol'].includes(code)?'ignore':'error',output:'htmlAndMathml',trust:false})+'</span>';
  }
  function rich(value,highlights=[]) {
    const source=String(value??'');
    const ranges=[];
    for(const highlight of highlights) {
      if(!highlight || !source.includes(highlight))throw new Error('Highlight is not an exact substring: '+highlight);
      let start=0;
      while((start=source.indexOf(highlight,start))!==-1){ranges.push([start,start+highlight.length]);start+=highlight.length;}
    }
    ranges.sort((a,b)=>a[0]-b[0]);
    const merged=[];
    for(const range of ranges){const last=merged.at(-1);if(last&&range[0]<=last[1])last[1]=Math.max(last[1],range[1]);else merged.push(range.slice());}
    if((source.match(/(?<!\\)\$/g)||[]).length%2)throw new Error('Unbalanced formula delimiters: '+source);
    const pieces=source.split(/(?<!\\)\$/);
    let offset=0;
    return pieces.map((piece,index)=>{
      const inside=merged.filter(([start,end])=>start<offset+piece.length&&end>offset);
      let result='';
      if(index%2){result=math(piece);if(inside.length){result='<strong>'+result+'</strong>';highlightCount++;}}
      else {
        let cursor=0;
        for(const [start,end] of inside){const left=Math.max(0,start-offset),right=Math.min(piece.length,end-offset);result+=esc(piece.slice(cursor,left))+'<strong>'+esc(piece.slice(left,right))+'</strong>';cursor=right;highlightCount++;}
        result+=esc(piece.slice(cursor));
      }
      offset+=piece.length+1;
      return result;
    }).join('');
  }
  function sourceBlock(ids) {
    const sourceRefs=[...new Set(ids||[])].map(id=>typeof id==='object'?id:refs.get(id)).filter(Boolean);
    if(!sourceRefs.length)return '';
    return '<details class="sources"><summary>来源与核对资料（'+sourceRefs.length+'）</summary><ul>'+sourceRefs.map(ref=>'<li>'+(ref.url?'<a href="'+esc(ref.url)+'" target="_blank" rel="noopener noreferrer">'+esc(ref.title)+'</a>':esc(ref.title))+(ref.detail?'<span class="source-detail">'+esc(ref.detail)+'</span>':'')+'</li>').join('')+'</ul></details>';
  }
  function keywordRow(keywords) {
    return '<div class="keywords" aria-label="检索词">'+keywords.map(keyword=>'<button type="button" class="keyword" data-keyword="'+esc(keyword)+'"><strong>'+esc(keyword)+'</strong></button>').join('')+'</div>';
  }
  function cardHTML(card,level=3) {
    if(!Array.isArray(card.keywords)||card.keywords.length<3||card.keywords.length>5)throw new Error('Expected 3–5 keywords: '+card.id);
    records.push({id:card.id,section:card.section,kind:'model',search:normalize([card.id,card.title,card.keywords.join(' '),card.text].join(' ')),fuzzy:[card.title,...card.keywords].join(' ')});
    return '<article class="searchable model-card" id="'+anchor(card.id)+'" data-kind="model">'+keywordRow(card.keywords)+'<h'+level+' class="card-title"><a class="card-id" href="#'+esc(card.id)+'" aria-label="定位 '+esc(card.id)+'">'+esc(card.id)+'</a>'+rich(card.title)+'</h'+level+'><p class="card-body">'+rich(card.text,card.highlights||[])+'</p>'+(card.derivation?'<p class="derivation-link"><a href="research/derivations.md">查看公式推导与边界</a></p>':'')+sourceBlock(card.sources)+'</article>';
  }
  function tableHTML(key,table,level=4) {
    records.push({id:key,section:'C',kind:'table',search:normalize([table.title,table.columns,table.rows,table.notation,table.boundary,table.algorithm].flat(Infinity).filter(Boolean).join(' ')),fuzzy:table.title+' '+table.columns.join(' ')});
    const body='<div class="table-scroll" role="region" aria-label="'+esc(table.title)+'" tabindex="0"><table><thead><tr>'+table.columns.map(column=>'<th scope="col">'+rich(column)+'</th>').join('')+'</tr></thead><tbody>'+table.rows.map(row=>'<tr>'+row.map((cell,index)=>(index===0?'<th scope="row">':'<td>')+rich(cell)+(index===0?'</th>':'</td>')).join('')+'</tr>').join('')+'</tbody></table></div>';
    const extra=['notation','boundary','algorithm'].filter(key=>table[key]).map(key=>'<p class="table-note">'+rich(table[key])+'</p>').join('');
    return '<article class="searchable table-card" id="'+anchor(key)+'" data-kind="table"><h'+level+' class="card-title">'+rich(table.title)+'</h'+level+'>'+body+extra+sourceBlock(table.sources)+'</article>';
  }
  function chainHTML(chain,level=4) {
    const title='有向链拓扑序计数：容斥与卷积递推',id='chain-recurrence';
    records.push({id,section:'C',kind:'table',search:normalize([title,chain.model,chain.definition,chain.recurrence,chain.cdq,chain.proof,'链递推'].join(' ')),fuzzy:title});
    const formula=String(chain.recurrence).match(/\$([\s\S]*?)\$/);
    return '<article class="searchable chain-card" id="'+anchor(id)+'" data-kind="table"><h'+level+' class="card-title">'+title+'</h'+level+'><p>'+rich(chain.model)+'</p><p>'+rich(chain.definition)+'</p>'+(formula?math(formula[1],true):'<p>'+rich(chain.recurrence)+'</p>')+'<p>'+rich(chain.cdq)+'</p><p><strong>为什么成立：</strong>'+rich(chain.proof)+'</p>'+sourceBlock(chain.sources)+'</article>';
  }
  const CParts=Array.isArray(data.counting_parts)?data.counting_parts.map(part=>({...part,ids:[...part.ids]})):[];
  if(CParts.length){
    const ids=CParts.flatMap(part=>part.ids),expected=data.cards.filter(card=>card.section==='C').map(card=>card.id);
    if(new Set(ids).size!==ids.length||ids.some(id=>!expected.includes(id)))throw new Error('Invalid or duplicate counting-part card');
    const remainder=expected.filter(id=>!ids.includes(id));
    (CParts.find(part=>part.id==='patterns')||CParts.at(-1)).ids.push(...remainder);
  }
  function orderedCards(section){return section==='C'&&CParts.length?CParts.flatMap(part=>part.ids.map(id=>byId.get(id))):data.cards.filter(card=>card.section===section);}
  function overviewHTML() {
    const rows=data.counting_overview||[];
    if(!rows.length)return '';
    const parsed=rows.map(row=>({cells:Array.isArray(row)?row:[row.model??row.title??row.problem??'',row.structure??row.condition??row.method??'',row.complexity??row.time??row.cost??''],ids:Array.isArray(row)?[]:(row.ids||[])}));
    return '<aside class="counting-overview" aria-label="计数可行性速览"><h4>计数可行性速览</h4><div class="table-scroll" tabindex="0"><table><thead><tr><th scope="col">问题模型</th><th scope="col">可利用的结构</th><th scope="col">方法与复杂度</th></tr></thead><tbody>'+parsed.map(row=>'<tr>'+row.cells.map((cell,index)=>(index===0?'<th scope="row">':'<td>')+rich(cell)+(index===0&&row.ids.length?'<span class="overview-refs">'+row.ids.map(id=>'<a class="jump" href="#'+esc(id)+'">'+esc(id)+'</a>').join(' · ')+'</span>':'')+(index===0?'</th>':'</td>')).join('')+'</tr>').join('')+'</tbody></table></div></aside>';
  }
  function extensionsHTML(level=4){return '<p class="table-conventions">'+rich(data.print_tables.conventions)+'</p>'+tableHTML('twelvefold',data.print_tables.twelvefold,level)+tableHTML('stirling',data.print_tables.stirling,level)+chainHTML(data.print_tables.chain_detail,level);}
  const counts=Object.fromEntries(ORDER.map(code=>[code,data.cards.filter(card=>card.section===code).length]));
  let content='';
  for(const section of sections) {
    content+='<section class="model-section" id="'+anchor('section-'+section.code)+'" data-section="'+section.code+'"><div class="section-heading"><h2>'+section.code+' · '+esc(section.title)+'</h2><span>'+counts[section.code]+' 条</span></div><p class="section-note">'+rich(section.subtitle)+'</p>';
    if(section.code==='C'&&CParts.length) {
      CParts.forEach((part,index)=>{
        content+='<div class="counting-part" id="'+anchor('counting-'+part.id)+'"><h3 class="part-heading">'+esc(part.title)+'</h3>'+(index===0?overviewHTML():'')+part.ids.map(id=>cardHTML(byId.get(id),4)).join('');
        if(index===CParts.length-1)content+=extensionsHTML(4);
        content+='</div>';
      });
    }else{content+=orderedCards(section.code).map(card=>cardHTML(card)).join('');if(section.code==='C')content+=overviewHTML()+extensionsHTML(3);}
    content+='</section>';
  }
  const shapeIndex=Array.isArray(data.shape_index)?data.shape_index:[];
  const shapeHTML='<details class="directory shape-directory" open><summary>从题面找模型 <span>按模糊线索定位</span></summary><div class="shape-groups">'+shapeIndex.map(group=>'<section class="shape-group"><h3>'+esc(group.group)+'</h3><ul>'+group.items.map(([label,ids])=>{
    for(const id of ids)if(!byId.has(id))throw new Error('Unknown shape-index target: '+id);
    return '<li><strong>'+esc(label)+'</strong><div class="shape-targets">'+ids.map(id=>'<a class="jump" href="#'+esc(id)+'" title="'+esc(byId.get(id).title)+'">'+esc(id)+' <span>'+esc(byId.get(id).title)+'</span></a>').join('')+'</div></li>';
  }).join('')+'</ul></section>').join('')+'</div></details>';
  function tocLinks(code){
    const link=card=>'<li><a class="jump" href="#'+esc(card.id)+'"><b>'+esc(card.id)+'</b> '+esc(card.title)+'</a></li>';
    return code==='C'&&CParts.length?CParts.map(part=>'<li class="toc-part-label">'+esc(part.title)+'</li>'+part.ids.map(id=>link(byId.get(id))).join('')).join(''):orderedCards(code).map(link).join('');
  }
  const tocHTML='<details class="directory full-directory"><summary>全部条目目录 <span>'+data.cards.length+' 条 · 3 个展开</span></summary><div class="toc-sections">'+sections.map(section=>'<details class="toc-section"><summary>'+section.code+' · '+esc(section.title)+' <span>'+counts[section.code]+' 条</span></summary><ol>'+tocLinks(section.code)+(section.code==='C'?'<li><a class="jump" href="#twelvefold">十二重计数表</a></li><li><a class="jump" href="#stirling">斯特林数行／列计算表</a></li><li><a class="jump" href="#chain-recurrence">有向链拓扑序计数递推</a></li>':'')+'</ol></details>').join('')+'</div></details>';
  const css=`:root{color-scheme:light;--ink:#183341;--muted:#5c707a;--edge:#d8e3e6;--accent:#086c65;--paper:#fff;--wash:#edf5f4}*{box-sizing:border-box}html{scroll-behavior:smooth;scroll-padding-top:128px}body{margin:0;background:#f3f7f7;color:var(--ink);font:16px/1.85 system-ui,-apple-system,"PingFang SC","Microsoft YaHei",sans-serif}main{max-width:1160px;margin:auto;padding:38px 28px 70px}h1{font-size:clamp(24px,3vw,35px);line-height:1.45;margin:0 0 12px}h2{font-size:24px;line-height:1.5;margin:0}a{color:var(--accent);text-decoration:none}a:hover{text-decoration:underline}a:focus-visible,button:focus-visible,input:focus-visible,select:focus-visible,summary:focus-visible,[tabindex]:focus-visible{outline:3px solid #73bcb5;outline-offset:3px}nav{display:flex;gap:9px 23px;flex-wrap:wrap;margin:14px 0}.intro,.section-note{color:var(--muted);font-size:14px}.intro{max-width:960px;margin:11px 0}.guide{padding:13px 17px;border-left:3px solid var(--accent);background:var(--wash);font-size:14px}.directory{margin:16px 0;border:1px solid var(--edge);border-radius:9px;background:white}.directory>summary{padding:12px 18px;cursor:pointer;font-weight:750}.directory summary span{font-weight:400;font-size:13px;color:var(--muted);margin-left:12px}.shape-groups{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:4px 28px;padding:0 20px 20px}.shape-group h3{font-size:16px;margin:13px 0 7px;color:#244f58}.shape-group ul{padding:0;list-style:none;margin:0}.shape-group li{border-top:1px solid #e9eeee;padding:9px 0}.shape-group li>strong{font-size:14px}.shape-targets{display:flex;gap:4px 8px;flex-wrap:wrap;margin-top:5px}.shape-targets a{font-size:12px;line-height:1.55;padding:3px 6px;background:#f2f7f7;border-radius:4px}.shape-targets a span{color:#46616a}.toc-sections{padding:0 20px 17px;display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:6px 25px;align-items:start}.toc-section{padding:6px 0}.toc-section summary{cursor:pointer;font-size:15px;font-weight:650}.toc-section ol{padding-left:0;list-style:none;margin:8px 0}.toc-section li{font-size:13px;line-height:1.7;padding:3px 0}.toc-section b{display:inline-block;min-width:35px}.toc-part-label{font-weight:750;color:#244f58;margin-top:8px}.overview-refs{display:block;font-size:12px;margin-top:4px}.tools{position:sticky;top:0;z-index:5;background:#f3f7f7f5;backdrop-filter:blur(8px);border-bottom:1px solid var(--edge);padding:13px 0 9px}.search-row{display:grid;grid-template-columns:1fr auto auto;gap:9px}input,select,button{font:inherit;min-width:0}input,select,.reset{border:1px solid #afc1c6;border-radius:6px;padding:9px 12px;background:white;color:var(--ink)}.reset{cursor:pointer;font-size:14px}.results-line{font-size:13px;color:var(--muted);margin:6px 0 0;line-height:1.6}.approximation{border:1px solid #e0bf8f;background:#fff8eb;color:#745016;padding:10px 14px;border-radius:7px;margin:15px 0;font-size:14px}.section-heading{display:flex;justify-content:space-between;align-items:center;gap:14px;margin-top:38px;border-bottom:2px solid #aecdc9;padding-bottom:9px}.section-heading>span{font-size:14px;color:var(--muted);white-space:nowrap}.section-note{margin:9px 0 16px}.part-heading{font-size:20px;border-left:4px solid var(--accent);padding:3px 12px;margin:25px 0 14px}.model-card,.table-card,.chain-card{background:var(--paper);border:1px solid var(--edge);border-radius:9px;padding:17px 22px;margin:12px 0;break-inside:avoid;overflow-wrap:anywhere}.model-card:target,.table-card:target,.chain-card:target{border-color:var(--accent);box-shadow:0 0 0 2px #086c6520}.model-card.approximate,.table-card.approximate,.chain-card.approximate{border-color:#d3b17e}.keywords{display:flex;gap:5px 8px;flex-wrap:wrap;margin:0 0 9px}.keyword{border:0;border-radius:4px;background:var(--wash);color:#175c57;font-size:12px;line-height:1.5;padding:3px 7px;cursor:pointer}.keyword:hover{background:#dbeeea}.keyword strong{font-weight:750}.card-title{font-size:18px;line-height:1.65;margin:0 0 8px;overflow-wrap:anywhere}.card-id{font-size:13px;letter-spacing:.03em;display:inline-block;margin-right:11px;padding:1px 7px;background:#edf3f5;border-radius:4px;font-weight:750;vertical-align:2px}.card-body strong{font-weight:750;color:#102f39}.model-card p,.chain-card p{margin:7px 0}.derivation-link{font-size:13px}.sources{border-top:1px solid #edf1f2;margin-top:12px;padding-top:8px;color:var(--muted);font-size:12px}.sources summary{cursor:pointer;width:fit-content}.sources ul{padding-left:19px;margin:7px 0 2px}.sources li{margin:7px 0}.source-detail{display:block}.formula-inline{display:inline-block;vertical-align:middle;max-width:100%;overflow-x:auto;overflow-y:hidden;padding:2px 0}.formula-block{display:block;max-width:100%;overflow-x:auto;overflow-y:hidden;padding:8px 0}.katex{font-size:1.04em}.katex-display{margin:.35em 0}.table-scroll{overflow-x:auto;margin:13px 0;border:1px solid var(--edge);border-radius:5px}table{border-collapse:collapse;width:100%;min-width:700px;font-size:14px}th,td{padding:11px 12px;text-align:left;vertical-align:middle;border-bottom:1px solid var(--edge);border-right:1px solid var(--edge)}thead th{background:#edf4f4;color:#204b56}tbody th{background:#f8fafb;min-width:110px}tr:last-child td,tr:last-child th{border-bottom:0}th:last-child,td:last-child{border-right:0}table .formula-inline{overflow:visible;max-width:none}.table-note,.table-conventions{font-size:14px}.table-conventions{padding:12px 16px;background:var(--wash);border-left:3px solid #5f9e95;margin-top:25px}.counting-overview{padding:3px 0}.counting-overview h4{font-size:16px;margin:10px 0}.empty{padding:30px;background:white;border:1px dashed #afc4c8;border-radius:8px;text-align:center;color:var(--muted);margin-top:22px}[hidden]{display:none!important}footer{margin-top:42px;font-size:13px;color:var(--muted);border-top:1px solid var(--edge);padding-top:18px}.top-link{float:right}@media(max-width:720px){main{padding:25px 14px 45px}.shape-groups,.toc-sections{grid-template-columns:1fr}.search-row{grid-template-columns:1fr auto}.search-row input{grid-column:1/-1}input,select{font-size:15px}.model-card,.table-card,.chain-card{padding:15px}.card-title{font-size:17px}.section-heading{margin-top:27px}.shape-targets a span{display:none}html{scroll-padding-top:175px}.directory summary span{display:block;margin-left:15px;font-size:12px}}@media print{body{background:white;font-size:10pt}main{max-width:none;padding:0}.tools,nav,.results-line,.sources,.empty,footer,.directory,.guide,.approximation,.derivation-link{display:none!important}.model-card,.table-card,.chain-card{border:0;border-radius:0;padding:5px 0;margin:8px 0}.keyword{background:none;padding:0 6px 0 0;font-size:8pt}.keywords{margin-bottom:3px}.formula-inline,.formula-block,.table-scroll{overflow:visible}.section-heading{margin-top:18px}.table-scroll{border-radius:0}table{min-width:0;font-size:8pt}.katex{font-size:1em}h1{font-size:20pt}h2{font-size:15pt}.part-heading{font-size:12pt}.card-title{font-size:11pt}.card-id{padding:0;background:none}.intro{font-size:9pt}}`;
  const browserScript=`const SYNONYMS=${JSON.stringify(SYNONYMS)};${normalize.toString()};${bigrams.toString()};${selectMatches.toString()};
const records=${JSON.stringify(records).replace(/</g,'\\u003c')};
const search=document.getElementById('search'),selector=document.getElementById('section-filter'),articles=[...document.querySelectorAll('.searchable')],sections=[...document.querySelectorAll('.model-section')];
function filter(){const result=selectMatches(records,search.value,selector.value),visible=new Set(result.ids);for(const directory of document.querySelectorAll('.directory'))directory.hidden=!!(search.value.trim()||selector.value);let models=0,tables=0;for(const card of articles){card.hidden=!visible.has(card.id);card.classList.toggle('approximate',result.approximate&&!card.hidden);if(!card.hidden){if(card.dataset.kind==='model')models++;else tables++;}}for(const section of sections){section.hidden=![...section.querySelectorAll('.searchable')].some(card=>!card.hidden);for(const part of section.querySelectorAll('.counting-part'))part.hidden=![...part.querySelectorAll('.searchable')].some(card=>!card.hidden);const overview=section.querySelector('.counting-overview');if(overview)overview.hidden=!!search.value.trim();const conventions=section.querySelector('.table-conventions');if(conventions)conventions.hidden=![...section.querySelectorAll('[data-kind="table"]')].some(card=>!card.hidden);}document.getElementById('result-count').textContent=models+' 条模式 / 共 ${data.cards.length} 条 · '+tables+' 个计算表或展开 / 共 3 个';document.getElementById('approximation').hidden=!result.approximate;document.getElementById('empty').hidden=!!(models+tables);}
function reset(){search.value='';selector.value='';filter();}
search.addEventListener('input',filter);selector.addEventListener('change',filter);document.getElementById('reset').addEventListener('click',()=>{reset();search.focus();});
document.addEventListener('click',event=>{const keyword=event.target.closest('[data-keyword]');if(keyword){search.value=keyword.dataset.keyword;selector.value='';filter();search.focus();document.querySelector('.tools').scrollIntoView({block:'start',behavior:'smooth'});}const link=event.target.closest('a[href^="#"]');if(link&&document.getElementById(decodeURIComponent(link.hash.slice(1))))reset();});
function revealHash(){const target=document.getElementById(decodeURIComponent(location.hash.slice(1)));if(target&&(target.closest('.model-section')||target.classList.contains('model-section'))){reset();requestAnimationFrame(()=>target.scrollIntoView({block:'start'}));}}
window.addEventListener('hashchange',revealHash);const query=new URLSearchParams(location.search).get('q');if(query)search.value=query;filter();if(location.hash)revealHash();`;
  const html='<!doctype html>\n<html lang="zh-CN"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><meta name="description" content="从模糊题面定位精确算法模型，附数据结构、计数、多项式与复杂度边界。"><title>'+esc(data.title)+'</title><link rel="stylesheet" href="assets/katex/katex.min.css"><style>'+css+'</style></head><body><main id="top"><header><h1>'+esc(data.title)+'</h1><nav aria-label="本册导航"><a href="'+esc(data.print_artifact?.href||'dictionary.pdf')+'">'+esc(data.print_artifact?.label||'打印版 PDF')+'</a><a href="dictionary.md">可编辑正文</a><a href="research/derivations.md">公式推导与边界</a><a href="research/sources.md">来源记录</a></nav><p class="intro">'+data.cards.length+' 条模式 · M D N C Q I · '+esc(data.date)+' 更新</p><p class="guide"><strong>先按题面找，再核对条件。</strong>检索词只是入口；正文中加粗的对象、限制和例外，决定方法是否适用。多个关键词用空格分开，也可点击条目前的检索词。</p></header><div class="tools"><div class="search-row"><input id="search" type="search" aria-label="搜索问题形态与关键词" placeholder="例如：树上路径 颜色、双子树、无标号、逆序对"><select id="section-filter" aria-label="选择板块"><option value="">全部板块</option>'+sections.map(section=>'<option value="'+section.code+'">'+section.code+' · '+esc(section.title)+'</option>').join('')+'</select><button type="button" class="reset" id="reset">清除筛选</button></div><p id="result-count" class="results-line" aria-live="polite"></p></div>'+shapeHTML+tocHTML+'<p class="approximation" id="approximation" hidden><strong>近似匹配，请核对模型条件。</strong>没有找到直接命中项，以下是题面用词接近的条目。</p><div class="empty" id="empty" hidden>没有匹配的条目。可换成更短的词、减少关键词，或选择“全部板块”。</div>'+content+'<footer><a class="top-link" href="#top">回到目录 ↑</a>内部编号用于本册定位；外部题号与链接收在来源中。正文、公式和计算表可离线阅读。</footer></main><script>'+browserScript+'</script></body></html>\n';
  for(const match of html.matchAll(/href="#([^"]+)"/g))if(match[1]!=='top'&&!anchors.has(match[1]))throw new Error('Broken directory anchor: '+match[1]);
  if(records.filter(record=>record.kind==='model').length!==data.cards.length)throw new Error('Card render mismatch');
  if(records.filter(record=>record.kind==='table').length!==3)throw new Error('Expected three expansions');
  if(records.some(record=>record.search.includes('https://')))throw new Error('Source URLs must not leak into body search');
  const tests=runSearchTests(records);
  fs.writeFileSync(path.join(ROOT,'index.html'),html);
  return {cards:data.cards.length,tables:3,sections:counts,shapeGroups:shapeIndex.length,keywords:data.cards.reduce((sum,card)=>sum+card.keywords.length,0),highlightOccurrences:highlightCount,formulas:formulaCount,anchors:anchors.size,searchChecks:tests,bytes:Buffer.byteLength(html)};
}
module.exports={normalize,bigrams,selectMatches,runSearchTests,build};
if(require.main===module){try{if(process.argv.includes('--test-search'))console.log(JSON.stringify({searchChecks:runSearchTests()}));else console.log(JSON.stringify(build(),null,2));}catch(error){console.error(error.stack||error.message);process.exitCode=1;}}
