global sys_write

sys_write:
	push rbp
	mov rbp,rsp
	push rbx
	mov rbx,1 ;STDOUT	
	mov rax,4 ;write
	mov rcx,rdi ;char
	mov rdx,1 ;len
	int 80h
	pop rbx
	mov rsp,rbp
	pop rbp
	ret
