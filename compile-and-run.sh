cd src/parser;
bison -d -t -Wcounterexamples parser.y;
mv lex.yy.c lex.yy.cpp;
mv parser.tab.c parser.tab.cpp;
flex lexer.l ; g++ execution-tree-builder.cpp lex.yy.cpp parser.tab.cpp;
cd ../../;
src/parser/a.out < $1 | python3 src/analyser/analyser.py;
