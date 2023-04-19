%option noyywrap

%{
    #include "parser.tab.h"
%}

%%

";" { return SEMICOLON; }
"." { return READ; }
"=" { return ASSIGNMENT; }

"if" { return IF; }
"else" { return ELSE; }
"assert" { return ASSERT; }

"(" { return L_PARENTHESIS; }
")" { return R_PARENTHESIS; }
"{" { return L_BRACKET; }
"}" { return R_BRACKET; }

"true" { return CONST_TRUE; }
"false" { return CONST_FALSE; }

"+" { return OP_PLUS; }
"-" { return OP_MINUS; }
"*" { return OP_MUL; }
"==" { return OP_EQ; }
"!=" { return OP_NE; }
"<" { return OP_LESS; }
"<=" { return OP_LEQ; }
"not" { return OP_NOT; }
"and" { return OP_AND; }
"or" { return OP_OR; }

[0-9]+ { yylval.val = atoi(yytext); return INT; }
[a-z][A-Za-z0-9]* { yylval.id = strdup(yytext); return ID; }
[ \t\n] { }

%%
