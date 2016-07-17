lex p1.l
yacc -d p1yacc1.y
cc lex.yy.c y.tab.c -w
./a.out

