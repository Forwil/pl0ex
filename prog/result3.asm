        .data
_s1:    .ascii  " The ASCII of c1 is "
_s2:    .ascii  " The first letter is after m "
_s3:    .ascii  " The ASCII of c1 is "
_s4:    .ascii  " The first letter is after M "
_s5:    .ascii  " The letter you input is invalid "
_s6:    .ascii  " The result is: "
        .text
         move,$fp,$sp
         sw,$fp,-4($sp)
         sw,$fp,-8($sp)
         b,main
                #       ENTER main:
main:
         addi,$sp,$sp,-132
                #               BEC,1,-,i
         li,$t0,1
         move,$t1,$fp
         sw,$t0,-44($t1)
                #       LABEL L0:
$L0:
                #               SMOE,i,2,_t0
         move,$t1,$fp
         lw,$t0,-44($t1)
         li,$t1,2
         sle,$t2,$t0,$t1
         move,$t3,$fp
         sw,$t2,-48($t3)
                #               JZ,_t0,L13
         move,$t1,$fp
         lw,$t0,-48($t1)
         beqz,$t0,$L13
                #               EQ,i,1,_t1
         move,$t1,$fp
         lw,$t0,-44($t1)
         li,$t1,1
         seq,$t2,$t0,$t1
         move,$t3,$fp
         sw,$t2,-52($t3)
                #               JZ,_t1,L1
         move,$t1,$fp
         lw,$t0,-52($t1)
         beqz,$t0,$L1
                #               BEC,w,-,c1
         li,$t0,119
         move,$t1,$fp
         sw,$t0,-20($t1)
                #               BEC,e,-,c2
         li,$t0,101
         move,$t1,$fp
         sw,$t0,-24($t1)
                #               BEC,1,-,i1
         li,$t0,1
         move,$t1,$fp
         sw,$t0,-28($t1)
                #               BEC,2,-,i2
         li,$t0,2
         move,$t1,$fp
         sw,$t0,-32($t1)
                #       LABEL L1:
$L1:
                #               EQ,i,2,_t2
         move,$t1,$fp
         lw,$t0,-44($t1)
         li,$t1,2
         seq,$t2,$t0,$t1
         move,$t3,$fp
         sw,$t2,-56($t3)
                #               JZ,_t2,L2
         move,$t1,$fp
         lw,$t0,-56($t1)
         beqz,$t0,$L2
                #               BEC,F,-,c1
         li,$t0,70
         move,$t1,$fp
         sw,$t0,-20($t1)
                #               BEC,I,-,c2
         li,$t0,73
         move,$t1,$fp
         sw,$t0,-24($t1)
                #               BEC,3,-,i1
         li,$t0,3
         move,$t1,$fp
         sw,$t0,-28($t1)
                #               BEC,5,-,i2
         li,$t0,5
         move,$t1,$fp
         sw,$t0,-32($t1)
                #       LABEL L2:
$L2:
                #               BIGE,c1,a,_t3
         move,$t1,$fp
         lw,$t0,-20($t1)
         li,$t1,97
         sge,$t2,$t0,$t1
         move,$t3,$fp
         sw,$t2,-60($t3)
                #               JZ,_t3,L5
         move,$t1,$fp
         lw,$t0,-60($t1)
         beqz,$t0,$L5
                #               SMOE,c1,m,_t4
         move,$t1,$fp
         lw,$t0,-20($t1)
         li,$t1,109
         sle,$t2,$t0,$t1
         move,$t3,$fp
         sw,$t2,-64($t3)
                #               JZ,_t4,L3
         move,$t1,$fp
         lw,$t0,-64($t1)
         beqz,$t0,$L3
                #               SUB,c1,a,_t5
         move,$t1,$fp
         lw,$t0,-20($t1)
         li,$t1,97
         sub,$t2,$t0,$t1
         move,$t3,$fp
         sw,$t2,-68($t3)
                #               ADD,_t5,97,_t6
         move,$t1,$fp
         lw,$t0,-68($t1)
         li,$t1,97
         add,$t2,$t0,$t1
         move,$t3,$fp
         sw,$t2,-72($t3)
                #               BEC,_t6,-,ordc1
         move,$t1,$fp
         lw,$t0,-72($t1)
         move,$t1,$fp
         sw,$t0,-40($t1)
                #               WRITE,-,-," The ASCII of c1 is "
         li,$v0,4
         la,$a0,_s1
         syscall
                #               WRITE,-,-,ordc1
         move,$t1,$fp
         lw,$t0,-40($t1)
         li,$v0,1
         move,$a0,$t0
         syscall
                #               JMP,-,L4
         b,$L4
                #       LABEL L3:
$L3:
                #               WRITE,-,-," The first letter is after m "
         li,$v0,4
         la,$a0,_s2
         syscall
                #       LABEL L4:
$L4:
                #               JMP,-,L10
         b,$L10
                #       LABEL L5:
