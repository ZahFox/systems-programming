.data
msg1:	   .asciiz "Hello, world!\n"

.text
main:
    la    $a0, msg1
    jal   print_str1
    
    la    $a0, msg1
    li    $a1, 14
    jal   print_str2
    
    la    $a0, msg1
    jal   print_str3
    
    li	  $v0, 10
    syscall
    
# print_str1 - print a string one char at a time in a while loop
#       $a0  - pointer to beginning of string
print_str1: 
    lbu   $t0, 0($a0)
    bne   $t0, $zero, print_char1 # while (*print_me != '\0')
    jr    $ra
print_char1:
    move  $t0, $a0
    lb    $a0, 0($t0)   
    li	  $v0, 11 
    syscall                   # print(*print_me);
    addiu $a0, $t0, 1         # print_me++
    j     print_str1
    

# print_str2 - print a string one char at a time in a for loop
#       $a0  - pointer to beginning of string
#       $a1  - the length of the string
print_str2:
    li    $t0, 0
    move  $t1, $a0
print_char2:
    bgt   $t0, $a1, print_str2_ret
    lbu   $a0, 0($t1)   
    li	  $v0, 11 
    syscall
    addiu $t0, $t0, 1
    addiu $t1, $t1, 1
    j     print_char2
print_str2_ret:
    jr   $ra
    
# print_str3 - print a string one char at a time in a do while loop
#       $a0  - pointer to beginning of string
print_str3:
    lbu    $t0, 0($a0)
    beq    $t0, $zero, print_str3_ret
print_char3:
    move   $t0, $a0
    lbu    $a0, 0($t0)
    li     $v0, 11
    syscall
    addiu  $a0, $t0, 1
    lbu    $t0, 1($t0)
    bgt    $t0, $zero, print_char3
print_str3_ret:
    jr   $ra
