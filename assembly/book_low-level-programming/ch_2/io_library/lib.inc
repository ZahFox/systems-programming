section .text

string_length_2:
    xor rax, rax
    sub ecx, ecx
    sub al, al  ; set the string delimiter to NULL/0
    not ecx
    cld ; clears the DF flag in the EFLAGS register. When the DF
        ; flag is set to 0, string operations
        ; increment the index registers (ESI and/or EDI). 
    repne scasb ; keep incrementing the index until the NULL byte is encountered
    ; fast way to compute and save the length
    not ecx
    dec ecx
    mov eax, ecx
    ret


string_length:
    xor rax, rax
  .loop:
    cmp byte [rdi+rax], 0
    je .end 
    inc rax
    jmp .loop 
  .end:
    ret


print_string:
    push rdi
    call string_length ; string length stored in rax
    pop rsi  ; store the string to print in rsi
    mov rdx, rax ; sting length used for # of bytes argument
    mov rdi, 1 ; stdout file descriptor
    mov rax, 1 ; 'write' syscall identifier
    syscall
    ret

print_char:
    mov rsi, rdi
    mov rdx, 1
    mov rdi, 1
    mov rax, 1
    syscall
    ret


print_newline:
    mov rdi, 0xA
    call print_char
    ret

; rdi - source, rsi - dest, rdx - dest length
string_copy:
    push rdi
    push rsi
    push rdx
    call string_length
    pop rdx
    pop rsi
    pop rdi
    cmp rax, rdx
    jae .too_long
    push rsi
      .loop:
        mov dl, byte[rdi]
        mov byte[rsi], dl
        inc rdi
        inc rsi
        test dl, dl
        jnz .loop
    pop rax
    ret
  .too_long:
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


