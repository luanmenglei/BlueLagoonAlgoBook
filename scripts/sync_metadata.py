"""Export annotations for browsing; never overwrite authoritative header comments."""
from pathlib import Path
import json
from metadata import load_chapters
ROOT=Path(__file__).resolve().parents[1]

def sync():
    dest=ROOT/'output';dest.mkdir(exist_ok=True)
    (dest/'catalog.json').write_text(json.dumps(load_chapters(),ensure_ascii=False,indent=2)+'\n',encoding='utf-8')

if __name__=='__main__':sync();print('Header annotations exported to output/catalog.json')
