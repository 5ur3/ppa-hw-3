%define parse.error detailed

%{
    #include <stdio.h>
    #include "execution-tree-builder.h"

    int yylex();
    void yyerror(const char* msg) {
        printf("ERR: %s\n", msg);
    }
%}

%union {
    unsigned int val;
    char *id;
    void* node;
}

%token
SEMICOLON
READ
SKIP
ASSIGNMENT

IF
ELSE
ASSERT

L_PARENTHESIS
R_PARENTHESIS
L_BRACKET
R_BRACKET

CONST_TRUE
CONST_FALSE

OP_EQ
OP_NE
OP_LESS
OP_LEQ
%left OP_PLUS
%left OP_MINUS
%left OP_MUL
%left OP_NOT
%left OP_AND
%left OP_OR

%token<val> INT
%token<id> ID

%type<val> int
%type<id> id

%type<node>
seqStatement
statement
expression
bool

%%

input:
    seqStatement { make_execution_tree($1); }

seqStatement:
    statement SEMICOLON seqStatement { $$ = make_recursive_seq_staement($1, $3); }
|   { $$ = make_recursive_seq_staement(NULL, NULL); }

statement:
    id ASSIGNMENT expression { $$ = make_assignment_statement($1, $3); }
|   IF L_PARENTHESIS bool R_PARENTHESIS statement ELSE statement { $$ = make_if_statement($3, $5, $7); }
|   L_BRACKET seqStatement R_BRACKET { $$ = make_seq_statement($2); }
|   ASSERT L_PARENTHESIS bool R_PARENTHESIS { $$ = make_assert($3); }

expression:
    int { $$ = make_const_expr($1); }
|   id { $$ = make_id_expr($1); }
|   READ { $$ = make_read_expr(); }
|   expression OP_PLUS expression { $$ = make_add_expr($1, $3); }
|   expression OP_MINUS expression { $$ = make_sub_expr($1, $3); }
|   expression OP_MUL expression { $$ = make_mul_expr($1, $3); }
|   L_PARENTHESIS expression R_PARENTHESIS { $$ = make_paren_expr($2); }

bool:
    CONST_TRUE { $$ = make_const_bool(1); }
|   CONST_FALSE { $$ = make_const_bool(0); }
|   expression OP_EQ expression { $$ = make_eq_bool($1, $3); }
|   expression OP_NE expression { $$ = make_ne_bool($1, $3); }
|   expression OP_LESS expression { $$ = make_less_bool($1, $3); }
|   expression OP_LEQ expression { $$ = make_leq_bool($1, $3); }
|   OP_NOT bool { $$ = make_not_bool($2); }
|   bool OP_AND bool { $$ = make_and_bool($1, $3); }
|   bool OP_OR bool { $$ = make_or_bool($1, $3); }
|   L_PARENTHESIS bool R_PARENTHESIS { $$ = make_paren_bool($2); }

id:
    ID { $$ = $1; }

int:
    INT { $$ = $1; }
|   OP_MINUS INT { $$ = -$2; }

%%


int main() {
    yyparse();

    return 0;
}
