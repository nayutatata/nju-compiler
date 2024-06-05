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
_gcd:
  move $fp,$sp
  addi $sp,$sp,-20000
  lw $t1,20($fp)
  li $t2,0
  beq $t1,$t2,label0
  j label1
label0:
  lw $t0,24($fp)
  move $v0,$t0
  move $sp,$fp
  jr $ra
label1:
  lw $t1,20($fp)
  addi $sp,-4
  sw $t1,0($sp)
  lw $t2,20($fp)
  addi $sp,-4
  sw $t2,0($sp)
  lw $t0,24($fp)
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
  move $t1,$v0
  sw $t1,-4($fp)
  lw $t2,-4($fp)
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
  jal _gcd
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
  sw $t0,-8($fp)
  lw $t1,-8($fp)
  move $v0,$t1
  move $sp,$fp
  jr $ra
_lcm:
  move $fp,$sp
  addi $sp,$sp,-20000
  lw $t2,20($fp)
  lw $t0,24($fp)
  mul $t2,$t2,$t0
  sw $t2,-4($fp)
  lw $t1,24($fp)
  addi $sp,-4
  sw $t1,0($sp)
  lw $t2,20($fp)
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
  jal _gcd
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
  sw $t0,-8($fp)
  lw $t1,-4($fp)
  lw $t2,-8($fp)
  div $t1,$t1,$t2
  sw $t1,-12($fp)
  lw $t0,-12($fp)
  move $v0,$t0
  move $sp,$fp
  jr $ra
main:
  move $fp,$sp
  addi $sp,$sp,-20000
  li $t1,0
  sw $t1,-4($fp)
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
  move $t2,$v0
  sw $t2,-408($fp)
  lw $t1,-408($fp)
  move $t0,$t1
  sw $t0,-412($fp)
  li $t2,0
  sw $t2,-416($fp)
label2:
  lw $t0,-416($fp)
  lw $t1,-412($fp)
  blt $t0,$t1,label3
  j label4
label3:
  li $t2,0
  sw $t2,-420($fp)
  li $t0,4
  sw $t0,-424($fp)
  lw $t1,-416($fp)
  lw $t2,-424($fp)
  mul $t1,$t1,$t2
  sw $t1,-428($fp)
  lw $t0,-420($fp)
  lw $t1,-428($fp)
  add $t0,$t0,$t1
  sw $t0,-420($fp)
  lw $t2,-424($fp)
  li $t0,50
  mul $t2,$t2,$t0
  sw $t2,-424($fp)
  addi $t1,$fp,-204
  sw $t1,-432($fp)
  lw $t2,-432($fp)
  lw $t0,-420($fp)
  add $t2,$t2,$t0
  sw $t2,-436($fp)
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
  move $t1,$v0
  sw $t1,-440($fp)
  lw $t2,-436($fp)
  lw $t0,-440($fp)
  sw $t0,0($t2)
  li $t1,0
  sw $t1,-444($fp)
  li $t2,4
  sw $t2,-448($fp)
  lw $t0,-416($fp)
  lw $t1,-448($fp)
  mul $t0,$t0,$t1
  sw $t0,-452($fp)
  lw $t2,-444($fp)
  lw $t0,-452($fp)
  add $t2,$t2,$t0
  sw $t2,-444($fp)
  lw $t1,-448($fp)
  li $t2,50
  mul $t1,$t1,$t2
  sw $t1,-448($fp)
  addi $t0,$fp,-404
  sw $t0,-456($fp)
  lw $t1,-456($fp)
  lw $t2,-444($fp)
  add $t1,$t1,$t2
  sw $t1,-460($fp)
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
  sw $t0,-464($fp)
  lw $t1,-460($fp)
  lw $t2,-464($fp)
  sw $t2,0($t1)
  lw $t0,-416($fp)
  li $t1,1
  add $t0,$t0,$t1
  sw $t0,-468($fp)
  lw $t0,-468($fp)
  move $t2,$t0
  sw $t2,-416($fp)
  j label2
label4:
  li $t1,0
  sw $t1,-472($fp)
  li $t2,4
  sw $t2,-476($fp)
  li $t0,0
  lw $t1,-476($fp)
  mul $t0,$t0,$t1
  sw $t0,-480($fp)
  lw $t2,-472($fp)
  lw $t0,-480($fp)
  add $t2,$t2,$t0
  sw $t2,-472($fp)
  lw $t1,-476($fp)
  li $t2,50
  mul $t1,$t1,$t2
  sw $t1,-476($fp)
  addi $t0,$fp,-404
  sw $t0,-484($fp)
  lw $t1,-484($fp)
  lw $t2,-472($fp)
  add $t1,$t1,$t2
  sw $t1,-488($fp)
  lw $t1,-488($fp)
  lw $t0,0($t1)
  sw $t0,-492($fp)
  li $t2,1
  sw $t2,-416($fp)
label5:
  lw $t0,-416($fp)
  lw $t1,-412($fp)
  blt $t0,$t1,label6
  j label7
