        .data
_s1:    .ascii  "shabi"
        .text
         move,$fp,$sp
         sw,$fp,-4($sp)
         sw,$fp,-8($sp)
         b,main
gcd:
         addi,$sp,$sp,-44
         move,$t1,$fp
         lw,$t0,-24($t1)
         li,$t1,0
         move,$t3,$fp
         lw,$t2,-28($t3)
         seq,$t2,$t0,$t1
         move,$t0,$fp
         sw,$t2,-28($t0)
         move,$t1,$fp
         lw,$t0,-28($t1)
         beqz,$t0,$L0
         move,$t1,$fp
         lw,$t0,-20($t1)
         move,$t1,$fp
         sw,$t0,-16($t1)
         b,$L1
$L0:
         move,$t1,$fp
         lw,$t0,-24($t1)
         sw,$t0,-20($sp)
         move,$t1,$fp
         lw,$t0,-20($t1)
         move,$t2,$fp
         lw,$t1,-24($t2)
         move,$t3,$fp
         lw,$t2,-32($t3)
         div,$t0,$t1
         mflo,$t2
         move,$t0,$fp
         sw,$t2,-32($t0)
         move,$t1,$fp
         lw,$t0,-32($t1)
         move,$t2,$fp
         lw,$t1,-24($t2)
         move,$t3,$fp
         lw,$t2,-36($t3)
         mult,$t0,$t1
         mflo,$t2
         move,$t0,$fp
         sw,$t2,-36($t0)
         move,$t1,$fp
         lw,$t0,-20($t1)
         move,$t2,$fp
         lw,$t1,-36($t2)
         move,$t3,$fp
         lw,$t2,-40($t3)
         sub,$t2,$t0,$t1
         move,$t0,$fp
         sw,$t2,-40($t0)
         move,$t1,$fp
         lw,$t0,-40($t1)
         sw,$t0,-24($sp)
         move,$t0,$fp
         lw,$t0,-8($t0)
         sw,$fp,-4($sp)
         sw,$t0,-8($sp)
         sw,$ra,-12($sp)
         sw,$v0,-16($sp)
         move,$fp,$sp
         jal,gcd
         lw,$fp,-4($sp)
         lw,$ra,-12($sp)
         move,$t1,$fp
         sw,$v0,-44($t1)
         lw,$v0,-16($sp)
         move,$t1,$fp
         lw,$t0,-44($t1)
         move,$t1,$fp
         sw,$t0,-16($t1)
$L1:
         move,$t1,$fp
         lw,$t0,-16($t1)
         move,$v0,$t0
         addi,$sp,$sp,44
         jr,$ra
main:
         addi,$sp,$sp,-28
         li,$v0,4
         la,$a0,_s1
         syscall
         li,$v0,5
         syscall
         move $t1,$v0
         move,$t2,$fp
         sw,$t1,-20($t2)
         li,$v0,5
         syscall
         move $t1,$v0
         move,$t2,$fp
         sw,$t1,-24($t2)
         move,$t2,$fp
         lw,$t1,-20($t2)
         sw,$t1,-20($sp)
         move,$t2,$fp
         lw,$t1,-24($t2)
         sw,$t1,-24($sp)
         move,$t1,$fp
         lw,$t1,-8($t1)
         sw,$fp,-4($sp)
         sw,$t1,-8($sp)
         sw,$ra,-12($sp)
         sw,$v0,-16($sp)
         move,$fp,$sp
         jal,gcd
         lw,$fp,-4($sp)
         lw,$ra,-12($sp)
         move,$t2,$fp
         sw,$v0,-28($t2)
         lw,$v0,-16($sp)
         move,$t2,$fp
         lw,$t1,-28($t2)
         li,$v0,1
         move,$a0,$t1
         syscall
         addi,$sp,$sp,28
