global read_port
global get_rax
global get_rbx
global get_rcx
global get_rdx
global set_rax
global set_rbx
global set_rcx
global set_rdx

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



get_rax:
	ret

get_rbx:
	mov rax, rbx
	ret

get_rcx:
	mov rax, rcx
	ret

get_rdx:
	mov rax, rdx
	ret

set_rax:
	mov rax, rdi
	ret
