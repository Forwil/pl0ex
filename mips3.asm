        .data
_s1:    .ascii  "shabi\n"
        .text
         move,$fp,$sp
         sw,$fp,-4($sp)
         sw,$fp,-8($sp)
         b,main
mod:
         addi,$sp,$sp,-36
         move,$t1,$fp
         lw,$t0,-20($t1)
         move,$t2,$fp
         lw,$t1,-24($t2)
         move,$t3,$fp
         lw,$t2,-28($t3)
         div,$t0,$t1
         mflo,$t2
         move,$t0,$fp
         sw,$t2,-28($t0)
         move,$t1,$fp
         lw,$t0,-28($t1)
         move,$t2,$fp
         lw,$t1,-24($t2)
         move,$t3,$fp
         lw,$t2,-32($t3)
         mult,$t0,$t1
         mflo,$t2
         move,$t0,$fp
         sw,$t2,-32($t0)
         move,$t1,$fp
         lw,$t0,-20($t1)
         move,$t2,$fp
         lw,$t1,-32($t2)
         move,$t3,$fp
         lw,$t2,-36($t3)
         sub,$t2,$t0,$t1
         move,$t0,$fp
         sw,$t2,-36($t0)
         move,$t1,$fp
         lw,$t0,-36($t1)
         move,$t1,$fp
         sw,$t0,-16($t1)
         move,$t1,$fp
         lw,$t0,-16($t1)
         move,$v0,$t0
         addi,$sp,$sp,36
         jr,$ra
gcd:
         addi,$sp,$sp,-36
$L0:
         move,$t2,$fp
         lw,$t1,-24($t2)
         move,$t2,$fp
         sw,$t1,-28($t2)
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
         jal,mod
         lw,$fp,-4($sp)
         lw,$ra,-12($sp)
         move,$t2,$fp
         sw,$v0,-32($t2)
         lw,$v0,-16($sp)
         move,$t2,$fp
         lw,$t1,-32($t2)
         move,$t2,$fp
         sw,$t1,-24($t2)
         move,$t2,$fp
         lw,$t1,-28($t2)
         move,$t2,$fp
         sw,$t1,-20($t2)
         move,$t2,$fp
         lw,$t1,-24($t2)
         li,$t2,0
         move,$t4,$fp
         lw,$t3,-36($t4)
         seq,$t3,$t1,$t2
         move,$t1,$fp
         sw,$t3,-36($t1)
         move,$t2,$fp
         lw,$t1,-36($t2)
         beqz,$t1,$L0
         move,$t2,$fp
         lw,$t1,-20($t2)
         move,$t2,$fp
         sw,$t1,-16($t2)
         move,$t2,$fp
         lw,$t1,-16($t2)
         move,$v0,$t1
         addi,$sp,$sp,36
         jr,$ra
main:
         addi,$sp,$sp,-32
         li,$v0,4
         la,$a0,_s1
         syscall
         li,$v0,5
         syscall
         move $t2,$v0
         move,$t3,$fp
         sw,$t2,-20($t3)
         li,$v0,5
         syscall
         move $t2,$v0
         move,$t3,$fp
         sw,$t2,-24($t3)
         move,$t3,$fp
         lw,$t2,-20($t3)
         sw,$t2,-20($sp)
         move,$t3,$fp
         lw,$t2,-24($t3)
         sw,$t2,-24($sp)
         move,$t2,$fp
         lw,$t2,-8($t2)
         sw,$fp,-4($sp)
         sw,$t2,-8($sp)
         sw,$ra,-12($sp)
         sw,$v0,-16($sp)
         move,$fp,$sp
         jal,gcd
         lw,$fp,-4($sp)
         lw,$ra,-12($sp)
         move,$t3,$fp
         sw,$v0,-32($t3)
         lw,$v0,-16($sp)
         move,$t3,$fp
         lw,$t2,-32($t3)
         li,$v0,1
         move,$a0,$t2
         syscall
         addi,$sp,$sp,32