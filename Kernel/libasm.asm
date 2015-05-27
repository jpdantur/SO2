global get_call

global get_rax
global get_rbx
global get_rcx
global get_rdx

get_call:
	mov rax, rcx
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

