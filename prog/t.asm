		.data
hello:	.space	1024
		.text
main:
		la	$a0,	hello
		li	$a1,	1024

		li	$v0,	8
		syscall

		li	$v0,	4
		syscall

		li	$v0,	10
		syscall
