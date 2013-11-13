;====================Awaken's C0 Compiler====================
; 标号以@@name@scope的形式命名
; 函数以@scope的形式命名
; 字面值以$$value的形式命名
; 全局量以$name@global的形式命名
; 常量以$name@scope的形式命名
; 全局变量以$name@scope的形式命名

.386									;32位模式
.model flat, stdcall					;32位寻址, stdcall模式
option casemap :none					;大小写敏感

includelib msvcrt.lib	;vc运行时环境
printf PROTO C:ptr sbyte, :vararg
scanf PROTO C:ptr sbyte, :vararg

.data

$$printf_char db "%c", 0
$$printf_int db "%d", 0
$$printf_float db "%f", 0
$$new_line db 10, 13, 0
$$scanf_char db "%c", 0
$$scanf_int db "%d", 0
$$scanf_float db "%f", 0
<<<<<<< .mine
$$int$0 dd 0
$$int$1 dd 100
=======
$gc@global dd 'A'
$gi@global dd -9
$$float$22 dd 03f800000H
$gf@global dd 03f800000H
$$int$0 dd 0
$$int$1 dd 1
$$int$2 dd 2
$$int$3 dd 3
$$int$4 dd 4
$$int$5 dd 5
$$int$6 dd 6
$$int$7 dd 7
$$int$8 dd 8
$$int$9 dd 9
$$int$10 dd 10
$$string$11 db " ", 0
$gc@testcase02 dd 'x'
$gi@testcase02 dd 12
$gf@testcase02 dd 0c0833333H
$$char$16 dd 'X'
$$float$12 dd 03ff33333H
$$float$13 dd 03fc00000H
$$int$14 dd 40
$$int$15 dd 100
$$float$18 dd 04048f5c3H
$$int$17 dd 300
$$float$19 dd 042b7cccdH
$$float$20 dd 03e4ccccdH
$$float$21 dd 00H
$$float$23 dd 040000000H
$$float$24 dd 040800000H
$limit@testcase05 dd 03c23d70aH
$$float$25 dd 03f9d70a4H
$$float$28 dd 04019999aH
$$float$27 dd 0409d70a4H
$$float$26 dd 040570a3dH
$$float$29 dd 040400000H
$$float$30 dd 041000000H
$$float$31 dd 0461c4000H
$$float$32 dd 0466440cdH
$$int$33 dd 57
$$string$34 db "Test Case 01 - PASS", 0
$$string$35 db "Test Case 01 - FAILED:", 0
$$string$36 db "Test Case 02 - PASS", 0
$$string$37 db "Test Case 02 - FAILED:", 0
$$int$38 dd 5050
$$string$39 db "Test Case 03 - PASS", 0
$$string$40 db "Test Case 03 - FAILED:", 0
$$string$41 db "Test Case 04 - PASS", 0
$$string$42 db "Test Case 04 - FAILED:", 0
$$float$43 dd 0404851ecH
$$float$44 dd 04049999aH
$$string$45 db "Test Case 05 - PASS", 0
$$string$46 db "Test Case 05 - FAILED:", 0
$$int$47 dd 86
$$string$48 db "Test Case 06 - PASS", 0
$$string$49 db "Test Case 06 - FAILED:", 0
$$int$50 dd 467
$$string$51 db "Test Case 07 - PASS", 0
$$string$52 db "Test Case 07 - FAILED:", 0
$$string$53 db "Test Case 08 - PASS", 0
$$string$54 db "Test Case 08 - FAILED:", 0
$$int$55 dd 123
$$string$56 db "Test Case 09 - PASS", 0
$$string$57 db "Test Case 09 - FAILED:", 0
$$string$58 db "Test Case 10 - PASS", 0
$$string$59 db "Test Case 10 - FAILED:", 0
$$int$60 dd 20
$$string$61 db "Test Case 11 - PASS", 0
$$string$62 db "Test Case 11 - FAILED:", 0
>>>>>>> .r125

.data?

$i@global dd ?
$a0@global dd ?
$a1@global dd ?
$a2@global dd ?
$a3@global dd ?
$a4@global dd ?
$a5@global dd ?
$a6@global dd ?
$a7@global dd ?
$a8@global dd ?
$a9@global dd ?

.code

<<<<<<< .mine
=======
$get PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 0
	MOV EBX, dword ptr [esp] + 20
	CMP $$int$0, EBX
	JNE @@label0@get
	MOV EAX, dword ptr $a0@global
	JMP @@endof$get
@@label0@get:
	CMP $$int$1, EBX
	JNE @@label1@get
	MOV EAX, dword ptr $a1@global
	JMP @@endof$get
@@label1@get:
	CMP $$int$2, EBX
	JNE @@label2@get
	MOV EAX, dword ptr $a2@global
	JMP @@endof$get
@@label2@get:
	CMP $$int$3, EBX
	JNE @@label3@get
	MOV EAX, dword ptr $a3@global
	JMP @@endof$get
@@label3@get:
	CMP $$int$4, EBX
	JNE @@label4@get
	MOV EAX, dword ptr $a4@global
	JMP @@endof$get
@@label4@get:
	CMP $$int$5, EBX
	JNE @@label5@get
	MOV EAX, dword ptr $a5@global
	JMP @@endof$get
@@label5@get:
	CMP $$int$6, EBX
	JNE @@label6@get
	MOV EAX, dword ptr $a6@global
	JMP @@endof$get
@@label6@get:
	CMP $$int$7, EBX
	JNE @@label7@get
	MOV EAX, dword ptr $a7@global
	JMP @@endof$get
@@label7@get:
	CMP $$int$8, EBX
	JNE @@label8@get
	MOV EAX, dword ptr $a8@global
	JMP @@endof$get
@@label8@get:
	CMP $$int$9, EBX
	JNE @@label9@get
	MOV EAX, dword ptr $a9@global
	JMP @@endof$get
@@label9@get:
@@endof$get:
	ADD ESP, 0
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 4
$get ENDP

