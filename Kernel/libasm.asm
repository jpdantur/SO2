global get_rax
global get_rbx
global get_rcx
global get_rdx
global set_rax
global get_seconds
global get_minutes
global get_hours

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

get_seconds:
	mov rax,0
	out 70h,al
	in ax,71h
	ret

get_minutes:
	mov rax,02h
	out 70h,al
	in ax,71h
	ret

get_hours:
	mov rax,04h
	out 70h,al
	in ax,71h
	ret