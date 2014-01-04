        .data
        .text
         move,$fp,$sp
         sw,$fp,-4($sp)
         sw,$fp,-8($sp)
         b,main
                #       ENTER swap:
swap:
         addi,$sp,$sp,-28
                #               BEC,(a),-,c
         move,$t1,$fp
         lw,$t0,-20($t1)
         lw,$t0,0($t0)
         move,$t1,$fp
         sw,$t0,-28($t1)
                #               BEC,(b),-,(a)
         move,$t1,$fp
         lw,$t0,-24($t1)
         lw,$t0,0($t0)
         move,$t1,$fp
         lw,$t1,-20($t1)
         sw,$t0,0($t1)
                #               BEC,c,-,(b)
         move,$t1,$fp
         lw,$t0,-28($t1)
         move,$t1,$fp
         lw,$t1,-24($t1)
         sw,$t0,0($t1)
                #               END,-,-,swap
         move,$t1,$fp
         lw,$t0,-44($t1)
         move,$v0,$t0
         addi,$sp,$sp,28
         jr,$ra
                #       ENTER main:
main:
         addi,$sp,$sp,-60
                #               READ,-,-,c
         li,$v0,5
         syscall
         move $t1,$v0
         move,$t2,$fp
         sw,$t1,-40($t2)
                #               READ,-,-,d
         li,$v0,5
         syscall
         move $t1,$v0
         move,$t2,$fp
         sw,$t1,-44($t2)
                #               BEC,c,1,a
         move,$t1,$fp
         li,$t2,1
         addi,$t1,$t1,-36
         li,$t3,4
         mult,$t3,$t2
         mflo,$t3
         add,$t1,$t3,$t1
         move,$t5,$fp
         lw,$t4,-40($t5)
         sw,$t4,0($t1)
                #               BEC,d,2,a
         move,$t1,$fp
         li,$t2,2
         addi,$t1,$t1,-36
         li,$t3,4
         mult,$t3,$t2
         mflo,$t3
         add,$t1,$t3,$t1
         move,$t6,$fp
         lw,$t5,-44($t6)
         sw,$t5,0($t1)
                #               GETADD,a,1,_t0
         move,$t1,$fp
         li,$t2,1
         addi,$t1,$t1,-36
         li,$t3,4
         mult,$t3,$t2
         mflo,$t3
         add,$t1,$t3,$t1
         move,$t6,$fp
         sw,$t1,-48($t6)
                #               PUSH,_t0
         move,$t2,$fp
         lw,$t1,-48($t2)
         sw,$t1,-20($sp)
                #               GETADD,a,2,_t1
         move,$t1,$fp
         li,$t2,2
         addi,$t1,$t1,-36
         li,$t3,4
         mult,$t3,$t2
         mflo,$t3
         add,$t1,$t3,$t1
         move,$t6,$fp
         sw,$t1,-52($t6)
                #               PUSH,_t1
         move,$t2,$fp
         lw,$t1,-52($t2)
         sw,$t1,-24($sp)
                #               CALL,swap,2,-
         move,$t1,$fp
         lw,$t1,-8($t1)
         sw,$fp,-4($sp)
         sw,$t1,-8($sp)
         sw,$ra,-12($sp)
         sw,$v0,-16($sp)
         move,$fp,$sp
         jal,swap
         lw,$fp,-4($sp)
         lw,$ra,-12($sp)
         lw,$v0,-16($sp)
                #               GETARR,a,1,_t2
         move,$t1,$fp
         li,$t2,1
         addi,$t1,$t1,-36
         li,$t3,4
         mult,$t3,$t2
         mflo,$t3
         add,$t1,$t3,$t1
         lw,$t3,0($t1)
         move,$t6,$fp
         sw,$t3,-56($t6)
                #               WRITE,-,-,_t2
         move,$t2,$fp
         lw,$t1,-56($t2)
         li,$v0,1
         move,$a0,$t1
         syscall
                #               GETARR,a,2,_t3
         move,$t2,$fp
         li,$t3,2
         addi,$t2,$t2,-36
         li,$t6,4
         mult,$t6,$t3
         mflo,$t6
         add,$t2,$t6,$t2
         lw,$t6,0($t2)
         move,$t7,$fp
         sw,$t6,-60($t7)
                #               WRITE,-,-,_t3
         move,$t3,$fp
         lw,$t2,-60($t3)
         li,$v0,1
         move,$a0,$t2
         syscall
                #               END,-,-,-
         addi,$sp,$sp,60
