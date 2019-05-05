# Assembly Programming Tutorial Notes

## Basic Syntax

An assembly program can be divided into three sections: the **data** section, the **bss** section, and the **text** section.

### The _data_ section

`section.data`

The **data** section is used for declaring initialized data or constants. This data does not change at runtime. You can declare various constant values, file names, or buffer size, etc., in this section.

### The _bss_ section

`section.bss`

The **bss** section is used for declaring variables.

### The _text_ section

The **text** section is used for keeping the actual code. This section must begin with the declaration **global \_start**, which tells the kernel where the program execution begins.

```asm
section.text
  global _start
_start:
```

### Comments

Assembly language comments begin with a semicolon (;). e.g., `add eax, ebx ; comment`

### Syntax of Assembly Language Statements

`[label] mnemonic [operands] [;comment]`

The fields in the square brackets are optional.

## Compiling and Linking an Assembly Program is NASM

Make sure you have set the path of **nasm** and **ld** binaries in your PATH environment variable.

To assemble the program type `nasm -f elf <FILEPATH>`

To link the object file and create an executable file, type `ld -m elf_i386 -s -o <EXECUTABLE_NAME> <OBJECT_FILEPATH>`

## Memory Segments

A segmented memory model divides the system memory into groups of independent segments referenced by pointers located in the segment registers.

### Data Segment

It is represented by the **.data** section and the **.bss** sections. **.data** is where data elements for a program are stored and remains static throughout the program. The **.bss** section is also a static memory section that contains buffers for data to be declared later in the program.

### Code Segment

This is the **.text** section. This defines an area in memory that stores the instruction codes. This is also a fixed area.

### Stack

This segment contains data values passed to functions and procedures within the program.

## Registers

### Processor Registers

In the IA-32 architecture there are ten 32-bit and six 16-bit processor registers grouped into three categories: general registers, control registers, and segment registers. The general registers are also divided into three groups: data registers, pointer registers, and index registers.

## Data Registers

Four 32-bit data registers are used for arithmetic, logical, and other operations. They can be used three ways:

- As complete 32-bit data registers: EAX, EBX, ECX, EDX.
- Lower halves of the 32-bit registers can be used as four 16-bit data registers: AX, BX, CX, and DX.
- As lower and higher 8-bit halves of each 16-bit register: AH, AL, BH, BL, CH, CL, DH, and DL.

- **AX is the primary accumulator**
- **BX is known as the base register** - as it could be used in indexed addressing
- **CX is known as the counter register** - stores the loop count in iterative operations.
- **DX is known as the data register** - Used with AX

## Pointer Registers

The pointer registers are 32-bit EIP, ESP, and EBP along with their corresponding 16-bit right portions, IP, SP, and BP.

- **Instruction Pointer (IP)** - Stores the offset address of the next instruction to be executed. IP with the CS register (CS:IP) gives the complete address of the current instruction in the code segment.
- **Stack Pointer (SP)** - Stores the offset value within the program stack. SP and SS (SS:SP) refers to the current position of data or address within the program stack.
- **Base Pointer (BP)** - Mainly helps in referencing the parameter variables passed to a subroutine.

## Control Registers

The 32-bit instruction pointer register and the 32-bit flags register combined are considered as the control registers.

The common flag bits are:

- **Overflow Flag (OF) bit 11** - Indicates an overflow of the sign bit during an arithmetic operation.
- **Direction Flag (DF) bit 10** - Direction to move for comparing string data. 0 -> left-to-right, 1 -> right-to-left.
- **Interrupt Flag (IF) bit 9** - 1 disables external interrupts, and 0 enables external interrupts.
- **Trap Flag (TF) bit 8** - Allows operation of the processor in single-step mode.
- **Sign Flag (SF) bit 7** - Shows the sign of the result of an arithmetic operation. 0 -> positive, 1 -> negative
- **Zero Flag (ZF) bit 6** - Indicates the result of an arithmetic or comparison operation. 0 -> a nonzero result, 1 -> a zero result.
- **Auxiliary Carry Flag (AF) bit 4** - Is set when a 1-byte arithmetic operation causes a carry from bit 3 into bit 4.
- **Parity Flag (PF) bit 2** - Indicates the total number of 1-bits in the result obtained from an arithmetic operation. 0 -> even number of 1-bits, 1 -> odd number of 1-bits.
- **Carry Flag (CF) bit 0** - It contains the carry of 0 or 1 from a high-order bit after an arithmetic oepration or the last bit of a shift or rotate operation.
