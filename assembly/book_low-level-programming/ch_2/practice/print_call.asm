section .data

newline_char: db 10
codes: db '0123456789abcdef'

section .text
global _start

print_newline:
	mov rax, 1						; 'write' syscall identifier
	mov rdi, 1						; stdout file descriptor
	mov rsi, newline_char ; where the newline char is located
	mov rdx, 1					  ; the amount of bytes to write
	syscall
	ret

print_hex:
	mov rax, rdi
	mov rdi, 1
	mov rdx, 1
	mov rcx, 64              ; how far rax will shift

	iterate:
		push rax               ; save the initial rax value
		sub rcx, 4      
		sar rax, cl            ; shift to 60, 56, 52, ... 4, 0
		                       ; the cl register is the smallest
		and rax, 0xf           ; clear all bits but the lowest four
		lea rsi, [codes + rax] ; take a hex digit character code
    mov rax, 1
    push rcx               ; syscall will overwrite rcx
    syscall
    pop rcx
    pop rax
    test rcx, rcx          ; rcx = 0 when all digits are shown
    jnz iterate
  ret

_start:
	mov rdi, 0x1122334455667788
	call print_hex
	call print_newline
	mov rax, 60
	xor rdi, rdi
	syscall
