global read_port
;global write_port
read_port:
	push rbp
	mov rbp,rsp

	mov rax, 0
	mov rdx, 0

	mov edx, edi
	in al, dx

	mov rsp,rbp
	pop rbp
	ret