        .data
_s1:    .ascii  "cocks:"
_s2:    .ascii  "rabbits:"
_s3:    .ascii  "cocks:"
_s4:    .ascii  "rabbits:"
        .text
         move,$fp,$sp
         sw,$fp,-4($sp)
         sw,$fp,-8($sp)
         b,main
                #       ENTER calculation:
calculation:
         addi,$sp,$sp,-68
                #               BEC,0,-,n
         li,$t0,0
         move,$t1,$fp
         sw,$t0,-28($t1)
                #               BEC,1,-,cock
         li,$t0,1
         move,$t1,$fp
         sw,$t0,-32($t1)
                #       LABEL L0:
$L0:
                #               SMOE,cock,(head),_t0
         move,$t1,$fp
         lw,$t0,-32($t1)
         move,$t2,$fp
         lw,$t1,-20($t2)
         lw,$t1,0($t1)
         sle,$t2,$t0,$t1
         move,$t3,$fp
         sw,$t2,-40($t3)
                #               JZ,_t0,L2
         move,$t1,$fp
         lw,$t0,-40($t1)
         beqz,$t0,$L2
                #               SUB,(head),cock,_t1
         move,$t1,$fp
         lw,$t0,-20($t1)
         lw,$t0,0($t0)
         move,$t2,$fp
         lw,$t1,-32($t2)
         sub,$t2,$t0,$t1
         move,$t3,$fp
         sw,$t2,-44($t3)
                #               BEC,_t1,-,rabbit
         move,$t1,$fp
         lw,$t0,-44($t1)
         move,$t1,$fp
         sw,$t0,-36($t1)
                #               MUL,cock,2,_t2
         move,$t1,$fp
         lw,$t0,-32($t1)
         li,$t1,2
         mult,$t0,$t1
         mflo,$t2
         move,$t3,$fp
         sw,$t2,-48($t3)
                #               MUL,rabbit,4,_t3
         move,$t1,$fp
         lw,$t0,-36($t1)
         li,$t1,4
         mult,$t0,$t1
         mflo,$t2
         move,$t3,$fp
         sw,$t2,-52($t3)
                #               ADD,_t2,_t3,_t4
         move,$t1,$fp
         lw,$t0,-48($t1)
         move,$t2,$fp
         lw,$t1,-52($t2)
         add,$t2,$t0,$t1
         move,$t3,$fp
         sw,$t2,-56($t3)
                #               EQ,_t4,(foot),_t5
         move,$t1,$fp
         lw,$t0,-56($t1)
         move,$t2,$fp
         lw,$t1,-24($t2)
         lw,$t1,0($t1)
         seq,$t2,$t0,$t1
         move,$t3,$fp
         sw,$t2,-60($t3)
                #               JZ,_t5,L1
         move,$t1,$fp
         lw,$t0,-60($t1)
         beqz,$t0,$L1
                #               WRITE,-,-,"cocks:"
         li,$v0,4
         la,$a0,_s1
         syscall
                #               WRITE,-,-,cock
         move,$t1,$fp
         lw,$t0,-32($t1)
         li,$v0,1
         move,$a0,$t0
         syscall
                #               WRITE,-,-,"rabbits:"
         li,$v0,4
         la,$a0,_s2
         syscall
                #               WRITE,-,-,rabbit
         move,$t2,$fp
         lw,$t1,-36($t2)
         li,$v0,1
         move,$a0,$t1
         syscall
                #               ADD,n,1,_t6
         move,$t3,$fp
         lw,$t2,-28($t3)
         li,$t3,1
         add,$t4,$t2,$t3
         move,$t5,$fp
         sw,$t4,-64($t5)
                #               BEC,_t6,-,n
         move,$t3,$fp
         lw,$t2,-64($t3)
         move,$t3,$fp
         sw,$t2,-28($t3)
                #       LABEL L1:
$L1:
                #               ADD,cock,1,cock
         move,$t3,$fp
         lw,$t2,-32($t3)
         li,$t3,1
         add,$t4,$t2,$t3
         move,$t5,$fp
         sw,$t4,-32($t5)
                #               JMP,-,L0
         b,$L0
                #       LABEL L2:
$L2:
                #               EQ,n,0,_t7
         move,$t3,$fp
         lw,$t2,-28($t3)
         li,$t3,0
         seq,$t4,$t2,$t3
         move,$t5,$fp
         sw,$t4,-68($t5)
                #               JZ,_t7,L3
         move,$t3,$fp
         lw,$t2,-68($t3)
         beqz,$t2,$L3
                #               WRITE,-,-,"cocks:"
         li,$v0,4
         la,$a0,_s3
         syscall
                #               WRITE,-,-,0
         li,$t2,0
         li,$v0,1
         move,$a0,$t2
         syscall
                #               WRITE,-,-,"rabbits:"
         li,$v0,4
         la,$a0,_s4
         syscall
                #               WRITE,-,-,0
         li,$t3,0
         li,$v0,1
         move,$a0,$t3
         syscall
                #       LABEL L3:
$L3:
                #               END,-,-,calculation
         move,$t5,$fp
         lw,$t4,-52($t5)
         move,$v0,$t4
         addi,$sp,$sp,68
         jr,$ra
                #       ENTER main:
