.text
main:
    li    $a0, 25
    jal   fib
print_int:
    move  $a0, $v0
    li	  $v0, 1
    syscall
exit:
    li	  $v0, 10
    syscall


# fib - a recursive implementation of the fibonacci function
#       $a0  - n - the starting number
fib:
    beq    $a0, $zero, return_0
    li     $t0, 1
    beq    $a0, $t0, return_1
    subiu  $t0, $a0, 1
    subiu  $s0, $a0, 2
    addiu  $sp, $sp, -8
    sw	   $ra, 4($sp)    # save n-2
    sw	   $s0, 0($sp)    # save n-2
    
    move   $a0, $t0
    jal    fib            # x = fib(n-1)
    lw     $s0, 0($sp)    # retrieve n-2
    sw     $v0, 0($sp)    # save x
    move   $a0, $s0
    jal    fib            # y = fib(n-2)
    lw     $s0, 0($sp)    # retrieve x
    lw     $ra, 4($sp)
    addu   $v0, $v0, $s0  # x = x + y
    addiu  $sp, $sp, 8
    jr     $ra
return_0:
    li     $v0, 0
    jr     $ra
return_1:
    li     $v0, 1
    jr     $ra