$set PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 0
	MOV EBP, dword ptr [esp] + 20
	MOV EBX, dword ptr [esp] + 24
	CMP $$int$0, EBP
	JNE @@label0@set
	MOV EAX, EBX
	MOV dword ptr $a0@global, EAX
@@label0@set:
	CMP $$int$1, EBP
	JNE @@label1@set
	MOV EAX, EBX
	MOV dword ptr $a1@global, EAX
@@label1@set:
	CMP $$int$2, EBP
	JNE @@label2@set
	MOV EAX, EBX
	MOV dword ptr $a2@global, EAX
@@label2@set:
	CMP $$int$3, EBP
	JNE @@label3@set
	MOV EAX, EBX
	MOV dword ptr $a3@global, EAX
@@label3@set:
	CMP $$int$4, EBP
	JNE @@label4@set
	MOV EAX, EBX
	MOV dword ptr $a4@global, EAX
@@label4@set:
	CMP $$int$5, EBP
	JNE @@label5@set
	MOV EAX, EBX
	MOV dword ptr $a5@global, EAX
@@label5@set:
	CMP $$int$6, EBP
	JNE @@label6@set
	MOV EAX, EBX
	MOV dword ptr $a6@global, EAX
@@label6@set:
	CMP $$int$7, EBP
	JNE @@label7@set
	MOV EAX, EBX
	MOV dword ptr $a7@global, EAX
@@label7@set:
	CMP $$int$8, EBP
	JNE @@label8@set
	MOV EAX, EBX
	MOV dword ptr $a8@global, EAX
@@label8@set:
	CMP $$int$9, EBP
	JNE @@label9@set
	MOV EAX, EBX
	MOV dword ptr $a9@global, EAX
@@label9@set:
@@endof$set:
	ADD ESP, 0
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 8
$set ENDP

$randomize PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 0
	PUSH $$int$3
	PUSH $$int$0
	CALL $set
	PUSH $$int$8
	PUSH $$int$1
	CALL $set
	PUSH $$int$4
	PUSH $$int$2
	CALL $set
	PUSH $$int$1
	PUSH $$int$3
	CALL $set
	PUSH $$int$6
	PUSH $$int$4
	CALL $set
	PUSH $$int$5
	PUSH $$int$5
	CALL $set
	PUSH $$int$0
	PUSH $$int$6
	CALL $set
	PUSH $$int$2
	PUSH $$int$7
	CALL $set
	PUSH $$int$9
	PUSH $$int$8
	CALL $set
	PUSH $$int$7
	PUSH $$int$9
	CALL $set
@@endof$randomize:
	ADD ESP, 0
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 0
$randomize ENDP

$print PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 0
	MOV EBP, $$int$0
@@label0@print:
	CMP EBP, $$int$10
	JGE @@label1@print
	PUSH offset $$string$11
	CALL printf
	ADD ESP, 4
	PUSH EBP
	CALL $get
	MOV EBX, EAX
	PUSH EBX
	PUSH offset $$printf_int
	CALL printf
	ADD ESP, 8
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
	ADD EBP, $$int$1
	JMP @@label0@print
@@label1@print:
@@endof$print:
	ADD ESP, 0
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 0
$print ENDP

$swap PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 0
	MOV EDI, dword ptr [esp] + 20
	MOV ESI, dword ptr [esp] + 24
	PUSH EDI
	CALL $get
	MOV EBP, EAX
	PUSH ESI
	CALL $get
	PUSH EAX
	PUSH EDI
	CALL $set
	PUSH EBP
	PUSH ESI
	CALL $set
@@endof$swap:
	ADD ESP, 0
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 8
$swap ENDP

$sort PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 0
	MOV EDI, $$int$0
@@label0@sort:
	CMP EDI, $$int$10
	JGE @@label1@sort
	MOV ESI, EDI
	ADD ESI, $$int$1
@@label2@sort:
	CMP ESI, $$int$10
	JGE @@label3@sort
	PUSH EDI
	CALL $get
	PUSH ESI
	MOV EBP, EAX
	CALL $get
	CMP EBP, EAX
	MOV EBX, EAX
	JLE @@label4@sort
	PUSH ESI
	PUSH EDI
	CALL $swap
@@label4@sort:
	ADD ESI, $$int$1
	JMP @@label2@sort
@@label3@sort:
	ADD EDI, $$int$1
	JMP @@label0@sort
@@label1@sort:
@@endof$sort:
	ADD ESP, 0
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 0
$sort ENDP

$qsort PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 0
	MOV EBX, dword ptr [esp] + 20
	ADD EBX, $$int$1
	CMP dword ptr [esp] + 24, EBX
	JL @@label0@qsort
	PUSH dword ptr [esp] + 20
	CALL $get
	MOV EDI, EAX
	MOV ESI, dword ptr [esp] + 20
	ADD ESI, $$int$1
	MOV EBP, dword ptr [esp] + 24
@@label1@qsort:
	CMP ESI, EBP
	JGE @@label2@qsort
	PUSH ESI
	CALL $get
	CMP EAX, EDI
	MOV EBX, EAX
	JGE @@label3@qsort
	ADD ESI, $$int$1
	JMP @@label4@qsort
@@label3@qsort:
	PUSH EBP
	PUSH ESI
	CALL $swap
	SUB EBP, $$int$1
@@label4@qsort:
	JMP @@label1@qsort
@@label2@qsort:
	PUSH ESI
	CALL $get
	CMP EAX, EDI
	MOV EBX, EAX
	JGE @@label5@qsort
	PUSH dword ptr [esp] + 20
	PUSH ESI
	CALL $swap
	PUSH ESI
	PUSH dword ptr [esp] + 24
	CALL $qsort
	PUSH dword ptr [esp] + 24
	PUSH EBP
	CALL $qsort
	JMP @@label6@qsort
@@label5@qsort:
	MOV EBX, dword ptr [esp] + 24
	SUB EBX, dword ptr [esp] + 20
	CMP EBX, $$int$1
	JNE @@label7@qsort
	JMP @@endof$qsort
