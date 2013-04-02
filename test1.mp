outputtemplate := "%j%c.eps";
input lib;
beginfig(1);

pair pi[];
pair po[];

for i = 0 upto 4:
  pi[i] = 2*cm*dir(i*(360/5));
  po[i] = 4*cm*dir(i*(360/5));
  draw pi[i]--po[i] withcolor red;
endfor;
for i = 0 upto 3:
  draw po[i]--po[i+1] withcolor blue;
endfor;
draw po[4]--po[0] withcolor blue;

draw pi[0]--pi[2]--pi[4]--pi[1]--pi[3]--cycle withcolor 0.6green;

for i = 0 upto 4:
  draw vertex(pi[i]);
  draw vertex(po[i]);
endfor;

label.bot(btex Zeman's graph etex scaled 2, (0,-4cm));

endfig;

beginfig(2);

def draw_complete_graph(expr n, u, v) =
  pair p[];

  for i=0 upto n-1:
    p[i] = u*cm*dir(i*(360/n)) + (0,(-v+1)*cm);
    draw p[i] withpen pencircle scaled 8 withcolor black;
  endfor;
  for i=0 upto n-1:
    for j=0 upto n-1:
      if i <> j:
        draw p[i]--p[j];
      fi;
    endfor;
  endfor;
enddef;

draw_complete_graph(10, 4, 0);
draw_complete_graph(10, 4, 7);
endfig;

beginfig(3);
draw (0,0)--(10cm, 0)--(10cm,10cm)--cycle withpen pencircle scaled 10;
endfig;

end;
