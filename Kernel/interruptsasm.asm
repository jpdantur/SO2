global _int80Handler
global _timerTick
global _keyboard
extern int80
extern timerTick
;extern keyboard_buffer_write
extern keyboard
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
%endmacro

%macro popaq 0
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
	call setParams
	call int80
	popaq
	iretq

_timerTick:
	pushaq
	call timerTick
	
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