$L5:
                #               BIGE,c1,A,_t7
         move,$t2,$fp
         lw,$t1,-20($t2)
         li,$t2,65
         sge,$t3,$t1,$t2
         move,$t4,$fp
         sw,$t3,-76($t4)
                #               JZ,_t7,L8
         move,$t2,$fp
         lw,$t1,-76($t2)
         beqz,$t1,$L8
                #               SMOE,c1,M,_t8
         move,$t2,$fp
         lw,$t1,-20($t2)
         li,$t2,77
         sle,$t3,$t1,$t2
         move,$t4,$fp
         sw,$t3,-80($t4)
                #               JZ,_t8,L6
         move,$t2,$fp
         lw,$t1,-80($t2)
         beqz,$t1,$L6
                #               SUB,c1,A,_t9
         move,$t2,$fp
         lw,$t1,-20($t2)
         li,$t2,65
         sub,$t3,$t1,$t2
         move,$t4,$fp
         sw,$t3,-84($t4)
                #               ADD,_t9,65,_t10
         move,$t2,$fp
         lw,$t1,-84($t2)
         li,$t2,65
         add,$t3,$t1,$t2
         move,$t4,$fp
         sw,$t3,-88($t4)
                #               BEC,_t10,-,ordc1
         move,$t2,$fp
         lw,$t1,-88($t2)
         move,$t2,$fp
         sw,$t1,-40($t2)
                #               WRITE,-,-," The ASCII of c1 is "
         li,$v0,4
         la,$a0,_s3
         syscall
                #               WRITE,-,-,ordc1
         move,$t2,$fp
         lw,$t1,-40($t2)
         li,$v0,1
         move,$a0,$t1
         syscall
                #               JMP,-,L7
         b,$L7
                #       LABEL L6:
$L6:
                #               WRITE,-,-," The first letter is after M "
         li,$v0,4
         la,$a0,_s4
         syscall
                #       LABEL L7:
$L7:
                #               JMP,-,L9
         b,$L9
                #       LABEL L8:
$L8:
                #               WRITE,-,-," The letter you input is invalid "
         li,$v0,4
         la,$a0,_s5
         syscall
                #       LABEL L9:
$L9:
                #       LABEL L10:
$L10:
                #               SMOE,c1,c2,_t11
         move,$t3,$fp
         lw,$t2,-20($t3)
         move,$t4,$fp
         lw,$t3,-24($t4)
         sle,$t4,$t2,$t3
         move,$t5,$fp
         sw,$t4,-92($t5)
                #               JZ,_t11,L11
         move,$t3,$fp
         lw,$t2,-92($t3)
         beqz,$t2,$L11
                #               MUL,a,i1,_t12
         li,$t2,97
         move,$t4,$fp
         lw,$t3,-28($t4)
         mult,$t2,$t3
         mflo,$t4
         move,$t5,$fp
         sw,$t4,-96($t5)
                #               MUL,m,i2,_t13
         li,$t2,109
         move,$t4,$fp
         lw,$t3,-32($t4)
         mult,$t2,$t3
         mflo,$t4
         move,$t5,$fp
         sw,$t4,-100($t5)
                #               ADD,_t12,_t13,_t14
         move,$t3,$fp
         lw,$t2,-96($t3)
         move,$t4,$fp
         lw,$t3,-100($t4)
         add,$t4,$t2,$t3
         move,$t5,$fp
         sw,$t4,-104($t5)
                #               ADD,a,m,_t15
         li,$t2,97
         li,$t3,109
         add,$t4,$t2,$t3
         move,$t5,$fp
         sw,$t4,-108($t5)
                #               DIV,_t14,_t15,_t16
         move,$t3,$fp
         lw,$t2,-104($t3)
         move,$t4,$fp
         lw,$t3,-108($t4)
         div,$t2,$t3
         mflo,$t4
         move,$t5,$fp
         sw,$t4,-112($t5)
                #               BEC,_t16,-,i3
         move,$t3,$fp
         lw,$t2,-112($t3)
         move,$t3,$fp
         sw,$t2,-36($t3)
                #               JMP,-,L12
         b,$L12
                #       LABEL L11:
$L11:
                #               MUL,a,i2,_t17
         li,$t2,97
         move,$t4,$fp
         lw,$t3,-32($t4)
         mult,$t2,$t3
         mflo,$t4
         move,$t5,$fp
         sw,$t4,-116($t5)
                #               MUL,m,i1,_t18
         li,$t2,109
         move,$t4,$fp
         lw,$t3,-28($t4)
         mult,$t2,$t3
         mflo,$t4
         move,$t5,$fp
         sw,$t4,-120($t5)
                #               ADD,_t17,_t18,_t19
         move,$t3,$fp
         lw,$t2,-116($t3)
         move,$t4,$fp
         lw,$t3,-120($t4)
         add,$t4,$t2,$t3
         move,$t5,$fp
         sw,$t4,-124($t5)
                #               SUB,a,m,_t20
         li,$t2,97
         li,$t3,109
         sub,$t4,$t2,$t3
         move,$t5,$fp
         sw,$t4,-128($t5)
                #               DIV,_t19,_t20,_t21
         move,$t3,$fp
         lw,$t2,-124($t3)
         move,$t4,$fp
         lw,$t3,-128($t4)
         div,$t2,$t3
         mflo,$t4
         move,$t5,$fp
         sw,$t4,-132($t5)
                #               BEC,_t21,-,i3
         move,$t3,$fp
         lw,$t2,-132($t3)
         move,$t3,$fp
         sw,$t2,-36($t3)
                #       LABEL L12:
$L12:
                #               WRITE,-,-," The result is: "
         li,$v0,4
         la,$a0,_s6
         syscall
                #               WRITE,-,-,i3
         move,$t3,$fp
         lw,$t2,-36($t3)
         li,$v0,1
         move,$a0,$t2
         syscall
                #               ADD,i,1,i
         move,$t4,$fp
         lw,$t3,-44($t4)
         li,$t4,1
         add,$t5,$t3,$t4
         move,$t6,$fp
         sw,$t5,-44($t6)
                #               JMP,-,L0
         b,$L0
                #       LABEL L13:
$L13:
                #               END,-,-,-
         addi,$sp,$sp,132
