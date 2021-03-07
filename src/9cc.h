#include <stdbool.h>

typedef enum{
    TK_RESERVED,
    TK_IDENT,
    TK_NUM,
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
    ND_ASSIGN,
    ND_LVAR,
} NodeKind;

typedef struct Node Node;
typedef struct Token Token;

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

Token *token;
char *user_input;

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
Token *tokenize(char* user_input);
void gen(Node *node);