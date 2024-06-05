.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
  li $v0,4
  la $a0,_prompt
  syscall
  li $v0,5
  syscall
  jr $ra
write:
  li $v0,1
  syscall
  li $v0,4
  la $a0,_ret
  syscall
  move $v0,$0
  jr $ra
_add:
  move $fp,$sp
  addi $sp,$sp,-20000
  lw $t0,20($fp)
  lw $t1,24($fp)
  add $t0,$t0,$t1
  sw $t0,-4($fp)
  lw $t2,-4($fp)
  move $v0,$t2
  move $sp,$fp
  jr $ra
_sub:
  move $fp,$sp
  addi $sp,$sp,-20000
  lw $t0,20($fp)
  lw $t1,24($fp)
  sub $t0,$t0,$t1
  sw $t0,-4($fp)
  lw $t2,-4($fp)
  move $v0,$t2
  move $sp,$fp
  jr $ra
_mul:
  move $fp,$sp
  addi $sp,$sp,-20000
  lw $t0,20($fp)
  lw $t1,24($fp)
  mul $t0,$t0,$t1
  sw $t0,-4($fp)
  lw $t2,-4($fp)
  move $v0,$t2
  move $sp,$fp
  jr $ra
main:
  move $fp,$sp
  addi $sp,$sp,-20000
  addi $sp,-4
  sw $t0,0($sp)
  addi $sp,-4
  sw $t1,0($sp)
  addi $sp,-4
  sw $t2,0($sp)
  addi $sp,-4
  sw $ra,0($sp)
  addi $sp,-4
  sw $fp,0($sp)
  jal read
  lw $fp,0($sp)
  addi $sp 4
  lw $ra,0($sp)
  addi $sp 4
  lw $t2,0($sp)
  addi $sp 4
  lw $t1,0($sp)
  addi $sp 4
  lw $t0,0($sp)
  addi $sp 4
  move $t0,$v0
  sw $t0,-4($fp)
  lw $t2,-4($fp)
  move $t1,$t2
  sw $t1,-8($fp)
  addi $sp,-4
  sw $t0,0($sp)
  addi $sp,-4
  sw $t1,0($sp)
  addi $sp,-4
  sw $t2,0($sp)
  addi $sp,-4
  sw $ra,0($sp)
  addi $sp,-4
  sw $fp,0($sp)
  jal read
  lw $fp,0($sp)
  addi $sp 4
  lw $ra,0($sp)
  addi $sp 4
  lw $t2,0($sp)
  addi $sp 4
  lw $t1,0($sp)
  addi $sp 4
  lw $t0,0($sp)
  addi $sp 4
  move $t0,$v0
  sw $t0,-12($fp)
  lw $t2,-12($fp)
  move $t1,$t2
  sw $t1,-16($fp)
  addi $sp,-4
  sw $t0,0($sp)
  addi $sp,-4
  sw $t1,0($sp)
  addi $sp,-4
  sw $t2,0($sp)
  addi $sp,-4
  sw $ra,0($sp)
  addi $sp,-4
  sw $fp,0($sp)
  jal read
  lw $fp,0($sp)
  addi $sp 4
  lw $ra,0($sp)
  addi $sp 4
  lw $t2,0($sp)
  addi $sp 4
  lw $t1,0($sp)
  addi $sp 4
  lw $t0,0($sp)
  addi $sp 4
  move $t0,$v0
  sw $t0,-20($fp)
  lw $t2,-20($fp)
  move $t1,$t2
  sw $t1,-24($fp)
  addi $sp,-4
  sw $t0,0($sp)
  addi $sp,-4
  sw $t1,0($sp)
  addi $sp,-4
  sw $t2,0($sp)
  addi $sp,-4
  sw $ra,0($sp)
  addi $sp,-4
  sw $fp,0($sp)
  jal read
  lw $fp,0($sp)
  addi $sp 4
  lw $ra,0($sp)
  addi $sp 4
  lw $t2,0($sp)
  addi $sp 4
  lw $t1,0($sp)
  addi $sp 4
  lw $t0,0($sp)
  addi $sp 4
  move $t0,$v0
  sw $t0,-28($fp)
  lw $t2,-28($fp)
  move $t1,$t2
  sw $t1,-32($fp)
  lw $t0,-32($fp)
  addi $sp,-4
  sw $t0,0($sp)
  lw $t1,-24($fp)
  addi $sp,-4
  sw $t1,0($sp)
  addi $sp,-4
  sw $t0,0($sp)
  addi $sp,-4
  sw $t1,0($sp)
  addi $sp,-4
  sw $t2,0($sp)
  addi $sp,-4
  sw $ra,0($sp)
  addi $sp,-4
  sw $fp,0($sp)
  jal _add
  lw $fp,0($sp)
  addi $sp 4
  lw $ra,0($sp)
  addi $sp 4
  lw $t2,0($sp)
  addi $sp 4
  lw $t1,0($sp)
  addi $sp 4
  lw $t0,0($sp)
  addi $sp 4
  move $t2,$v0
  sw $t2,-36($fp)
  lw $t0,-36($fp)
  addi $sp,-4
  sw $t0,0($sp)
  lw $t1,-16($fp)
  addi $sp,-4
  sw $t1,0($sp)
  lw $t2,-8($fp)
  addi $sp,-4
  sw $t2,0($sp)
  addi $sp,-4
  sw $t0,0($sp)
  addi $sp,-4
  sw $t1,0($sp)
  addi $sp,-4
  sw $t2,0($sp)
  addi $sp,-4
  sw $ra,0($sp)
  addi $sp,-4
  sw $fp,0($sp)
  jal _sub
  lw $fp,0($sp)
  addi $sp 4
  lw $ra,0($sp)
  addi $sp 4
  lw $t2,0($sp)
  addi $sp 4
  lw $t1,0($sp)
  addi $sp 4
  lw $t0,0($sp)
  addi $sp 4
  move $t0,$v0
  sw $t0,-40($fp)
  lw $t1,-40($fp)
  addi $sp,-4
  sw $t1,0($sp)
  lw $t2,-16($fp)
  addi $sp,-4
  sw $t2,0($sp)
  lw $t0,-8($fp)
  addi $sp,-4
  sw $t0,0($sp)
  addi $sp,-4
  sw $t0,0($sp)
  addi $sp,-4
  sw $t1,0($sp)
  addi $sp,-4
  sw $t2,0($sp)
  addi $sp,-4
  sw $ra,0($sp)
  addi $sp,-4
  sw $fp,0($sp)
  jal _sub
  lw $fp,0($sp)
  addi $sp 4
  lw $ra,0($sp)
  addi $sp 4
  lw $t2,0($sp)
  addi $sp 4
  lw $t1,0($sp)
  addi $sp 4
  lw $t0,0($sp)
  addi $sp 4
  move $t1,$v0
  sw $t1,-44($fp)
  lw $t2,-44($fp)
  addi $sp,-4
  sw $t2,0($sp)
  addi $sp,-4
  sw $t0,0($sp)
  addi $sp,-4
  sw $t1,0($sp)
  addi $sp,-4
  sw $t2,0($sp)
  addi $sp,-4
  sw $ra,0($sp)
  addi $sp,-4
  sw $fp,0($sp)
  jal _mul
  lw $fp,0($sp)
  addi $sp 4
  lw $ra,0($sp)
  addi $sp 4
  lw $t2,0($sp)
  addi $sp 4
  lw $t1,0($sp)
  addi $sp 4
  lw $t0,0($sp)
  addi $sp 4
  move $t0,$v0
  sw $t0,-48($fp)
  lw $t1,-48($fp)
  addi $sp,-4
  sw $t1,0($sp)
  addi $sp,-4
  sw $t0,0($sp)
  addi $sp,-4
  sw $t1,0($sp)
  addi $sp,-4
  sw $t2,0($sp)
  addi $sp,-4
  sw $ra,0($sp)
  addi $sp,-4
  sw $fp,0($sp)
  jal _add
  lw $fp,0($sp)
  addi $sp 4
  lw $ra,0($sp)
  addi $sp 4
  lw $t2,0($sp)
  addi $sp 4
  lw $t1,0($sp)
  addi $sp 4
  lw $t0,0($sp)
  addi $sp 4
  move $t2,$v0
  sw $t2,-52($fp)
  lw $t1,-52($fp)
  move $t0,$t1
  sw $t0,-56($fp)
  lw $t2,-56($fp)
  move $a0,$t2
  addi $sp,-4
  sw $t0,0($sp)
  addi $sp,-4
  sw $t1,0($sp)
  addi $sp,-4
  sw $t2,0($sp)
  addi $sp,-4
  sw $ra,0($sp)
  addi $sp,-4
  sw $fp,0($sp)
  jal write
  lw $fp,0($sp)
  addi $sp 4
  lw $ra,0($sp)
  addi $sp 4
  lw $t2,0($sp)
  addi $sp 4
  lw $t1,0($sp)
  addi $sp 4
  lw $t0,0($sp)
  addi $sp 4
  li $t0,0
  move $v0,$t0
  move $sp,$fp
  jr $ra