@@label7@qsort:
	MOV EBX, ESI
	SUB EBX, $$int$1
	PUSH dword ptr [esp] + 20
	PUSH EBX
	CALL $swap
	PUSH EBX
	PUSH dword ptr [esp] + 24
	CALL $qsort
	PUSH dword ptr [esp] + 24
	PUSH EBP
	CALL $qsort
@@label6@qsort:
@@label0@qsort:
@@endof$qsort:
	ADD ESP, 0
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 8
$qsort ENDP

$testcase01 PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 8
	MOV ESI, dword ptr [esp] + 28
	MOV EBP, dword ptr [esp] + 32
	MOV EAX, $$int$6
	MOV EBX, ESI
	ADD EBX, EBP
	PUSH EBX
	FILD dword ptr [esp]
	ADD ESP, 4
	FSTP dword ptr [esp] + 4
	FLD dword ptr [esp] + 4
	FADD dword ptr [esp] + 36
	FSTP dword ptr [esp]
	FLD dword ptr [esp]
	SUB ESP, 4
	FISTP dword ptr [esp]
	POP EBX
	MOV dword ptr $i@global, EAX
	MOV EAX, EBX
@@endof$testcase01:
	ADD ESP, 8
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 12
$testcase01 ENDP

$testcase02 PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 124
	MOV EDI, dword ptr $gc@testcase02
	MOV EAX, $$int$1
	ADD EAX, $$int$2
	MOV ESI, dword ptr [esp] + 144
	ADD ESI, dword ptr [esp] + 148
	FLD dword ptr [esp] + 152
	FADD $$float$12
	FSTP dword ptr [esp] + 152
	FLD dword ptr [esp] + 152
	FMUL $$float$13
	FSTP dword ptr [esp] + 104
	FILD $$int$14
	FSTP dword ptr [esp] + 40
	FILD $$int$15
	FSTP dword ptr [esp] + 36
	MOV EBX, EDI
	ADD EBX, $$int$1
	MOV ECX, EBX
	SUB ECX, $$int$2
	MOV EDX, $$char$16
	MOV EBX, ECX
	SUB EBX, EDX
	MOV EBP, EBX
	ADD EBP, $$int$4
	MOV EBX, EBP
	SUB EBX, $$int$1
	MOV dword ptr [esp] + 68, EAX
	MOV EAX, EBX
	ADD EAX, EDI
	MOV EBP, EAX
	ADD EBP, dword ptr $gi@testcase02
	MOV EBX, EBP
	NEG EBX
	MOV ECX, $$int$2
	MOV dword ptr [esp] + 116, EDX
	MOV EDX, EBX
	IMUL EDX, dword ptr [esp] + 68
	MOV EAX, EDX
	MOV dword ptr [esp] + 64, EDX
	CDQ
	IDIV ECX
	MOV EDX, EAX
	ADD EDX, EBX
	MOV ECX, EDX
	ADD ECX, EBP
	MOV EBP, ECX
	ADD EBP, dword ptr [esp] + 148
	MOV EBX, EBP
	ADD EBX, ESI
	MOV EAX, EBX
	NEG EAX
	MOV EBX, EAX
	ADD EBX, dword ptr [esp] + 148
	PUSH EBX
	FILD dword ptr [esp]
	ADD ESP, 4
	FSTP dword ptr [esp] + 48
	FLD dword ptr [esp] + 48
	FDIV dword ptr [esp] + 104
	FSTP dword ptr [esp] + 100
	FLD dword ptr [esp] + 100
	FCHS
	FSTP dword ptr [esp] + 44
	FLD dword ptr [esp] + 44
	FADD dword ptr [esp] + 40
	FSTP dword ptr [esp] + 96
	FLD dword ptr [esp] + 96
	FMUL dword ptr [esp] + 36
	FSTP dword ptr [esp] + 92
	FILD dword ptr [esp] + 116
	FSTP dword ptr [esp] + 20
	PUSH ESI
	FILD dword ptr [esp]
	ADD ESP, 4
	FSTP dword ptr [esp] + 12
	FILD $$int$17
	FSTP dword ptr [esp] + 8
	MOV EBX, EDI
	ADD EBX, dword ptr $gi@testcase02
	PUSH EBX
	FILD dword ptr [esp]
	ADD ESP, 4
	FSTP dword ptr [esp] + 32
	FLD dword ptr [esp] + 32
	FSUB dword ptr $gf@testcase02
	FSTP dword ptr [esp] + 28
	FLD dword ptr [esp] + 28
	FMUL $$float$18
	FSTP dword ptr [esp] + 24
	FLD dword ptr [esp] + 24
	FSUB dword ptr [esp] + 20
	FSTP dword ptr [esp] + 16
	FLD dword ptr [esp] + 16
	FADD dword ptr [esp] + 12
	FSTP dword ptr [esp] + 88
	FLD dword ptr [esp] + 88
	FSUB dword ptr [esp] + 8
	FSTP dword ptr [esp] + 84
	FLD dword ptr [esp] + 84
	FCHS
	FSTP dword ptr [esp] + 4
	FLD dword ptr [esp] + 4
	FADD $$float$19
	FSTP dword ptr [esp] + 80
	FLD dword ptr [esp] + 80
	FCHS
	FSTP dword ptr [esp]
	FLD dword ptr [esp]
	FSUB $$float$20
	FSTP dword ptr [esp] + 76
	FLD dword ptr [esp] + 76
	SUB ESP, 4
	FISTP dword ptr [esp]
	POP EBX
	MOV EAX, EBX
@@endof$testcase02:
	ADD ESP, 124
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 12
$testcase02 ENDP

$testcase03 PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 0
	MOV EBP, $$int$1
	MOV EBX, EBP
	SUB EBX, EBX
@@label0@testcase03:
	CMP EBP, $$int$15
	JG @@label1@testcase03
	ADD EBX, EBP
	MOV EAX, $$int$1
	ADD EAX, EBP
	MOV EBP, EAX
	JMP @@label0@testcase03
