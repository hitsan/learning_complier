#include <stdio.h>
#include <stdlib.h>
#include "9cc.h"

Token *token;
char *user_input;
Node *code[100];
LVar *locals;

int main(int argc, char **argv){
    if (argc != 2) {
        error("Illegal number of argument!\n");
        return 1;
    }
    user_input = argv[1];
    token = tokenize();
    locals = calloc(1,sizeof(LVar));
    program();

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, 208\n");

    for (int i = 0; code[i]; i++) {
        gen(code[i]);
        printf("  pop rax\n");
    }

    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return 0;
}