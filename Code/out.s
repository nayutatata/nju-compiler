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
_foo:
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
main:
  move $fp,$sp
  addi $sp,$sp,-20000
  li $t0,2
  addi $sp,-4
  sw $t0,0($sp)
  li $t1,1
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
  jal _foo
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
  addi $sp,8
  move $t2,$v0
  sw $t2,-4($fp)
  lw $t0,-4($fp)
  move $v0,$t0
  move $sp,$fp
  jr $ra