@@label1@testcase03:
	MOV EAX, EBX
@@endof$testcase03:
	ADD ESP, 0
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 0
$testcase03 ENDP

$testcase04 PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 0
	MOV EBP, dword ptr [esp] + 20
	CMP EBP, $$int$1
	JNE @@label0@testcase04
	MOV EAX, $$int$2
	JMP @@endof$testcase04
	JMP @@label1@testcase04
@@label0@testcase04:
	CMP EBP, $$int$2
	JNE @@label2@testcase04
	MOV EBX, $$int$1
	NEG EBX
	MOV EAX, EBX
	JMP @@endof$testcase04
	JMP @@label3@testcase04
@@label2@testcase04:
	CMP EBP, $$int$3
	JNE @@label4@testcase04
	MOV EAX, $$int$1
	JMP @@endof$testcase04
	JMP @@label5@testcase04
@@label4@testcase04:
	MOV EBX, EBP
	SUB EBX, $$int$1
	PUSH EBX
	CALL $testcase04
	MOV EDI, EAX
	MOV EBX, EBP
	SUB EBX, $$int$2
	PUSH EBX
	CALL $testcase04
	MOV ESI, EDI
	ADD ESI, EAX
	MOV EBX, EBP
	SUB EBX, $$int$3
	PUSH EBX
	CALL $testcase04
	MOV EBX, ESI
	SUB EBX, EAX
	MOV EAX, EBX
	JMP @@endof$testcase04
@@label5@testcase04:
@@label3@testcase04:
@@label1@testcase04:
@@endof$testcase04:
	ADD ESP, 0
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 4
$testcase04 ENDP

$getpi PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 16
	FLD $$float$21
	FSTP dword ptr [esp] + 12
	FLD $$float$22
	FSTP dword ptr [esp] + 8
	MOV EBP, $$int$0
	MOV EBX, EBP
@@label0@getpi:
	CMP EBX, dword ptr [esp] + 36
	JGE @@label1@getpi
	FLD $$float$22
	FDIV dword ptr [esp] + 8
	FSTP dword ptr [esp] + 4
	FLD dword ptr [esp] + 8
	FADD $$float$23
	FSTP dword ptr [esp] + 8
	CMP EBP, $$int$0
	JNE @@label2@getpi
	MOV EBP, $$int$1
	JMP @@label3@getpi
@@label2@getpi:
	FLD dword ptr [esp] + 4
	FCHS
	FSTP dword ptr [esp] + 4
	MOV EBP, $$int$0
@@label3@getpi:
	FLD dword ptr [esp] + 12
	FADD dword ptr [esp] + 4
	FSTP dword ptr [esp] + 12
	ADD EBX, $$int$1
	JMP @@label0@getpi
@@label1@getpi:
	FLD dword ptr [esp] + 12
	FMUL $$float$24
	FSTP dword ptr [esp]
	MOV EAX, dword ptr [esp]
@@endof$getpi:
	ADD ESP, 16
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 4
$getpi ENDP

$testcase05 PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 20
	MOV EBP, $$int$0
	MOV EBX, EBP
@@label0@testcase05:
	CMP EBX, $$int$1
	JE @@label1@testcase05
	ADD EBP, $$int$2
	PUSH EBP
	CALL $getpi
	MOV dword ptr [esp] + 8, EAX
	FLD dword ptr [esp] + 8
	FSTP dword ptr [esp] + 12
	MOV ESI, EBP
	SUB ESI, $$int$1
	PUSH ESI
	CALL $getpi
	MOV dword ptr [esp] + 4, EAX
	FLD dword ptr [esp] + 4
	FSTP dword ptr [esp] + 16
	FLD dword ptr [esp] + 16
	FSUB dword ptr [esp] + 12
	FSTP dword ptr [esp]
	FLD dword ptr [esp]
	FLD dword ptr $limit@testcase05
	FCOMPP
	FNSTSW AX
	TEST AH, 65
	JNE @@label2@testcase05
	MOV EBX, $$int$1
@@label2@testcase05:
	JMP @@label0@testcase05
@@label1@testcase05:
	MOV EAX, dword ptr [esp] + 12
@@endof$testcase05:
	ADD ESP, 20
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 0
$testcase05 ENDP

$testcase06 PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 32
	MOV ESI, $$int$5
	MOV EDI, $$int$4
	NEG EDI
	MOV EAX, $$int$7
	SUB EAX, EDI
	MOV ECX, ESI
	IMUL ECX, $$int$2
	MOV dword ptr [esp] + 24, EAX
	MOV EAX, $$int$7
	CDQ
	IDIV EDI
	MOV EBX, ESI
	ADD EBX, ECX
	MOV EDX, EBX
	SUB EDX, EAX
	MOV ECX, $$int$1
	ADD ECX, $$int$7
	MOV EBX, $$int$3
	NEG EBX
	MOV EAX, $$int$7
	MOV dword ptr [esp] + 28, EAX
	MOV EAX, EBX
	MOV dword ptr [esp] + 12, EDX
	CDQ
	IDIV $$int$2
	MOV EDX, $$int$3
	IMUL EDX, $$int$8
	MOV EBP, ESI
	ADD EBP, EBX
	MOV EBX, EBP
	SUB EBX, $$int$1
	MOV EBP, EBX
	IMUL EBP, dword ptr [esp] + 24
	MOV dword ptr [esp] + 4, EAX
	MOV EAX, EBP
	MOV dword ptr [esp], EDX
	CDQ
	IDIV dword ptr [esp] + 12
	MOV EBP, EAX
	IMUL EBP, ECX
	MOV ESI, EBP
	IMUL ESI, dword ptr [esp] + 28
	MOV EBX, EBP
	IMUL EBX, $$int$4
	MOV EBP, EBX
	SUB EBP, dword ptr [esp] + 4
	MOV EBX, EBP
	ADD EBX, dword ptr [esp]
	MOV EBP, EBX
	ADD EBP, ESI
	MOV ESI, EBP
	SUB ESI, EDI
	MOV EBX, ESI
	IMUL EBX, $$int$3
	MOV EBP, EBX
	SUB EBP, $$int$2
	MOV EBX, EBP
	ADD EBX, $$int$1
	MOV EAX, EBX
