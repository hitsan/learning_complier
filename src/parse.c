#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "9cc.h"

Node *new_node(NodeKind kind, Node *lhs, Node *rhs){
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
} 

Node *new_node_num(int val){
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
} 

void error_at(char *loc, char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

void error(char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

bool consume(char *op){
    if((token->kind != TK_RESERVED &&
        token->kind != TK_RETURN   &&
        token->kind != TK_IF       &&
        token->kind != TK_WHILE    &&
        token->kind != TK_FOR)     ||
        token->len  != strlen(op)  || 
        memcmp(token->str, op, token->len)){
        return false;
    }
    token = token->next;
    return true;
}

void expect(char *op){
    if(token->kind != TK_RESERVED || 
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len)){
        error_at(token->str, "expected '%s'.", op);
    }
    token = token->next;
}

int expect_number(){
    if(token->kind != TK_NUM){
        error_at(token->str, "expected a number.");
    }
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof(){
    return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *cur, char *str, int len){
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

Token *consume_ident(){
    if(token->kind != TK_IDENT){
        return NULL;
    }
    Token* tok = token;
    token = token->next;
    return tok;
}

bool startswith(char *p, char *q){
    return memcmp(p, q, strlen(q)) == 0;
}

Node *primary(){
    if(consume("(")){
        Node *node = expr();
        expect(")");
        return node;
    }

    Token *tok = consume_ident();
    if(tok){
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;

        LVar *lvar = find_lvar(tok);
        if(lvar){
            node->offset = lvar->offset;
        }else{
            lvar = calloc(1, sizeof(LVar));
            lvar->next = locals;
            lvar->name = tok->str;
            lvar->len = tok->len;
            lvar->offset = locals->offset + 8;
            node->offset = lvar->offset;
            locals = lvar;
        }
        return node;
    }

    return new_node_num(expect_number());
}

Node *unary(){
    if(consume("+")){
        return primary();
    }
    if(consume("-")){
        return new_node(ND_SUB, new_node_num(0), primary());
    }
    return primary();
}

Node *mul(){
    Node *node = unary();

    for(;;){
        if(consume("*")){
            node = new_node(ND_MUL, node, unary());
        }else if(consume("/")){
            node = new_node(ND_DIV, node, unary());
        }else{
            return node;
        }
    }
}

Node *add(){
    Node *node = mul();

    for(;;){
        if(consume("+")){
            node = new_node(ND_ADD, node, mul());
        }else if(consume("-")){
            node = new_node(ND_SUB, node, mul());
        }else{
            return node;
        }
    }
}

Node *relational(){
    Node *node = add();

    for(;;){
        if(consume("<")){
            node = new_node(ND_LT, node, add());
        }else if(consume("<=")){
            node = new_node(ND_LE, node, add());
        }else if(consume(">")){
            node = new_node(ND_LT, add(), node);
        }else if(consume(">=")){
            node = new_node(ND_LE, add(), node);
        }else{
            return node;
        }
    }
}

Node *equality(){
    Node *node = relational();

    for(;;){
        if(consume("==")){
            node = new_node(ND_EQ, node, relational());
        }else if(consume("!=")){
            node = new_node(ND_NE, node, relational());
        }else{
            return node;
        }
    }
}

Node *assign(){
    Node *node = equality();
    if(consume("=")){
        node = new_node(ND_ASSIGN, node, assign());
    }
    return node;
}

Node *expr(){
    return assign();
}

Node *stmt(){
    Node *node;

    // if(consume("if") && consume("(")){
    //     expr();
    //     stmt();

    // }else if(consume("while") && consume("(")){
        
    // }else if(consume("for") && consume("(")){
    if(consume("if") && consume("(")){
        node = new_node(ND_IF, expr(), node);
        expect(")");
        node->rhs = stmt();
        if(consume("else")){
            node = new_node(ND_ELSE, node, stmt());
        }
    }else if(consume("while") && consume("(")){
        node = new_node(ND_IF, expr(), node);
        expect(")");
        node->rhs = stmt();
    }else if(consume("for") && consume("(")){
        //1つ先のトークンを読む
        node = new_node(ND_IF, expr(), node);
        expect(")");
        node->rhs = stmt();
    }else if(consume("return")){
        node = new_node(ND_RETURN, expr(), node);
        expect(";");
    }else{
        node = expr();
        expect(";");
    }
    return node;
}

void program(){
    int i = 0;
    while(!at_eof()){
        code[i++] = stmt();
    }
    code[i] = NULL;
}

LVar *find_lvar(Token *tok){
    for(LVar *var = locals; var; var = var->next){
        if(var->len == tok->len && !memcmp(tok->str, var->name, var->len)){
            return var;
        }
    }
    return NULL;
}

int is_alnum(char c){
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           (c == '_');
}

Token *tokenize(){
    char *p = user_input;
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while(*p){
        if(isspace(*p)){
            p++;
            continue;
        }

        if(startswith(p, "==") || startswith(p, "!=") ||
           startswith(p, "<=") || startswith(p, ">=")){
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        if(strchr("+-*/()<>=;", *p)){
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        if(isdigit(*p)){
            cur = new_token(TK_NUM, cur, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - q;
            continue;
        }

        if(startswith(p, "if") && !is_alnum(p[2])){
            cur = new_token(TK_IF, cur, p, 2);
            p += 2;
            continue;
        }

        if(startswith(p, "else") && !is_alnum(p[4])){
            cur = new_token(TK_IF, cur, p, 4);
            p += 4;
            continue;
        }

        if(startswith(p, "while") && !is_alnum(p[5])){
            cur = new_token(TK_WHILE, cur, p, 5);
            p += 5;
            continue;
        }

        if(startswith(p, "for") && !is_alnum(p[3])){
            cur = new_token(TK_FOR, cur, p, 3);
            p += 3;
            continue;
        }

        if (startswith(p, "return") == 1 && !is_alnum(p[6])) {
            cur = new_token(TK_RETURN, cur, p, 6);
            p += 6;
            continue;
        }

        if(*p >= 'a' && *p <= 'z'){
            char* s = p;
            while(*p >= 'a' && *p <= 'z'){
                p++;
            }
            cur = new_token(TK_IDENT, cur, s, p-s);
            continue;
        }

        error_at(p, "Invalid token!");
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}