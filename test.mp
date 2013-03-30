beginfig(1);

pair pi[];
pair po[];

for i = 0 upto 4:
  pi[i] = 3*cm*dir(i*(360/5));
  po[i] = 4*cm*dir(i*(360/5));
  %draw pi[i] withpen pencircle scaled 4;
  %draw po[i] withpen pencircle scaled 4;
  draw pi[i]--po[i] withcolor green;
endfor;
for i = 0 upto 3:
  draw po[i]--po[i+1] withcolor blue;
endfor;
draw po[4]--po[0] withcolor blue;

draw pi[0]--pi[2]--pi[4]--pi[1]--pi[3]--cycle withcolor red;

endfig;

beginfig(2);

def draw_complete_graph(expr n, u, v) =
  pair p[];

  for i=0 upto n-1:
    p[i] = u*cm*dir(i*(360/n)) + (0,-v*cm);
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
draw_complete_graph(10, 4, 8);
endfig;

end;