@@endof$testcase06:
	ADD ESP, 32
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 0
$testcase06 ENDP

$testcase07 PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 96
	FLD $$float$25
	FSTP dword ptr [esp] + 92
	FLD $$float$26
	FCHS
	FSTP dword ptr [esp] + 72
	FLD $$float$27
	FSUB dword ptr [esp] + 72
	FSTP dword ptr [esp] + 60
	FLD dword ptr [esp] + 92
	FMUL $$float$23
	FSTP dword ptr [esp] + 52
	FLD $$float$27
	FDIV dword ptr [esp] + 72
	FSTP dword ptr [esp] + 44
	FLD dword ptr [esp] + 92
	FADD dword ptr [esp] + 52
	FSTP dword ptr [esp] + 48
	FLD dword ptr [esp] + 48
	FSUB dword ptr [esp] + 44
	FSTP dword ptr [esp] + 40
	FLD $$float$22
	FADD $$float$27
	FSTP dword ptr [esp] + 32
	FLD $$float$28
	FCHS
	FSTP dword ptr [esp] + 76
	FLD $$float$27
	FSTP dword ptr [esp] + 84
	FLD dword ptr [esp] + 76
	FDIV $$float$23
	FSTP dword ptr [esp] + 24
	FLD $$float$29
	FMUL $$float$30
	FSTP dword ptr [esp] + 16
	FLD dword ptr [esp] + 92
	FADD dword ptr [esp] + 76
	FSTP dword ptr [esp] + 68
	FLD dword ptr [esp] + 68
	FSUB $$float$22
	FSTP dword ptr [esp] + 64
	FLD dword ptr [esp] + 64
	FMUL dword ptr [esp] + 60
	FSTP dword ptr [esp] + 56
	FLD dword ptr [esp] + 56
	FDIV dword ptr [esp] + 40
	FSTP dword ptr [esp] + 36
	FLD dword ptr [esp] + 36
	FMUL dword ptr [esp] + 32
	FSTP dword ptr [esp] + 92
	FLD dword ptr [esp] + 92
	FMUL dword ptr [esp] + 84
	FSTP dword ptr [esp] + 12
	FLD dword ptr [esp] + 92
	FMUL $$float$24
	FSTP dword ptr [esp] + 28
	FLD dword ptr [esp] + 28
	FSUB dword ptr [esp] + 24
	FSTP dword ptr [esp] + 20
	FLD dword ptr [esp] + 20
	FADD dword ptr [esp] + 16
	FSTP dword ptr [esp] + 88
	FLD dword ptr [esp] + 88
	FADD dword ptr [esp] + 12
	FSTP dword ptr [esp] + 8
	FLD dword ptr [esp] + 8
	FSUB dword ptr [esp] + 72
	FSTP dword ptr [esp] + 84
	FLD dword ptr [esp] + 84
	FMUL $$float$29
	FSTP dword ptr [esp] + 4
	FLD dword ptr [esp] + 4
	FSUB $$float$23
	FSTP dword ptr [esp]
	FLD dword ptr [esp]
	FADD $$float$22
	FSTP dword ptr [esp] + 80
	FLD dword ptr [esp] + 80
	SUB ESP, 4
	FISTP dword ptr [esp]
	POP EBX
	MOV EAX, EBX
@@endof$testcase07:
	ADD ESP, 96
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 0
$testcase07 ENDP

