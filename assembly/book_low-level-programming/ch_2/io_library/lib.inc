section .text

string_length:
    xor rax, rax
  .loop:
    cmp byte [rdi+rax], 0
    je .end 
    inc rax
    jmp .loop 
  .end:
    ret

string_length_2:
    xor rax, rax
    sub ecx, ecx
    sub al, al  ; set the string delimiter to NULL/0
    not ecx
    cld ; Clears the DF flag in the EFLAGS register. When the DF
        ; flag is set to 0, string operations
        ; increment the index registers (ESI and/or EDI). 
    repne scasb ; keep incrementing the index until the NULL byte is encountered
    ; fast way to compute and save the length
    not ecx
    dec ecx
    mov eax, ecx
    ret

print_string:
    xor rax, rax
    ret


print_char:
    xor rax, rax
    ret

print_newline:
    xor rax, rax
    ret


print_uint:
    xor rax, rax
    ret


print_int:
    xor rax, rax
    ret

string_equals:
    xor rax, rax
    ret


read_char:
    xor rax, rax
    ret 

read_word:
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    xor rax, rax
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
    ret 


string_copy:
    ret