global get_rax
global get_rbx
global get_rcx
global get_rdx
global set_rax

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