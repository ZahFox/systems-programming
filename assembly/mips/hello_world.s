.data
    msg:     .asciiz "Hello, world!\n"
    atl:     .asciiz "42\n"
    buffer:  .space  16384    # Buffer Size

.text
    jal hello
    jal patl
    jal csharp
    jal gets
    
    la $a0, buffer
    li $v0, 4
    syscall
    j exit
    
hello:
    la $a0, msg
    j print

patl:
    la  $a0, atl
    j print

gets:
    la  $a0, buffer
    li  $a1, 16384
    li  $v0, 8    # specify "Read String" syscall
    syscall
    jr  $ra

csharp:
    li  $a0, 61
    li  $a1, 1000
    li  $a2, 7
    li  $a3, 127
    li  $v0, 31
    syscall
    jr  $ra

print:
    li  $v0, 4    # specify "Print String" syscall
    syscall
    jr $ra
    
exit:
    li  $v0, 10  # specify the "Exit" syscall
    syscall