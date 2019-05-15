# Chapter 2 Notes

## Compiling an .asm Source File

```bash
nasm -felf64 <FILE_PATH> -o <OUTPUT_PATH_1>
ld -o <OUTPUT_PATH_2> <OUTPUT_PATH_1>
```

## Registers

### General Purpose Registers

These are 64-bit registers with names r0, r1, ..., r15.

* r0 (rax) - used as an accumulator (and implicitly during div)
* r3 (rbx) - Used for base addressing
* r1 (rcx) - Used for cycles (e.g., in a loop)
* r2 (rdx) - Stores data during input/output operations
* r4 (rsp) - Stores the address of the topmost element in the hardware stack.
* r5 (rbp) - Stack frame's base.
* r6 (rsi) - Source index in string manipulation (such as movsd)
* r7 (rdi) - Destination index in string manipulation commands
*  r9...r15 - Store temporal variables (r10 saves CPU flags when syscall is executed)

### Accessing Parts of Registers

Addressing a part of a register is possible. You can address its lowest 32 bits, lowest 16 bits, or lowest 8 bits. When using r0, ..., r15 you can do this by suffixing the register's name (d for 32-bit, w for 16-bit, b for 8-bit).

Examples: 
* **r7b** is the lowest byte of register r7.
* **r3w** consists of the lowest two bytes of r3.
* **r0d** consists of the lowest four bytes of r0.
* **rax** - 64-bit rax, 32-bit eax, 16-bit ax, upper 8-bit ah, lower 8-bit al. (same pattern for rax, rbx, rcx, and rdx).
* The smallest parts of rsi and rdi are sil and dil.
* The smallest parts of rsp and rbp are spl and bpl.

### Flags Register

Another accessible register is called `rflags`. It stores flags, which reflect the current program state--for example, what was the result of the last arithmetic instruction: was it negative, did an overflow happen, etc. Its smaller parts are called `eflags` (32 bit) and `flags` (16 bit).

## Assembly Functions

The first six arguments passed to a function are stored in `rdi`, `rsi`, `rdx`, `rcx`, `r8`, and `r9`. The rest are passed on to the stack in reverse order.

These registers must be restored by the procedure being called after execution: `rbx`, `rbp`, `rsp`, `r12-r15`.

    You should never use rbp and rsp. They are implicitly used during the execution. rsp is used as a stack pointer.

All other registers should be saved by the caller before invoking a different procedure.

Implementation-wise, we are returning values by storing them in `rax` before the function ends its execution. If you need to return two values, you are allowed to use `rdx` for the second one.

The pattern of calling a function is as follows:

- Save all caller-saved registers you want to survive function call (you can use push for that).
- Store arguments in the relevant registers (`rdi`, `rsi`, etc.).
- Invoke function using `call`.
- After functions returns, `rax` will hold the return value.
- Restore caller-sasved registers stored before the function call.

## Commen Intel Assembly Instructions

`<INSTRUCTION> <DEST> <SOURCE>`

### `xor` - Logical Exclusive OR

Performs a bitwise OR (XOR) operation on the destination (first) and source (second) operands and stores the result in the destination operand location.

### `jmp`, `ja`, and similar ones

#### `jmp` - Jump

Transfers program control to a different point in the instruction stream without recording return information. The destination (target) operand specifies the address of the instruction being jumped to.

When executing a near jump, the processor jumps to the address (within the current code segment) that is specified with the target operand.

### `cmp` - Compare Two Operands

Compares the first source operand with the second operands and sets the status flags in the EFLAGS register according to the results. The comparison is performed by subtracting the second operand from the first operand and then setting the status flags in the same manner as the SUB instruction.

### `mov` - Move

Copies the second operand (source operand) to the first operand (destination operand). The source operand can be an immediate value, general-purpose register, segment register, or memory location; Both operands must be the same size.

### `inc` - Increment by 1

Adds 1 to the destination operand, while preserving the state of the CF flag. The destination operand can be a register or a memory location.

### `dec` - Decrement by 1

Subtracts 1 from the destination operand, while preserving the state of the CF flag. The destination operand can be a register or a memory location.

### `mul`, `imul` - Signed and Unsigned Multiplication (i is signed)

- **One-operand form** - Same for signed and unsigned. The source operand is a register or memory location and is multiplied by the value in AL, AX, EAX, or RAX register (depending on the operand size) and the product (twice the size of the input operand) is stored in the AX, DX:AX, EDX:EAX, RDX:RAX registers, respectively.

- **Two-operand form** - Source and destination operands are multiplied and stored in the destination operand location. The result is truncated.

- **Three-operand form** - The first source operand (from register or memory) is multiplied by a second source operand (from and immediate value) and stored in the destination operand location. The result is truncated.

### `div`, `idiv` - Signed and Unsigned Division (i is signed)

Divides the value in AX, DX:AX, EDX:EAX by the source operand and stores the result in AX (AH:AL), DX:AX, or EDX:EAX registers.

### `add`, `sub`

### `neg` - Two's Complement Negation

Replaces the value of operand (the destination operand) with its two's complement. (This operation is equivalent to substracting the operand from 0.) The destination operand is located in a general-purpose register or a memory location.

### `call` - Call Procedure

Saves procedure linking information on the stack and branches to the called procedure specified using the target operand. The target operand specifies the address of the first instruction in the called procedure.

When executing a near call (in the same code segment), the processor pushes the value of the EIP register (which contains the offset of the instruction following the CALL instruction) on the stack (for use later a return-instruction pointer).

### `ret` - Return from Procedure

Transfers program control to a return address located on the top of the stack. The address is usually placed on the stack by a CALL instruction, and the return is made to the instruction that follows the CALL instruction.

### `push` - Push Word, Doubleword or Quadword Onto the Stack

Decrements the stack pointer and then stores the source operand on the top of the stack.

The operand size (16, 32, or 64 bits) determines the amount by which the stack pointer is decremented (2, 4, or 8).

### `pop` - Pop a Value from the Stack

Loads the value from the top of the stack on to the location specified with the destination operand (or explicit opcode) and then increments the stack pointer.
