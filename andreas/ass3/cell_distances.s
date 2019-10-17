	.file	"cell_distances.c"
	.text
	.section	.rodata
.LC0:
	.string	"r"
.LC1:
	.string	"test_data/cell_e4"
.LC2:
	.string	"%d \n"
.LC4:
	.string	"%05.2f %lld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$248, %rsp
	.cfi_offset 3, -24
	movl	%edi, -244(%rbp)
	movq	%rsi, -256(%rbp)
	movq	-256(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -72(%rbp)
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	strlen
	subq	$1, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -80(%rbp)
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	strlen
	leaq	-2(%rax), %rdx
	movq	-72(%rbp), %rax
	leaq	2(%rax), %rcx
	movq	-80(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	strncpy
	movq	-80(%rbp), %rax
	movq	%rax, %rdi
	call	atoi
	movw	%ax, -82(%rbp)
	movl	$.LC0, %esi
	movl	$.LC1, %edi
	call	fopen
	movq	%rax, -96(%rbp)
	movzwl	-82(%rbp), %eax
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -104(%rbp)
	movw	$0, -18(%rbp)
	jmp	.L2
.L3:
	movzwl	-18(%rbp), %eax
	leaq	0(,%rax,8), %rdx
	movq	-104(%rbp), %rax
	leaq	(%rdx,%rax), %rbx
	movl	$4, %esi
	movl	$3465, %edi
	call	calloc
	movq	%rax, (%rbx)
	movzwl	-18(%rbp), %eax
	addl	$1, %eax
	movw	%ax, -18(%rbp)
.L2:
	movzwl	-18(%rbp), %eax
	cmpw	-82(%rbp), %ax
	jb	.L3
	movl	$0, -24(%rbp)
	movl	$0, -28(%rbp)
	movl	$30000, %edi
	call	malloc
	movq	%rax, -112(%rbp)
	movl	$30000, %edi
	call	malloc
	movq	%rax, -120(%rbp)
	movl	$6, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movl	$120000, %edi
	call	malloc
	movq	%rax, -128(%rbp)
	movl	$120000, %edi
	call	malloc
	movq	%rax, -136(%rbp)
	jmp	.L4
.L23:
	cmpq	$119999, -144(%rbp)
	ja	.L5
	movq	-144(%rbp), %rax
	movabsq	$-6148914691236517205, %rdx
	mulq	%rdx
	movq	%rdx, %rax
	shrq	$4, %rax
	jmp	.L6
.L5:
	movl	$5000, %eax
.L6:
	movw	%ax, -146(%rbp)
	call	omp_get_thread_num
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-104(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -160(%rbp)
	movw	$0, -30(%rbp)
	jmp	.L7
.L8:
	movzwl	-30(%rbp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movslq	%eax, %rdx
	movq	-128(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, %rdi
	call	toInt
	movw	%ax, -204(%rbp)
	movzwl	-30(%rbp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$3, %eax
	cltq
	leaq	8(%rax), %rdx
	movq	-128(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, %rdi
	call	toInt
	movw	%ax, -202(%rbp)
	movzwl	-30(%rbp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$3, %eax
	cltq
	leaq	16(%rax), %rdx
	movq	-128(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, %rdi
	call	toInt
	movw	%ax, -200(%rbp)
	movzwl	-30(%rbp), %edx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	addq	%rax, %rax
	movq	%rax, %rdx
	movq	-112(%rbp), %rax
	addq	%rdx, %rax
	movl	-204(%rbp), %edx
	movl	%edx, (%rax)
	movzwl	-200(%rbp), %edx
	movw	%dx, 4(%rax)
	movzwl	-30(%rbp), %eax
	addl	$1, %eax
	movw	%ax, -30(%rbp)
.L7:
	movzwl	-30(%rbp), %eax
	cmpw	-146(%rbp), %ax
	jb	.L8
	movw	$0, -32(%rbp)
	jmp	.L9
.L12:
	movzwl	-32(%rbp), %edx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	addq	%rax, %rax
	movq	%rax, %rdx
	movq	-112(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	%edx, -210(%rbp)
	movzwl	4(%rax), %eax
	movw	%ax, -206(%rbp)
	movzwl	-32(%rbp), %eax
	addl	$1, %eax
	movw	%ax, -34(%rbp)
	jmp	.L10
.L11:
	movzwl	-34(%rbp), %edx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	addq	%rax, %rax
	movq	%rax, %rdx
	movq	-120(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	%edx, -216(%rbp)
	movzwl	4(%rax), %eax
	movw	%ax, -212(%rbp)
	movzwl	-210(%rbp), %eax
	movswl	%ax, %edx
	movzwl	-216(%rbp), %eax
	cwtl
	movl	%edx, %ecx
	subl	%eax, %ecx
	movzwl	-210(%rbp), %eax
	movswl	%ax, %edx
	movzwl	-216(%rbp), %eax
	cwtl
	subl	%eax, %edx
	movl	%edx, %eax
	imull	%eax, %ecx
	movl	%ecx, %edx
	movzwl	-208(%rbp), %eax
	movswl	%ax, %ecx
	movzwl	-214(%rbp), %eax
	cwtl
	movl	%ecx, %esi
	subl	%eax, %esi
	movzwl	-208(%rbp), %eax
	movswl	%ax, %ecx
	movzwl	-214(%rbp), %eax
	cwtl
	subl	%eax, %ecx
	movl	%ecx, %eax
	imull	%esi, %eax
	leal	(%rdx,%rax), %ecx
	movzwl	-206(%rbp), %eax
	movswl	%ax, %edx
	movzwl	-212(%rbp), %eax
	cwtl
	movl	%edx, %esi
	subl	%eax, %esi
	movzwl	-206(%rbp), %eax
	movswl	%ax, %edx
	movzwl	-212(%rbp), %eax
	cwtl
	subl	%eax, %edx
	movl	%edx, %eax
	imull	%esi, %eax
	addl	%ecx, %eax
	movl	%eax, -196(%rbp)
	cvtsi2sdl	-196(%rbp), %xmm0
	movsd	.LC3(%rip), %xmm1
	divsd	%xmm1, %xmm0
	call	sqrt
	call	rint
	cvttsd2sil	%xmm0, %eax
	movw	%ax, -198(%rbp)
	movzwl	-198(%rbp), %eax
	leaq	0(,%rax,4), %rdx
	movq	-160(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	addl	$1, %edx
	movl	%edx, (%rax)
	movzwl	-34(%rbp), %eax
	addl	$1, %eax
	movw	%ax, -34(%rbp)
.L10:
	movzwl	-34(%rbp), %eax
	cmpw	-146(%rbp), %ax
	jb	.L11
	movzwl	-32(%rbp), %eax
	addl	$1, %eax
	movw	%ax, -32(%rbp)
.L9:
	movzwl	-32(%rbp), %eax
	movzwl	-146(%rbp), %edx
	subl	$1, %edx
	cmpl	%edx, %eax
	jl	.L12
	movl	-24(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -28(%rbp)
	jmp	.L13
.L22:
	cmpq	$119999, -168(%rbp)
	ja	.L14
	movq	-168(%rbp), %rax
	movabsq	$-6148914691236517205, %rdx
	mulq	%rdx
	movq	%rdx, %rax
	shrq	$4, %rax
	jmp	.L15
.L14:
	movl	$5000, %eax
.L15:
	movw	%ax, -170(%rbp)
	call	omp_get_thread_num
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-104(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -184(%rbp)
	movw	$0, -36(%rbp)
	jmp	.L16
.L17:
	movzwl	-36(%rbp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$3, %eax
	movslq	%eax, %rdx
	movq	-136(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, %rdi
	call	toInt
	movw	%ax, -222(%rbp)
	movzwl	-36(%rbp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$3, %eax
	cltq
	leaq	8(%rax), %rdx
	movq	-136(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, %rdi
	call	toInt
	movw	%ax, -220(%rbp)
	movzwl	-36(%rbp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$3, %eax
	cltq
	leaq	16(%rax), %rdx
	movq	-136(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, %rdi
	call	toInt
	movw	%ax, -218(%rbp)
	movzwl	-36(%rbp), %edx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	addq	%rax, %rax
	movq	%rax, %rdx
	movq	-120(%rbp), %rax
	addq	%rdx, %rax
	movl	-222(%rbp), %edx
	movl	%edx, (%rax)
	movzwl	-218(%rbp), %edx
	movw	%dx, 4(%rax)
	movzwl	-36(%rbp), %eax
	addl	$1, %eax
	movw	%ax, -36(%rbp)
.L16:
	movzwl	-36(%rbp), %eax
	cmpw	-170(%rbp), %ax
	jb	.L17
	movw	$0, -38(%rbp)
	jmp	.L18
.L21:
	movzwl	-38(%rbp), %edx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	addq	%rax, %rax
	movq	%rax, %rdx
	movq	-112(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	%edx, -228(%rbp)
	movzwl	4(%rax), %eax
	movw	%ax, -224(%rbp)
	movw	$0, -40(%rbp)
	jmp	.L19
.L20:
	movzwl	-40(%rbp), %edx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	addq	%rax, %rax
	movq	%rax, %rdx
	movq	-120(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	%edx, -234(%rbp)
	movzwl	4(%rax), %eax
	movw	%ax, -230(%rbp)
	movzwl	-228(%rbp), %eax
	movswl	%ax, %edx
	movzwl	-234(%rbp), %eax
	cwtl
	movl	%edx, %ecx
	subl	%eax, %ecx
	movzwl	-228(%rbp), %eax
	movswl	%ax, %edx
	movzwl	-234(%rbp), %eax
	cwtl
	subl	%eax, %edx
	movl	%edx, %eax
	imull	%eax, %ecx
	movl	%ecx, %edx
	movzwl	-226(%rbp), %eax
	movswl	%ax, %ecx
	movzwl	-232(%rbp), %eax
	cwtl
	movl	%ecx, %esi
	subl	%eax, %esi
	movzwl	-226(%rbp), %eax
	movswl	%ax, %ecx
	movzwl	-232(%rbp), %eax
	cwtl
	subl	%eax, %ecx
	movl	%ecx, %eax
	imull	%esi, %eax
	leal	(%rdx,%rax), %ecx
	movzwl	-224(%rbp), %eax
	movswl	%ax, %edx
	movzwl	-230(%rbp), %eax
	cwtl
	movl	%edx, %esi
	subl	%eax, %esi
	movzwl	-224(%rbp), %eax
	movswl	%ax, %edx
	movzwl	-230(%rbp), %eax
	cwtl
	subl	%eax, %edx
	movl	%edx, %eax
	imull	%esi, %eax
	addl	%ecx, %eax
	movl	%eax, -188(%rbp)
	cvtsi2sdl	-188(%rbp), %xmm0
	movsd	.LC3(%rip), %xmm1
	divsd	%xmm1, %xmm0
	call	sqrt
	call	rint
	cvttsd2sil	%xmm0, %eax
	movw	%ax, -190(%rbp)
	movzwl	-190(%rbp), %eax
	leaq	0(,%rax,4), %rdx
	movq	-184(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	addl	$1, %edx
	movl	%edx, (%rax)
	movzwl	-40(%rbp), %eax
	addl	$1, %eax
	movw	%ax, -40(%rbp)
.L19:
	movzwl	-40(%rbp), %eax
	cmpw	-170(%rbp), %ax
	jb	.L20
	movzwl	-38(%rbp), %eax
	addl	$1, %eax
	movw	%ax, -38(%rbp)
.L18:
	movzwl	-38(%rbp), %eax
	cmpw	-146(%rbp), %ax
	jb	.L21
	addl	$1, -28(%rbp)
	movl	-28(%rbp), %eax
	imull	$120000, %eax, %eax
	movl	%eax, %ecx
	movq	-96(%rbp), %rax
	movl	$0, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	fseek
.L13:
	movq	-96(%rbp), %rdx
	movq	-136(%rbp), %rax
	movq	%rdx, %rcx
	movl	$120000, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fread
	movq	%rax, -168(%rbp)
	cmpq	$0, -168(%rbp)
	jne	.L22
	addl	$1, -24(%rbp)
	movl	-24(%rbp), %eax
	imull	$120000, %eax, %eax
	movl	%eax, %ecx
	movq	-96(%rbp), %rax
	movl	$0, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	fseek
.L4:
	movq	-96(%rbp), %rdx
	movq	-128(%rbp), %rax
	movq	%rdx, %rcx
	movl	$120000, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fread
	movq	%rax, -144(%rbp)
	cmpq	$0, -144(%rbp)
	jne	.L23
	movq	-96(%rbp), %rax
	movq	%rax, %rdi
	call	fclose
	movw	$0, -42(%rbp)
	jmp	.L24
.L29:
	movq	$0, -56(%rbp)
	movw	$0, -58(%rbp)
	jmp	.L25
.L26:
	movzwl	-58(%rbp), %eax
	leaq	0(,%rax,8), %rdx
	movq	-104(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movzwl	-42(%rbp), %edx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, %eax
	addq	%rax, -56(%rbp)
	movzwl	-58(%rbp), %eax
	addl	$1, %eax
	movw	%ax, -58(%rbp)
.L25:
	movzwl	-58(%rbp), %eax
	cmpw	-82(%rbp), %ax
	jb	.L26
	cmpq	$0, -56(%rbp)
	je	.L31
	movzwl	-42(%rbp), %eax
	cvtsi2sdl	%eax, %xmm0
	movsd	.LC3(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movq	-56(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC4, %edi
	movl	$1, %eax
	call	printf
	jmp	.L28
.L31:
	nop
.L28:
	movzwl	-42(%rbp), %eax
	addl	$1, %eax
	movw	%ax, -42(%rbp)
.L24:
	cmpw	$3464, -42(%rbp)
	jbe	.L29
	movl	$0, %eax
	addq	$248, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.type	toInt, @function
toInt:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movw	$0, -2(%rbp)
	movq	-24(%rbp), %rax
	addq	$1, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	imulw	$10000, %ax, %dx
	movzwl	-2(%rbp), %eax
	addl	%edx, %eax
	movw	%ax, -2(%rbp)
	movq	-24(%rbp), %rax
	addq	$2, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	imulw	$1000, %ax, %dx
	movzwl	-2(%rbp), %eax
	addl	%edx, %eax
	movw	%ax, -2(%rbp)
	movq	-24(%rbp), %rax
	addq	$4, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	movl	%eax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	leal	0(,%rax,4), %edx
	addl	%edx, %eax
	sall	$2, %eax
	movl	%eax, %edx
	movzwl	-2(%rbp), %eax
	addl	%edx, %eax
	movw	%ax, -2(%rbp)
	movq	-24(%rbp), %rax
	addq	$5, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	movl	%eax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	movl	%eax, %edx
	movzwl	-2(%rbp), %eax
	addl	%edx, %eax
	movw	%ax, -2(%rbp)
	movq	-24(%rbp), %rax
	addq	$6, %rax
	movzbl	(%rax), %eax
	movsbw	%al, %dx
	movzwl	-2(%rbp), %eax
	addl	%edx, %eax
	subl	$48, %eax
	movw	%ax, -2(%rbp)
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	$44, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, %edx
	movzwl	-2(%rbp), %eax
	imull	%edx, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	toInt, .-toInt
	.section	.rodata
	.align 8
.LC3:
	.long	0
	.long	1079574528
	.ident	"GCC: (GNU) 9.1.1 20190503 (Red Hat 9.1.1-1)"
	.section	.note.GNU-stack,"",@progbits
