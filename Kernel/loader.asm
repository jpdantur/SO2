global loader
extern main
extern initializeKernelBinary

global 		WRITE_CR2
global 		READ_CR2
global 		WRITE_CR3
global 		READ_CR3
global		GET_FLAGS
global		TURN_ON_INTERRUPTS
global		TURN_OFF_INTERRUPTS
global		switch_u2k
global 		switch_k2u

loader:
	call initializeKernelBinary	; Set up the kernel binary, and get thet stack address
	mov rsp, rax				; Set up the stack with the returned address
	push rax
	call main
hang:
	hlt							; halt machine should kernel return
	jmp hang

WRITE_CR2:
		mov cr2, rdi
		ret

READ_CR2:
		mov rax, cr2
		ret

WRITE_CR3:
		mov cr3, rdi
		ret

READ_CR3:
		mov rax, cr3
		ret

GET_FLAGS:
		pushf
		pop 		rax
		ret

TURN_ON_INTERRUPTS:
		sti
		ret

TURN_OFF_INTERRUPTS:
		cli
		ret
switch_u2k:
		pop 		QWORD[ret_addr] 			;Direccion de retorno

		mov 		QWORD[task_stack], rsp
		mov 		rsp, [0x300000]

		push 		QWORD[ret_addr]
		ret


switch_k2u:
		pop 		QWORD[ret_addr] 			;Direccion de retorno

		mov 		[0x300000], rsp
		mov 		rsp, QWORD[task_stack]

		push 		QWORD[ret_addr]
		ret

ret_addr:
	resq 1
task_stack:
	resq 1

