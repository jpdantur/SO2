global _int80Handler
global _timerTick
global _keyboard
global _pageFaultHandler
extern int80
extern timerTick
extern PageFaultHandler
;extern keyboard_buffer_write
extern keyboard
extern switch_kernel_to_user
extern switch_user_to_kernel
global picMasterMask
global picSlaveMask
global _sti

%macro pushaq 0
    push rax      ;save current rax
    push rbx      ;save current rbx
    push rcx      ;save current rcx
    push rdx      ;save current rdx
    push rbp      ;save current rbp
    push rdi       ;save current rdi
    push rsi       ;save current rsi
    push r8        ;save current r8
    push r9        ;save current r9
    push r10      ;save current r10
    push r11      ;save current r11
    push r12      ;save current r12
    push r13      ;save current r13
    push r14      ;save current r14
    push r15      ;save current r15
    push fs
    push gs
%endmacro

%macro popaq 0
	pop gs
	pop fs
        pop r15
        pop r14
        pop r13
        pop r12
        pop r11
        pop r10
        pop r9
        pop r8
        pop rsi
        pop rdi
        pop rbp
        pop rdx
        pop rcx
        pop rbx
        pop rax
%endmacro


setParams:
	mov rdi,rcx
	mov rsi,rbx
	ret

_int80Handler:
	pushaq
	sti
;	cmp rax,9
;	je .a1
;	cmp rax,11
;	je .a1
.a2:
	call setParams
	call int80
	popaq
	iretq
;.a1:
;	mov rdi,rsp
;	call switch_user_to_kernel
;	mov rsp,rax
;	call setParams
;	call int80
;	call switch_kernel_to_user
;	mov rsp,rax
;	popaq
;	iretq

_timerTick:
	pushaq
	mov rdi, rsp
	call switch_user_to_kernel

	mov rsp, rax

	; schedule, get new process's RSP and load it
	call switch_kernel_to_user
	;xchg bx, bx
	cmp rax,0
	je .end
	mov rsp, rax
.end:
	;call timerTick
	
	mov al,0x20
	out 0x20,al

	popaq
	iretq

_keyboard:
    pushaq
    call keyboard
    mov al,0x20 ;EOI
    out 0x20,al ;
    popaq
    iretq

_pageFaultHandler:
    pushaq
    mov     eax, [rsp+136] ; error code
    mov     rdi, rax
    mov     rsi, cr2
    call    PageFaultHandler
    popaq
    add     esp, 8 ; error code
    iretq

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    ret

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    ret

_sti:
	sti
	ret
