#include <stdbool.h>

typedef enum{
    TK_RESERVED,
    TK_IDENT,
    TK_NUM,
    TK_RETURN,
    TK_IF,
    TK_WHILE,
    TK_FOR,
    TK_EOF,
} TokenKind;

typedef enum{
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_EQ,
    ND_NE,
    ND_LT,
    ND_LE,
    ND_NUM,
    ND_IF,
    ND_ELSE,
    ND_RETURN,
    ND_ASSIGN,
    ND_LVAR,
} NodeKind;

typedef struct Node Node;
typedef struct Token Token;
typedef struct LVar LVar;

struct Token{
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    int len;
};

struct Node{
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
    int offset;
};

struct LVar{
  LVar *next;
  char *name; 
  int len;    
  int offset; 
};

extern Token *token;
extern char *user_input;
extern Node *code[100];
extern LVar *locals;

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
void error_at(char *loc, char *fmt, ...);
void error(char *fmt, ...);
bool consume(char *op);
void expect(char *op);
int expect_number();
bool at_eof();
Node *primary();
Node *mul();
Node *unary();
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *assign();
void program();
LVar *find_lvar(Token *tok);
Token *tokenize();
Token *consume_ident();
int is_alnum(char c);
void gen(Node *node);
void gen_lval(Node *node);