$testcase08 PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 208
	MOV EAX, $$int$3
	FILD $$int$1
	FSTP dword ptr [esp] + 164
	FILD $$int$7
	FSTP dword ptr [esp] + 156
	FLD $$float$28
	FCHS
	FSTP dword ptr [esp] + 172
	FLD $$float$27
	FSTP dword ptr [esp] + 196
	FLD dword ptr [esp] + 172
	FMUL dword ptr [esp] + 156
	FSTP dword ptr [esp] + 152
	MOV ECX, $$int$2
	NEG ECX
	MOV EDX, $$int$7
	PUSH ECX
	FILD dword ptr [esp]
	ADD ESP, 4
	FSTP dword ptr [esp] + 140
	FLD $$float$26
	FCHS
	FSTP dword ptr [esp] + 168
	FLD dword ptr [esp] + 168
	FMUL dword ptr [esp] + 140
	FSTP dword ptr [esp] + 136
	FLD dword ptr [esp] + 196
	FSUB dword ptr [esp] + 136
	FSTP dword ptr [esp] + 132
	PUSH EAX
	FILD dword ptr [esp]
	ADD ESP, 4
	FSTP dword ptr [esp] + 124
	FLD $$float$25
	FMUL $$float$23
	FSTP dword ptr [esp] + 116
	FLD dword ptr [esp] + 196
	FDIV dword ptr [esp] + 168
	FSTP dword ptr [esp] + 108
	FLD $$float$25
	FADD dword ptr [esp] + 124
	FSTP dword ptr [esp] + 120
	FLD dword ptr [esp] + 120
	FADD dword ptr [esp] + 116
	FSTP dword ptr [esp] + 112
	FLD dword ptr [esp] + 112
	FSUB dword ptr [esp] + 108
	FSTP dword ptr [esp] + 104
	MOV dword ptr [esp] + 180, EDX
	MOV EDX, $$int$4
	NEG EDX
	FLD $$float$25
	FSTP dword ptr [esp] + 204
	PUSH EDX
	FILD dword ptr [esp]
	ADD ESP, 4
	FSTP dword ptr [esp] + 92
	FLD $$float$22
	FADD dword ptr [esp] + 196
	FSTP dword ptr [esp] + 96
	FLD dword ptr [esp] + 96
	FSUB dword ptr [esp] + 92
	FSTP dword ptr [esp] + 88
	FILD $$int$2
	FSTP dword ptr [esp] + 80
	FLD $$float$24
	FDIV dword ptr [esp] + 140
	FSTP dword ptr [esp] + 76
	FLD $$float$23
	FADD dword ptr [esp] + 124
	FSTP dword ptr [esp] + 68
	FLD dword ptr [esp] + 172
	FDIV dword ptr [esp] + 68
	FSTP dword ptr [esp] + 64
	FLD $$float$29
	FMUL $$float$30
	FSTP dword ptr [esp] + 56
	MOV ECX, dword ptr [esp] + 180
	IMUL ECX, EAX
	PUSH ECX
	FILD dword ptr [esp]
	ADD ESP, 4
	FSTP dword ptr [esp] + 40
	FLD dword ptr [esp] + 196
	FADD dword ptr [esp] + 92
	FSTP dword ptr [esp] + 28
	FLD dword ptr [esp] + 204
	FADD dword ptr [esp] + 164
	FSTP dword ptr [esp] + 160
	FLD dword ptr [esp] + 160
	FADD dword ptr [esp] + 152
	FSTP dword ptr [esp] + 148
	FLD dword ptr [esp] + 148
	FSUB $$float$22
	FSTP dword ptr [esp] + 144
	FLD dword ptr [esp] + 144
	FMUL dword ptr [esp] + 132
	FSTP dword ptr [esp] + 128
	FLD dword ptr [esp] + 128
	FDIV dword ptr [esp] + 104
	FSTP dword ptr [esp] + 100
	FLD dword ptr [esp] + 100
	FMUL dword ptr [esp] + 88
	FSTP dword ptr [esp] + 84
	FLD dword ptr [esp] + 84
	FDIV dword ptr [esp] + 80
	FSTP dword ptr [esp] + 204
	FLD dword ptr [esp] + 204
	FMUL dword ptr [esp] + 156
	FSTP dword ptr [esp] + 32
	FLD dword ptr [esp] + 32
	FMUL dword ptr [esp] + 28
	FSTP dword ptr [esp] + 24
	FLD $$float$31
	FDIV dword ptr [esp] + 140
	FSTP dword ptr [esp] + 12
	FLD dword ptr [esp] + 204
	FMUL dword ptr [esp] + 76
	FSTP dword ptr [esp] + 72
	FLD dword ptr [esp] + 72
	FSUB dword ptr [esp] + 64
	FSTP dword ptr [esp] + 60
	FLD dword ptr [esp] + 60
	FADD dword ptr [esp] + 56
	FSTP dword ptr [esp] + 52
	FLD dword ptr [esp] + 52
	FSUB dword ptr [esp] + 92
	FSTP dword ptr [esp] + 48
	FLD dword ptr [esp] + 48
	FADD dword ptr [esp] + 40
	FSTP dword ptr [esp] + 200
	FLD dword ptr [esp] + 200
	FADD dword ptr [esp] + 124
	FSTP dword ptr [esp] + 36
	FLD dword ptr [esp] + 36
	FADD dword ptr [esp] + 24
	FSTP dword ptr [esp] + 20
	FLD dword ptr [esp] + 20
	FSUB dword ptr [esp] + 168
	FSTP dword ptr [esp] + 16
	FLD dword ptr [esp] + 16
	FADD dword ptr [esp] + 12
	FSTP dword ptr [esp] + 196
	FLD dword ptr [esp] + 196
	FMUL dword ptr [esp] + 124
	FSTP dword ptr [esp] + 8
	FLD dword ptr [esp] + 8
	FSUB dword ptr [esp] + 80
	FSTP dword ptr [esp] + 4
	FLD dword ptr [esp] + 4
	FADD $$float$22
	FSTP dword ptr [esp] + 192
	FLD dword ptr [esp] + 192
	FADD $$float$32
	FSTP dword ptr [esp]
	FLD dword ptr [esp]
	SUB ESP, 4
	FISTP dword ptr [esp]
	POP EBX
	MOV EAX, EBX
@@endof$testcase08:
	ADD ESP, 208
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 0
$testcase08 ENDP

$testcase09 PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 0
	MOV EDI, dword ptr [esp] + 20
	MOV ESI, $$int$1
	MOV EBP, $$int$2
	CMP ESI, EBP
	JNE @@label0@testcase09
	MOV EDI, $$int$0
	JMP @@label1@testcase09
@@label0@testcase09:
@@label1@testcase09:
	MOV EBX, EBP
	SUB EBX, $$int$1
	CMP ESI, EBX
	JNE @@label2@testcase09
	JMP @@label3@testcase09
@@label2@testcase09:
	MOV EDI, $$int$0
@@label3@testcase09:
	CMP ESI, EBP
	JLE @@label4@testcase09
	MOV EDI, $$int$0
	JMP @@label5@testcase09
@@label4@testcase09:
@@label5@testcase09:
	CMP ESI, EBP
	JL @@label6@testcase09
	MOV EDI, $$int$0
	JMP @@label7@testcase09
@@label6@testcase09:
@@label7@testcase09:
	MOV EBX, EBP
	SUB EBX, $$int$1
	CMP ESI, EBX
	JL @@label8@testcase09
	JMP @@label9@testcase09
@@label8@testcase09:
	MOV EDI, $$int$0
@@label9@testcase09:
	CMP ESI, EBP
	JGE @@label10@testcase09
	JMP @@label11@testcase09
@@label10@testcase09:
	MOV EDI, $$int$0
@@label11@testcase09:
	CMP ESI, EBP
	JG @@label12@testcase09
	JMP @@label13@testcase09
@@label12@testcase09:
	MOV EDI, $$int$0
@@label13@testcase09:
	MOV EBX, $$int$1
	ADD EBX, ESI
	CMP EBX, EBP
	JG @@label14@testcase09
	JMP @@label15@testcase09
@@label14@testcase09:
	MOV EDI, $$int$0
@@label15@testcase09:
	MOV EAX, EDI
@@endof$testcase09:
	ADD ESP, 0
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 4
$testcase09 ENDP

