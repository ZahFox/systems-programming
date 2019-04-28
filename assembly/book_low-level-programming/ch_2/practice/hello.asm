global _start

section .data
message: db 'hello, world!', 10

section .text
_start:
	mov	rax, 1 ; system call number should be stored in rax
	mov	rdi, 1 ;vargunment #1 in rdi: where to write (descriptor)?
	mov	rsi, message ; where does the string start?
	mov	rdx, 14 ; how many bytes to  write?
	syscall ; invoked the system call

	mov rax, 60 ; 'exit' syscall
	xor rdi, rdi
	syscall
