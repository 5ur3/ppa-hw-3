#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

class Expr {
public:
  string type;
  vector<Expr *> children;
  void *content;

  Expr() { this->children = vector<Expr *>(); }
  Expr(string type) {
    this->type = type;
    this->children = vector<Expr *>();
  }
};

int ifs_count = 0;

void *make_const_bool(int val) {
  Expr *expr = new Expr("CONST_BOOL");
  expr->content = new bool(val);
  return expr;
}
void *make_eq_bool(void *expr1, void *expr2) {
  Expr *expr = new Expr("EQ");
  expr->children.push_back((Expr *)expr1);
  expr->children.push_back((Expr *)expr2);
  return expr;
}
void *make_ne_bool(void *expr1, void *expr2) {
  Expr *expr = new Expr("NE");
  expr->children.push_back((Expr *)expr1);
  expr->children.push_back((Expr *)expr2);
  return expr;
}
void *make_less_bool(void *expr1, void *expr2) {
  Expr *expr = new Expr("LESS");
  expr->children.push_back((Expr *)expr1);
  expr->children.push_back((Expr *)expr2);
  return expr;
}
void *make_leq_bool(void *expr1, void *expr2) {
  Expr *expr = new Expr("LEQ");
  expr->children.push_back((Expr *)expr1);
  expr->children.push_back((Expr *)expr2);
  return expr;
}
void *make_not_bool(void *b) {
  Expr *expr = new Expr("NOT");
  expr->children.push_back((Expr *)b);
  return expr;
}
void *make_and_bool(void *b1, void *b2) {
  Expr *expr = new Expr("AND");
  expr->children.push_back((Expr *)b1);
  expr->children.push_back((Expr *)b2);
  return expr;
}
void *make_or_bool(void *b1, void *b2) {
  Expr *expr = new Expr("OR");
  expr->children.push_back((Expr *)b1);
  expr->children.push_back((Expr *)b2);
  return expr;
}
void *make_paren_bool(void *b) { return b; }
void *make_read_expr() { return new Expr("READ"); }
void *make_add_expr(void *expr1, void *expr2) {
  Expr *expr = new Expr("PLUS");
  expr->children.push_back((Expr *)expr1);
  expr->children.push_back((Expr *)expr2);
  return expr;
}
void *make_sub_expr(void *expr1, void *expr2) {
  Expr *expr = new Expr("MINUS");
  expr->children.push_back((Expr *)expr1);
  expr->children.push_back((Expr *)expr2);
  return expr;
}
void *make_mul_expr(void *expr1, void *expr2) {
  Expr *expr = new Expr("MUL");
  expr->children.push_back((Expr *)expr1);
  expr->children.push_back((Expr *)expr2);
  return expr;
}
void *make_paren_expr(void *expr) {
  return expr;
}
void *make_const_expr(int val) {
  Expr *expr = new Expr("CONST_INT");
  expr->content = new int(val);
  return expr;
}
void *make_id_expr(char *id) {
  Expr *res = new Expr("ID");
  res->content = new string(id);
  return res;
}
void *make_assignment_statement(char *id, void *expr) {
  Expr *ass = new Expr("ASS");
  ass->children.push_back((Expr *)expr);
  ass->content = new string(id);
  return ass;
}
void *make_if_statement(void *condition, void *then_expr, void *else_expr) {
  Expr *ifs = new Expr("CONDITION");
  ifs_count++;
  ifs->children.push_back((Expr *)condition);
  ifs->children.push_back((Expr *)then_expr);
  ifs->children.push_back((Expr *)else_expr);
  return ifs;
}
void *make_seq_statement(void *seq_statement) {
  if (seq_statement == NULL) {
    return new Expr("SEQ_STATEMENT");
  }
  return seq_statement;
}
void *make_assert(void *condition) {
  Expr *expr = new Expr("ASSERT");
  expr->children.push_back((Expr *)condition);
  return expr;
}
void *make_recursive_seq_staement(void *first, void *rem) {
  if (first == NULL) {
    return NULL;
  }
  Expr *expr = new Expr("SEQ_STATEMENT");
  expr->children.push_back((Expr *)first);
  if (rem == NULL) {
    return expr;
  }

  Expr *rem_expr = (Expr *)rem;
  for (int i = 0; i < rem_expr->children.size(); i++) {
    expr->children.push_back(rem_expr->children[i]);
  }

  return expr;
}

int read_index = 0;
void print_statement(Expr *statement) {
  if (statement->type == "ID") {
    cout << *((string *)statement->content);
  } else if (statement->type == "CONST_BOOL") {
    if (*((bool *)statement->content)) {
      cout << "TRUE";
    } else {
      cout << "FALSE";
    }
  } else if (statement->type == "CONST_INT") {
    cout << *((int *)statement->content);
  } else if (statement->type == "READ") {
    cout << "I_" << read_index++;
  } else {
    cout << statement->type;
  }

  if (statement->type == "ASS") {
    cout << " " << *((string *)statement->content);
  }

  cout << " ";

  for (int i = 0; i < statement->children.size(); i++) {
    print_statement(statement->children[i]);
  }
}

void print_cond(Expr *cond) {
  cout << "COND ";
  print_statement(cond);
  cout << endl;
}

void print_execution_path(vector<Expr *> statements,
                          vector<int> execution_path) {
  if (statements.size() == 0) {
    return;
  }

  vector<Expr *> rest;
  if (statements[0]->type == "SEQ_STATEMENT") {
    for (int i = 0; i < statements[0]->children.size(); i++) {
      rest.push_back(statements[0]->children[i]);
    }
  } else if (statements[0]->type == "CONDITION") {
    Expr *cond = statements[0]->children[0];
    if (execution_path[execution_path.size() - 1]) {
      print_cond(cond);
      rest.push_back(statements[0]->children[1]);
    } else {
      Expr *not_cond = new Expr("NOT");
      not_cond->children.push_back(cond);
      print_cond(not_cond);
      rest.push_back(statements[0]->children[2]);
    }
    execution_path.pop_back();
  } else {
    print_statement(statements[0]);
    cout << endl;
  }

  for (int i = 1; i < statements.size(); i++) {
    rest.push_back(statements[i]);
  }

  return print_execution_path(rest, execution_path);
}

void make_execution_tree(void *statements) {
  for (int i = 0; i < (1 << ifs_count); i++) {
    int seed = i;
    vector<int> execution_path;
    while (seed) {
      execution_path.push_back(seed % 2);
      seed >>= 1;
    }

    while (execution_path.size() < ifs_count) {
      execution_path.push_back(0);
    }

    cout << "***" << endl;
    read_index = 0;
    print_execution_path(((Expr *)statements)->children, execution_path);
  }
}
