global _start

section .data
test_string: db "abcdef", 0

section .text
strlen:        ; rax will hold the length value
	xor rax, rax ; zeroing rax to avoid random values
	.loop:
		cmp byte [rdi+rax], 0 ; check for the null byte
		je .end
		inc rax
		jmp .loop
	.end:
		ret
		
_start:
	mov rdi, test_string
	call strlen
	mov rdi, rax
	mov rax, 60
	syscall