>>>>>>> .r125
$testcase10 PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 24
	FILD $$int$1
	FSTP dword ptr [esp] + 20
	FILD $$int$2
	FSTP dword ptr [esp] + 16
	FLD dword ptr [esp] + 20
	FLD dword ptr [esp] + 16
	FCOMPP
	FNSTSW AX
	TEST AH, 5
	JP @@label0@testcase10
	MOV EAX, $$int$0
	MOV dword ptr [esp] + 44, EAX
	JMP @@label1@testcase10
@@label0@testcase10:
@@label1@testcase10:
	FLD dword ptr [esp] + 20
	FLD dword ptr [esp] + 16
	FCOMPP
	FNSTSW AX
	TEST AH, 65
	JP @@label2@testcase10
	MOV EAX, $$int$0
	MOV dword ptr [esp] + 44, EAX
	JMP @@label3@testcase10
@@label2@testcase10:
@@label3@testcase10:
	FILD $$int$1
	FSTP dword ptr [esp] + 12
	FLD dword ptr [esp] + 16
	FSUB dword ptr [esp] + 12
	FSTP dword ptr [esp] + 8
	FLD dword ptr [esp] + 20
	FLD dword ptr [esp] + 8
	FCOMPP
	FNSTSW AX
	TEST AH, 65
	JP @@label4@testcase10
	JMP @@label5@testcase10
@@label4@testcase10:
	MOV EAX, $$int$0
	MOV dword ptr [esp] + 44, EAX
@@label5@testcase10:
	FLD dword ptr [esp] + 20
	FLD dword ptr [esp] + 16
	FCOMPP
	FNSTSW AX
	TEST AH, 65
	JNE @@label6@testcase10
	JMP @@label7@testcase10
@@label6@testcase10:
	MOV EAX, $$int$0
	MOV dword ptr [esp] + 44, EAX
@@label7@testcase10:
	FLD dword ptr [esp] + 20
	FLD dword ptr [esp] + 16
	FCOMPP
	FNSTSW AX
	TEST AH, 1
	JNE @@label8@testcase10
	JMP @@label9@testcase10
@@label8@testcase10:
	MOV EAX, $$int$0
	MOV dword ptr [esp] + 44, EAX
@@label9@testcase10:
	FILD $$int$1
	FSTP dword ptr [esp] + 4
	FLD dword ptr [esp] + 4
	FADD dword ptr [esp] + 20
	FSTP dword ptr [esp]
	FLD dword ptr [esp]
	FLD dword ptr [esp] + 16
	FCOMPP
	FNSTSW AX
	TEST AH, 1
	JNE @@label10@testcase10
	JMP @@label11@testcase10
@@label10@testcase10:
	MOV EAX, $$int$0
	MOV dword ptr [esp] + 44, EAX
@@label11@testcase10:
	MOV EAX, dword ptr [esp] + 44
@@endof$testcase10:
	ADD ESP, 24
	POP EDI
	POP ESI
	POP EBP
	POP EBX
	RET 4
$testcase10 ENDP

$testcase11 PROC
	PUSH EBX
	PUSH EBP
	PUSH ESI
	PUSH EDI
	SUB ESP, 0
<<<<<<< .mine
	MOV EBP, $$int$0
@@label0@main:
	CMP EBP, $$int$1
	JGE @@label1@main
	SUB ESP, 4
	PUSH ESP
	PUSH offset $$scanf_int
	CALL scanf
	ADD ESP, 8
=======
	MOV EBP, $$int$0
	MOV ESI, EBP
	CALL $randomize
	CALL $sort
@@label0@testcase11:
	CMP EBP, $$int$10
	JGE @@label1@testcase11
	PUSH EBP
	CALL $get
	CMP EBP, EAX
	MOV EBX, EAX
	JNE @@label2@testcase11
	ADD ESI, $$int$1
@@label2@testcase11:
	ADD EBP, $$int$1
	JMP @@label0@testcase11
@@label1@testcase11:
	CALL $randomize
	PUSH $$int$9
	PUSH $$int$0
	CALL $qsort
	MOV EBP, $$int$0
@@label3@testcase11:
	CMP EBP, $$int$10
	JGE @@label4@testcase11
	PUSH EBP
	CALL $get
	CMP EBP, EAX
	MOV EBX, EAX
	JNE @@label5@testcase11
	ADD ESI, $$int$1
@@label5@testcase11:
	ADD EBP, $$int$1
	JMP @@label3@testcase11
@@label4@testcase11:
	MOV EAX, ESI
@@endof$testcase11:
	ADD ESP, 0
	POP EDI
	POP ESI
	POP EBP
>>>>>>> .r125
	POP EBX
<<<<<<< .mine
	ADD EBP, EBX
	JMP @@label0@main
@@label1@main:
	PUSH EBP
	PUSH offset $$printf_int
	CALL printf
	ADD ESP, 8
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
=======
	RET 0
$testcase11 ENDP

$main PROC
	SUB ESP, 8
	PUSH dword ptr $gf@global
	PUSH dword ptr $gi@global
	PUSH dword ptr $gc@global
	CALL $testcase01
	CMP EAX, $$int$33
	MOV EBX, EAX
	JNE @@label0@main
	PUSH offset $$string$34
	CALL printf
	ADD ESP, 4
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
	JMP @@label1@main
@@label0@main:
	PUSH offset $$string$35
	CALL printf
	ADD ESP, 4
	PUSH dword ptr $gf@global
	PUSH dword ptr $gi@global
	PUSH dword ptr $gc@global
	CALL $testcase01
	MOV EBX, EAX
	PUSH EBX
	PUSH offset $$printf_int
	CALL printf
	ADD ESP, 8
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
@@label1@main:
	PUSH dword ptr $gf@global
	PUSH dword ptr $gi@global
	PUSH dword ptr $gc@global
	CALL $testcase02
	CMP EAX, $$int$3
	MOV EBX, EAX
	JNE @@label2@main
	PUSH offset $$string$36
	CALL printf
	ADD ESP, 4
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
	JMP @@label3@main
