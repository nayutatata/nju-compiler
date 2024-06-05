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
_mod:
  move $fp,$sp
  addi $sp,$sp,-20000
  lw $t0,20($fp)
  lw $t1,24($fp)
  div $t0,$t0,$t1
  sw $t0,-4($fp)
  lw $t2,-4($fp)
  lw $t0,24($fp)
  mul $t2,$t2,$t0
  sw $t2,-8($fp)
  lw $t1,20($fp)
  lw $t2,-8($fp)
  sub $t1,$t1,$t2
  sw $t1,-12($fp)
  lw $t0,-12($fp)
  move $v0,$t0
  move $sp,$fp
  jr $ra
main:
  move $fp,$sp
  addi $sp,$sp,-20000
  li $t1,2
  addi $sp,-4
  sw $t1,0($sp)
  li $t2,3
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
  jal _mod
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
  lw $t1,-4($fp)
  move $a0,$t1
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
