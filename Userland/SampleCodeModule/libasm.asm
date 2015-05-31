global sys_write
global sys_read

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

sys_read:
	push rbp
	mov rbp,rsp
	push rbx

	mov rcx, rdi
	mov rax,3 ;syscall read
	int 80h

	pop rbx
	mov rsp,rbp
	pop rbp
	ret