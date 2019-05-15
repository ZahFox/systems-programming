# GDB Notes

Type `quit` to exit `gdb`.

To debug a compiled executable, follow these steps:

1. Make sure `gdb` is installed
2. Run `gdb` with the executable as an argument: `gdb <EXE>`
3. (alt) inside of gdb:  `(gdb) file <EXE>`
4. Run `set disassembly-flavor intel`.
5. Run `break <LABEL>` e.g., `break _start`
6. Run `start`
7. Run `layout asm` and `layout regs`
8. Use `stepi` to move to new instructions.

To print a register's value in binary: `p /t $<REG>` e.g., `p /t $rax`