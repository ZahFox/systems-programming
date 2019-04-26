# Compiling an .asm Source File

```bash
nasm -felf64 <FILE_PATH> -o <OUTPUT_PATH_1>
ld -o <OUTPUT_PATH_2> <OUTPUT_PATH_1>
```

# Functions

The first six arguments passed to a function are stored in `rdi, rsi, rdx, rcx, r8 and r9`. The rest are passed on to the stack in reverse order.

These registers must be restored by the procedure being called after execution: `rbx, rbp, rsp, r12-r15`.
	> You should never use `rbp` and `rsp`. They are implicitly used during the execution. `rsp` is used as a stack pointer.

All other registers should be saved by the caller before invoking a different procedure.

Implementation-wise, we are returning values by sotring them in `rax` before the function ends its execution. If you need to return two values, you are allowed to use `rdx` for the second one.

The pattern of calling a function is as follows:

- Save all caller-saved registers you want to survive function call (you can use push for that).
- Store arguments in the relevant registers (`rdi`, `rsi`, etc.).
- Invoke function using `call`.
- After functions returns, `rax` will hold the return value.
- Restore caller-sasved registers stored before the function call.
