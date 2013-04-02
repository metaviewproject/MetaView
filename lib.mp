prologues:=2;

% implementation of figure naming and figure transparency
string name,tag; name := ""; tag := "";
def updatefigname =
	if tag="": filenametemplate (name & "%c.eps");
	else: filenametemplate (name & "%c_" & tag & ".eps");
	fi;
enddef;
	
def figname(expr n) = name := n; updatefigname; enddef;
def figtag(expr t) = tag := t; updatefigname; enddef;

picture transparent_picture;
color transparent_color; transparent_color := 0.9white;
def drawtransparent(expr num) =
	transparent_picture := currentpicture;
endfig;

if tag="": filenametemplate (name & "%c_transparent.eps");
else: filenametemplate (name & "%c_" & tag & "_transparent.eps");
fi;
beginfig(num);
	draw transparent_picture withcolor transparent_color;
endfig;

updatefigname;
enddef;

def from(expr p,d,len) =
	p+dir(d)*len
enddef;

def dirs(expr p,d,len) =
	p--from(p,d,len)
enddef;

def drawvertices(expr s,n) =
	for i:=s upto n:
		draw vertex(PQ[i]);
	endfor
enddef;

def drawfvertices(expr s,n,flags) =
	for i:=s upto n:
		draw vertex(PQ[i]) flags;
	endfor
enddef;

def vertex(expr p) = p withpen pencircle scaled 4pt enddef;
def drawemptyvertex(expr p) = unfill fullcircle scaled 4pt shifted p; draw fullcircle scaled 3.7pt shifted p; enddef;
def drawendpointvertex(expr p) = draw vertex(p) withcolor red; draw fullcircle scaled 6pt shifted p; enddef;
def createpath(expr p) = shakepath(p, 0.015cm,0.1cm) enddef;
vardef shakepath(expr p,d,l) =
	save r,b;
	path r; r := point(arctime 0 of p) of p;
	b := -1;
	for i:=l step l until arclength(p):
		r := r--(point(arctime i of p) of p)+dir(angle(direction(arctime i of p) of p rotated 90))*d*b;
		b := -b;
	endfor
	r--point(arctime arclength(p) of p) of p
enddef;
	
pen normalpen; normalpen := pencircle scaled 0.6pt;
pen boldpen; boldpen := pencircle scaled 1.5pt;
pen bolderpen; bolderpen := pencircle scaled 2pt;
def dotline = withdots scaled 0.82 withpen boldpen enddef;

vardef unclosedbubblec(expr p,c) =
	bubblec((p..reverse p..cycle),c)
enddef;	

vardef bubblec(expr p,c) =
	save r;
	path r; r := (point(arctime 0 of p) of p)+dir(angle(direction(arctime 0 of p) of p rotated 90))*c;
	for i:=0.01cm step 0.025cm until arclength(p):
		r := r..(point(arctime i of p) of p)+dir(angle(direction(arctime i of p) of p rotated 90))*c;
	endfor
	r..(point(arctime arclength(p) of p) of p)+dir(angle(direction(arctime arclength(p) of p) of p rotated 90))*c..cycle
enddef;

vardef bubble(expr p) = bubblec(p,0.12cm) enddef;
vardef unclosedbubble(expr p) = unclosedbubblec(p,0.12cm) enddef;
