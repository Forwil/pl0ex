        .data
_s1:    .ascii  "input x: "
_s2:    .ascii  "input y: "
_s3:    .ascii  "x mod y = "
_s4:    .ascii  "choice 1 2 3: "
        .text
         move,$fp,$sp
         sw,$fp,-4($sp)
         sw,$fp,-8($sp)
         b,main
                #       ENTER swap:
swap:
         addi,$sp,$sp,-20
                #               BEC,x,-,temp
         move,$t1,$fp
         lw,$t1,-8($t1)
         lw,$t0,-20($t1)
         move,$t1,$fp
         sw,$t0,-20($t1)
                #               BEC,y,-,x
         move,$t1,$fp
         lw,$t1,-8($t1)
         lw,$t0,-24($t1)
         move,$t1,$fp
         lw,$t1,-8($t1)
         sw,$t0,-20($t1)
                #               BEC,temp,-,y
         move,$t1,$fp
         lw,$t0,-20($t1)
         move,$t1,$fp
         lw,$t1,-8($t1)
         sw,$t0,-24($t1)
                #               END,-,-,swap
         move,$t1,$fp
         lw,$t0,-44($t1)
         move,$v0,$t0
         addi,$sp,$sp,20
         jr,$ra
                #       ENTER mod:
mod:
         addi,$sp,$sp,-36
                #               DIV,(fArg1),(fArg2),_t0
         move,$t2,$fp
         lw,$t1,-20($t2)
         lw,$t1,0($t1)
         move,$t3,$fp
         lw,$t2,-24($t3)
         lw,$t2,0($t2)
         div,$t1,$t2
         mflo,$t3
         move,$t4,$fp
         sw,$t3,-28($t4)
                #               MUL,_t0,(fArg2),_t1
         move,$t2,$fp
         lw,$t1,-28($t2)
         move,$t3,$fp
         lw,$t2,-24($t3)
         lw,$t2,0($t2)
         mult,$t1,$t2
         mflo,$t3
         move,$t4,$fp
         sw,$t3,-32($t4)
                #               SUB,(fArg1),_t1,_t2
         move,$t2,$fp
         lw,$t1,-20($t2)
         lw,$t1,0($t1)
         move,$t3,$fp
         lw,$t2,-32($t3)
         sub,$t3,$t1,$t2
         move,$t4,$fp
         sw,$t3,-36($t4)
                #               BEC,_t2,-,(fArg1)
         move,$t2,$fp
         lw,$t1,-36($t2)
         move,$t2,$fp
         lw,$t3,-20($t2)
         sw,$t1,0($t3)
                #               BEC,(fArg1),-,mod
         move,$t2,$fp
         lw,$t1,-20($t2)
         lw,$t1,0($t1)
         move,$t2,$fp
                #               END,-,-,mod
         move,$t2,$fp
         lw,$t1,-16($t2)
         move,$v0,$t1
         addi,$sp,$sp,36
         jr,$ra
                #       ENTER main:
main:
         addi,$sp,$sp,-60
                #               BEC,3,-,i
         li,$t2,3
         move,$t3,$fp
         sw,$t2,-36($t3)
                #       LABEL L0:
$L0:
                #               BIGE,i,1,_t3
         move,$t3,$fp
         lw,$t2,-36($t3)
         li,$t3,1
         sge,$t4,$t2,$t3
         move,$t5,$fp
         sw,$t4,-48($t5)
                #               JZ,_t3,L1
         move,$t3,$fp
         lw,$t2,-48($t3)
         beqz,$t2,$L1
                #               WRITE,-,-,"input x: "
         li,$v0,4
         la,$a0,_s1
         syscall
                #               READ,-,-,x
         li,$v0,5
         syscall
         move $t2,$v0
         move,$t3,$fp
         sw,$t2,-20($t3)
                #               WRITE,-,-,"input y: "
         li,$v0,4
         la,$a0,_s2
         syscall
                #               READ,-,-,y
         li,$v0,5
         syscall
         move $t2,$v0
         move,$t3,$fp
         sw,$t2,-24($t3)
                #               GETADD,x,-,_t4
         move,$t2,$fp
         addi,$t3,$t2,-20
         move,$t4,$fp
         sw,$t3,-52($t4)
                #               PUSH,_t4
         move,$t3,$fp
         lw,$t2,-52($t3)
         sw,$t2,-20($sp)
                #               GETADD,y,-,_t5
         move,$t2,$fp
         addi,$t3,$t2,-24
         move,$t4,$fp
         sw,$t3,-56($t4)
                #               PUSH,_t5
         move,$t3,$fp
         lw,$t2,-56($t3)
         sw,$t2,-24($sp)
                #               CALL,mod,2,_t6
         move,$t2,$fp
         lw,$t3,-8($t2)
         sw,$fp,-4($sp)
         sw,$t3,-8($sp)
         sw,$ra,-12($sp)
         sw,$v0,-16($sp)
         move,$fp,$sp
         jal,mod
         lw,$fp,-4($sp)
         lw,$ra,-12($sp)
         move,$t2,$fp
         sw,$v0,-60($t2)
         lw,$v0,-16($sp)
                #               BEC,_t6,-,x
         move,$t3,$fp
         lw,$t2,-60($t3)
         move,$t3,$fp
         sw,$t2,-20($t3)
                #               WRITE,-,-,"x mod y = "
         li,$v0,4
         la,$a0,_s3
         syscall
                #               WRITE,-,-,x
         move,$t3,$fp
         lw,$t2,-20($t3)
         li,$v0,1
         move,$a0,$t2
         syscall
                #               WRITE,-,-,"choice 1 2 3: "
         li,$v0,4
         la,$a0,_s4
         syscall
                #               READ,-,-,g
         li,$v0,5
         syscall
         move $t3,$v0
         move,$t4,$fp
         sw,$t3,-28($t4)
                #               ADD,i,-1,i
         move,$t4,$fp
         lw,$t3,-36($t4)
         li,$t4,-1
         add,$t5,$t3,$t4
         move,$t6,$fp
         sw,$t5,-36($t6)
                #               JMP,-,L0
         b,$L0
                #       LABEL L1:
$L1:
                #               END,-,-,-
         addi,$sp,$sp,60
