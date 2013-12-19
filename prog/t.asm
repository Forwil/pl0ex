		.data
hello:	.space	1024
		.text
main:

		li  $t0,	2
		li	$t1,	4
		sle	$t2,	$t0,	$t1

		la	$a0,	hello
		li	$a1,	1024

		li	$v0,	8
		syscall

		li	$v0,	4
		syscall

		li	$v0,	10
		syscall
