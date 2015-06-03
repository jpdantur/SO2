global sys_write
global sys_read
global sys_time
global sys_time_write

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

sys_time:
	push rbp
	mov rbp,rsp
	push rbx

	mov rbx,rsi
	mov rcx, rdi
	mov rax,2 ;syscall time
	int 80h

	pop rbx
	mov rsp,rbp
	pop rbp
	ret

sys_time_write:
	push rax
	push rbx
	push rdx

	mov rax, 5
	mov rbx, rdi
	mov rdx, rsi

	int 80h

	pop rdx
	pop rbx
	pop rax
	ret