@@label2@main:
	PUSH offset $$string$37
	CALL printf
	ADD ESP, 4
	PUSH dword ptr $gf@global
	PUSH dword ptr $gi@global
	PUSH dword ptr $gc@global
	CALL $testcase02
	MOV EBX, EAX
	PUSH EBX
	PUSH offset $$printf_int
	CALL printf
	ADD ESP, 8
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
@@label3@main:
	CALL $testcase03
	CMP EAX, $$int$38
	MOV EBX, EAX
	JNE @@label4@main
	PUSH offset $$string$39
	CALL printf
	ADD ESP, 4
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
	JMP @@label5@main
@@label4@main:
	PUSH offset $$string$40
	CALL printf
	ADD ESP, 4
	CALL $testcase03
	MOV EBX, EAX
	PUSH EBX
	PUSH offset $$printf_int
	CALL printf
	ADD ESP, 8
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
@@label5@main:
	PUSH dword ptr $i@global
	CALL $testcase04
	MOV EBX, EAX
	NEG EBX
	CMP EBX, $$int$3
	JNE @@label6@main
	PUSH offset $$string$41
	CALL printf
	ADD ESP, 4
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
	JMP @@label7@main
@@label6@main:
	PUSH offset $$string$42
	CALL printf
	ADD ESP, 4
	PUSH dword ptr $i@global
	CALL $testcase04
	MOV EBX, EAX
	PUSH EBX
	PUSH offset $$printf_int
	CALL printf
	ADD ESP, 8
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
@@label7@main:
	CALL $testcase05
	MOV dword ptr [esp], EAX
	FLD dword ptr [esp]
	FSTP dword ptr [esp] + 4
	FLD dword ptr [esp] + 4
	FLD $$float$43
	FCOMPP
	FNSTSW AX
	TEST AH, 65
	JP @@label8@main
	FLD dword ptr [esp] + 4
	FLD $$float$44
	FCOMPP
	FNSTSW AX
	TEST AH, 1
	JNE @@label9@main
	PUSH offset $$string$45
	CALL printf
	ADD ESP, 4
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
	JMP @@label10@main
@@label9@main:
	PUSH offset $$string$46
	CALL printf
	ADD ESP, 4
	SUB ESP, 8
	FLD dword ptr [esp] + 12
	FSTP qword ptr [esp]
	PUSH offset $$printf_float
	CALL printf
	ADD ESP, 12
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
@@label10@main:
	JMP @@label11@main
@@label8@main:
	PUSH offset $$string$46
	CALL printf
	ADD ESP, 4
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
@@label11@main:
	CALL $testcase06
	CMP $$int$47, EAX
	MOV EBX, EAX
	JNE @@label12@main
	PUSH offset $$string$48
	CALL printf
	ADD ESP, 4
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
	JMP @@label13@main
@@label12@main:
	PUSH offset $$string$49
	CALL printf
	ADD ESP, 4
	CALL $testcase06
	MOV EBX, EAX
	PUSH EBX
	PUSH offset $$printf_int
	CALL printf
	ADD ESP, 8
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
@@label13@main:
	CALL $testcase07
	MOV EBX, EAX
	NEG EBX
	CMP $$int$50, EBX
	JNE @@label14@main
	PUSH offset $$string$51
	CALL printf
	ADD ESP, 4
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
	JMP @@label15@main
@@label14@main:
	PUSH offset $$string$52
	CALL printf
	ADD ESP, 4
	CALL $testcase07
	MOV EBX, EAX
	NEG EBX
	PUSH EBX
	PUSH offset $$printf_int
	CALL printf
	ADD ESP, 8
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
@@label15@main:
	CALL $testcase08
	CMP $$int$1, EAX
	MOV EBX, EAX
	JNE @@label16@main
	PUSH offset $$string$53
	CALL printf
	ADD ESP, 4
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
	JMP @@label17@main
@@label16@main:
	PUSH offset $$string$54
	CALL printf
	ADD ESP, 4
	CALL $testcase08
	MOV EBX, EAX
	PUSH EBX
	PUSH offset $$printf_int
	CALL printf
	ADD ESP, 8
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
@@label17@main:
	PUSH $$int$55
	CALL $testcase09
	CMP $$int$55, EAX
	MOV EBX, EAX
	JNE @@label18@main
	PUSH offset $$string$56
	CALL printf
	ADD ESP, 4
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
	JMP @@label19@main
@@label18@main:
	PUSH offset $$string$57
	CALL printf
	ADD ESP, 4
	PUSH $$int$55
	CALL $testcase09
	MOV EBX, EAX
	PUSH EBX
	PUSH offset $$printf_int
	CALL printf
	ADD ESP, 8
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
@@label19@main:
	PUSH $$int$55
	CALL $testcase10
	CMP $$int$55, EAX
	MOV EBX, EAX
	JNE @@label20@main
	PUSH offset $$string$58
	CALL printf
	ADD ESP, 4
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
	JMP @@label21@main
@@label20@main:
	PUSH offset $$string$59
	CALL printf
	ADD ESP, 4
	PUSH $$int$55
	CALL $testcase10
	MOV EBX, EAX
	PUSH EBX
	PUSH offset $$printf_int
	CALL printf
	ADD ESP, 8
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
@@label21@main:
	CALL $testcase11
	CMP $$int$60, EAX
	MOV EBX, EAX
	JNE @@label22@main
	PUSH offset $$string$61
	CALL printf
	ADD ESP, 4
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
	JMP @@label23@main
@@label22@main:
	PUSH offset $$string$62
	CALL printf
	ADD ESP, 4
	CALL $testcase11
	MOV EBX, EAX
	PUSH EBX
	PUSH offset $$printf_int
	CALL printf
	ADD ESP, 8
	PUSH offset $$new_line
	CALL printf
	ADD ESP, 4
@@label23@main:
>>>>>>> .r125
@@endof$main:
	ADD ESP, 8
	RET 0
$main ENDP
END $main
