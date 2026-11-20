import re, sys, os
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import ParagraphStyle
from reportlab.lib import colors
from reportlab.lib.units import cm
from reportlab.platypus import *
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
D='/usr/share/fonts/truetype/dejavu/'
for n,f in [('S','DejaVuSans.ttf'),('SB','DejaVuSans-Bold.ttf'),('SI','DejaVuSans.ttf'),('SBI','DejaVuSans-Bold.ttf'),('M','DejaVuSansMono.ttf')]:
    pdfmetrics.registerFont(TTFont(n,D+f))
pdfmetrics.registerFontFamily('S',normal='S',bold='SB',italic='SI',boldItalic='SBI')
src,out=sys.argv[1],sys.argv[2]; base=os.path.dirname(src)
B=ParagraphStyle('b',fontName='S',fontSize=9.5,leading=13.5,spaceAfter=5)
H={1:ParagraphStyle('h1',parent=B,fontName='SB',fontSize=18,leading=22,spaceAfter=10),
   2:ParagraphStyle('h2',parent=B,fontName='SB',fontSize=14,leading=18,spaceBefore=12,spaceAfter=6),
   3:ParagraphStyle('h3',parent=B,fontName='SB',fontSize=11,leading=15,spaceBefore=8,spaceAfter=4)}
L=ParagraphStyle('l',parent=B,leftIndent=14,bulletIndent=4,spaceAfter=2)
C=ParagraphStyle('c',fontName='M',fontSize=8,leading=10.5,backColor=colors.HexColor('#f2f2f2'),borderPadding=5,spaceAfter=8,spaceBefore=3)
T=ParagraphStyle('t',parent=B,fontSize=8,leading=10,spaceAfter=0)
def inl(s):
    s=s.replace('&','&amp;').replace('<','&lt;').replace('>','&gt;')
    s=re.sub(r'`([^`]+)`',r'<font name="M" size="8.5">\1</font>',s)
    s=re.sub(r'\*\*(.+?)\*\*',r'<b>\1</b>',s)
    s=re.sub(r'(?<![\w*])\*([^*\n]+?)\*(?![\w*])',r'<i>\1</i>',s)
    s=re.sub(r'\[(PREENCHER[^\]]*)\]',r'<font backColor="#fff3a0">[\1]</font>',s)
    return s
txt=re.sub(r'<!--.*?-->','',open(src,encoding='utf8').read(),flags=re.S)
lines=txt.split('\n'); st=[]; i=0
while i<len(lines):
    l=lines[i]
    if l.startswith('```'):
        j=i+1; buf=[]
        while not lines[j].startswith('```'): buf.append(lines[j]); j+=1
        code='\n'.join(buf).replace('&','&amp;').replace('<','&lt;').replace('>','&gt;').replace(' ','&nbsp;').replace('\n','<br/>')
        st.append(Paragraph(code,C)); i=j+1; continue
    if l.startswith('|'):
        rows=[]
        while i<len(lines) and lines[i].startswith('|'):
            if not re.match(r'^\|[\s\-|]+\|$',lines[i]):
                rows.append([Paragraph(inl(c.strip().replace('\x00','|')),T) for c in lines[i].strip().strip('|').replace('\\|','\x00').split('|')])
            i+=1
        t=Table(rows,repeatRows=1)
        t.setStyle(TableStyle([('GRID',(0,0),(-1,-1),0.4,colors.grey),('BACKGROUND',(0,0),(-1,0),colors.HexColor('#e6e6e6')),('VALIGN',(0,0),(-1,-1),'TOP')]))
        st+= [t,Spacer(1,8)]; continue
    m=re.match(r'^(#{1,3}) (.*)',l)
    if m: st.append(Paragraph(inl(m.group(2)),H[len(m.group(1))]))
    elif l.strip()=='---': st.append(HRFlowable(width='100%',color=colors.grey,spaceBefore=4,spaceAfter=6))
    elif re.match(r'^\s*[-*] ',l): st.append(Paragraph(inl(re.sub(r'^\s*[-*] ','',l)),L,bulletText='•'))
    elif l.strip():
        ims=re.findall(r'!\[([^\]]*)\]\(([^)]+)\)',l)
        if ims:
            for alt,p in ims:
                fp=os.path.join(base,p)
                if os.path.exists(fp):
                    from reportlab.lib.utils import ImageReader
                    w,h=ImageReader(fp).getSize(); W=12*cm
                    st+=[Image(fp,W,W*h/w),Paragraph(f'<i>{inl(alt)}</i>',B)]
                else: st.append(Paragraph(f'[IMAGEM ausente: {inl(p)}]',B))
        else: st.append(Paragraph(inl(l),B))
    i+=1
def foot(c,d):
    c.setFont('S',8); c.drawCentredString(A4[0]/2,1*cm,str(d.page))
SimpleDocTemplate(out,pagesize=A4,leftMargin=2*cm,rightMargin=2*cm,topMargin=2*cm,bottomMargin=2*cm,title='Relatório — Newton para o SNL de Broyden').build(st,onFirstPage=foot,onLaterPages=foot)
