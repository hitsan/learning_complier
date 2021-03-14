.intel_syntax noprefix
.global main
main:
  push rbp
  mov rbp, rsp
  sub rsp, 208
  mov rax, rbp
  sub rax, 8
  mov rax
  push 2
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 8
  mov rax
  pop rax
  mov rax, [rax]
  push rax
  push 2
  pop rdi
  pop rax
  push rax
  pop rax
  mov rsp, rbp
  pop rbp
  ret