main:
         addi,$sp,$sp,-76
                #               BEC,0,-,i
         li,$t5,0
         move,$t6,$fp
         sw,$t5,-44($t6)
                #       LABEL L4:
$L4:
                #               SMOE,i,2,_t8
         move,$t6,$fp
         lw,$t5,-44($t6)
         li,$t6,2
         sle,$t7,$t5,$t6
         move,$s0,$fp
         sw,$t7,-56($s0)
                #               JZ,_t8,L8
         move,$t6,$fp
         lw,$t5,-56($t6)
         beqz,$t5,$L8
                #               EQ,i,0,_t9
         move,$t6,$fp
         lw,$t5,-44($t6)
         li,$t6,0
         seq,$t7,$t5,$t6
         move,$s0,$fp
         sw,$t7,-60($s0)
                #               JZ,_t9,L5
         move,$t6,$fp
         lw,$t5,-60($t6)
         beqz,$t5,$L5
                #               BEC,4,-,head
         li,$t5,4
         move,$t6,$fp
         sw,$t5,-48($t6)
                #               BEC,8,-,foot
         li,$t5,8
         move,$t6,$fp
         sw,$t5,-52($t6)
                #       LABEL L5:
$L5:
                #               EQ,i,1,_t10
         move,$t6,$fp
         lw,$t5,-44($t6)
         li,$t6,1
         seq,$t7,$t5,$t6
         move,$s0,$fp
         sw,$t7,-64($s0)
                #               JZ,_t10,L6
         move,$t6,$fp
         lw,$t5,-64($t6)
         beqz,$t5,$L6
                #               BEC,8,-,head
         li,$t5,8
         move,$t6,$fp
         sw,$t5,-48($t6)
                #               BEC,4,-,foot
         li,$t5,4
         move,$t6,$fp
         sw,$t5,-52($t6)
                #       LABEL L6:
$L6:
                #               EQ,i,2,_t11
         move,$t6,$fp
         lw,$t5,-44($t6)
         li,$t6,2
         seq,$t7,$t5,$t6
         move,$s0,$fp
         sw,$t7,-68($s0)
                #               JZ,_t11,L7
         move,$t6,$fp
         lw,$t5,-68($t6)
         beqz,$t5,$L7
                #               BEC,3,-,head
         li,$t5,3
         move,$t6,$fp
         sw,$t5,-48($t6)
                #               BEC,5,-,foot
         li,$t5,5
         move,$t6,$fp
         sw,$t5,-52($t6)
                #       LABEL L7:
$L7:
                #               BEC,head,i,headarray
         move,$t5,$fp
         move,$s0,$fp
         lw,$t7,-44($s0)
         addi,$t6,$t5,-28
         li,$s0,4
         mult,$s0,$t7
         mflo,$s0
         add,$t6,$s0,$t6
         move,$s2,$fp
         lw,$s1,-48($s2)
         sw,$s1,0($t6)
                #               BEC,foot,i,footarray
         move,$t5,$fp
         move,$s0,$fp
         lw,$t7,-44($s0)
         addi,$t6,$t5,-40
         li,$s0,4
         mult,$s0,$t7
         mflo,$s0
         add,$t6,$s0,$t6
         move,$s2,$fp
         lw,$s1,-52($s2)
         sw,$s1,0($t6)
                #               GETADD,headarray,i,_t12
         move,$t5,$fp
         move,$s0,$fp
         lw,$t7,-44($s0)
         addi,$t6,$t5,-28
         li,$s0,4
         mult,$s0,$t7
         mflo,$s0
         add,$t6,$s0,$t6
         move,$s1,$fp
         sw,$t6,-72($s1)
                #               PUSH,_t12
         move,$t6,$fp
         lw,$t5,-72($t6)
         sw,$t5,-20($sp)
                #               GETADD,footarray,i,_t13
         move,$t5,$fp
         move,$s0,$fp
         lw,$t7,-44($s0)
         addi,$t6,$t5,-40
         li,$s0,4
         mult,$s0,$t7
         mflo,$s0
         add,$t6,$s0,$t6
         move,$s1,$fp
         sw,$t6,-76($s1)
                #               PUSH,_t13
         move,$t6,$fp
         lw,$t5,-76($t6)
         sw,$t5,-24($sp)
                #               CALL,calculation,2,-
         move,$t5,$fp
         lw,$t6,-8($t5)
         sw,$fp,-4($sp)
         sw,$t6,-8($sp)
         sw,$ra,-12($sp)
         sw,$v0,-16($sp)
         move,$fp,$sp
         jal,calculation
         lw,$fp,-4($sp)
         lw,$ra,-12($sp)
         lw,$v0,-16($sp)
                #               ADD,i,1,i
         move,$t6,$fp
         lw,$t5,-44($t6)
         li,$t6,1
         add,$t7,$t5,$t6
         move,$s0,$fp
         sw,$t7,-44($s0)
                #               JMP,-,L4
         b,$L4
                #       LABEL L8:
$L8:
                #               END,-,-,-
         addi,$sp,$sp,76