label6:
  li $t2,0
  sw $t2,-496($fp)
  li $t0,4
  sw $t0,-500($fp)
  lw $t1,-416($fp)
  lw $t2,-500($fp)
  mul $t1,$t1,$t2
  sw $t1,-504($fp)
  lw $t0,-496($fp)
  lw $t1,-504($fp)
  add $t0,$t0,$t1
  sw $t0,-496($fp)
  lw $t2,-500($fp)
  li $t0,50
  mul $t2,$t2,$t0
  sw $t2,-500($fp)
  addi $t1,$fp,-404
  sw $t1,-508($fp)
  lw $t2,-508($fp)
  lw $t0,-496($fp)
  add $t2,$t2,$t0
  sw $t2,-512($fp)
  lw $t2,-512($fp)
  lw $t1,0($t2)
  sw $t1,-516($fp)
  lw $t0,-516($fp)
  addi $sp,-4
  sw $t0,0($sp)
  lw $t1,-492($fp)
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
  jal _lcm
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
  sw $t2,-520($fp)
  lw $t1,-520($fp)
  move $t0,$t1
  sw $t0,-492($fp)
  lw $t2,-416($fp)
  li $t0,1
  add $t2,$t2,$t0
  sw $t2,-524($fp)
  lw $t2,-524($fp)
  move $t1,$t2
  sw $t1,-416($fp)
  j label5
label7:
  li $t0,0
  sw $t0,-416($fp)
label8:
  lw $t1,-416($fp)
  lw $t2,-412($fp)
  blt $t1,$t2,label9
  j label10
label9:
  li $t0,0
  sw $t0,-528($fp)
  li $t1,4
  sw $t1,-532($fp)
  lw $t2,-416($fp)
  lw $t0,-532($fp)
  mul $t2,$t2,$t0
  sw $t2,-536($fp)
  lw $t1,-528($fp)
  lw $t2,-536($fp)
  add $t1,$t1,$t2
  sw $t1,-528($fp)
  lw $t0,-532($fp)
  li $t1,50
  mul $t0,$t0,$t1
  sw $t0,-532($fp)
  addi $t2,$fp,-204
  sw $t2,-540($fp)
  lw $t0,-540($fp)
  lw $t1,-528($fp)
  add $t0,$t0,$t1
  sw $t0,-544($fp)
  li $t2,0
  sw $t2,-548($fp)
  li $t0,4
  sw $t0,-552($fp)
  lw $t1,-416($fp)
  lw $t2,-552($fp)
  mul $t1,$t1,$t2
  sw $t1,-556($fp)
  lw $t0,-548($fp)
  lw $t1,-556($fp)
  add $t0,$t0,$t1
  sw $t0,-548($fp)
  lw $t2,-552($fp)
  li $t0,50
  mul $t2,$t2,$t0
  sw $t2,-552($fp)
  addi $t1,$fp,-404
  sw $t1,-560($fp)
  lw $t2,-560($fp)
  lw $t0,-548($fp)
  add $t2,$t2,$t0
  sw $t2,-564($fp)
  lw $t2,-564($fp)
  lw $t1,0($t2)
  sw $t1,-568($fp)
  lw $t0,-492($fp)
  lw $t1,-568($fp)
  div $t0,$t0,$t1
  sw $t0,-572($fp)
  lw $t0,-544($fp)
  lw $t2,0($t0)
  sw $t2,-576($fp)
  lw $t1,-576($fp)
  lw $t2,-572($fp)
  mul $t1,$t1,$t2
  sw $t1,-580($fp)
  lw $t0,-4($fp)
  lw $t1,-580($fp)
  add $t0,$t0,$t1
  sw $t0,-584($fp)
  lw $t0,-584($fp)
  move $t2,$t0
  sw $t2,-4($fp)
  lw $t1,-416($fp)
  li $t2,1
  add $t1,$t1,$t2
  sw $t1,-588($fp)
  lw $t1,-588($fp)
  move $t0,$t1
  sw $t0,-416($fp)
  j label8
label10:
  lw $t2,-492($fp)
  addi $sp,-4
  sw $t2,0($sp)
  lw $t0,-4($fp)
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
  jal _gcd
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
  sw $t1,-592($fp)
  lw $t0,-592($fp)
  move $t2,$t0
  sw $t2,-596($fp)
  lw $t1,-4($fp)
  lw $t2,-596($fp)
  div $t1,$t1,$t2
  sw $t1,-600($fp)
  lw $t1,-600($fp)
  move $t0,$t1
  sw $t0,-4($fp)
  lw $t2,-492($fp)
  lw $t0,-596($fp)
  div $t2,$t2,$t0
  sw $t2,-604($fp)
  lw $t2,-604($fp)
  move $t1,$t2
  sw $t1,-492($fp)
  lw $t0,-492($fp)
  li $t1,1
  beq $t0,$t1,label11
  j label12
label11:
  lw $t2,-4($fp)
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
  j label13
label12:
  lw $t0,-4($fp)
  move $a0,$t0
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
  lw $t1,-492($fp)
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
label13:
  li $t2,0
  move $v0,$t2
  move $sp,$fp
  jr $ra
