void *make_const_bool(int val);
void *make_eq_bool(void *expr1, void *expr2);
void *make_ne_bool(void *expr1, void *expr2);
void *make_less_bool(void *expr1, void *expr2);
void *make_leq_bool(void *expr1, void *expr2);
void *make_not_bool(void *b);
void *make_and_bool(void *b1, void *b2);
void *make_or_bool(void *b1, void *b2);
void *make_paren_bool(void *epxr);

void *make_read_expr();
void *make_add_expr(void *expr1, void *expr2);
void *make_sub_expr(void *expr1, void *expr2);
void *make_mul_expr(void *expr1, void *expr2);
void *make_paren_expr(void *expr);
void *make_const_expr(int val);
void *make_id_expr(char *id);

void *make_assignment_statement(char *id, void *expr);
void *make_if_statement(void *condition, void *then_expr, void *else_expr);
void *make_seq_statement(void *seq_statement);
void *make_assert(void *condition);

void *make_recursive_seq_staement(void *first, void *rem);

void make_execution_tree(void* statements);