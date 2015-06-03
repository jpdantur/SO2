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

setParams:
	mov rdi,rcx
	mov rsi,rbx
	ret

_int80Handler:
	sti
	call setParams
	call int80
	iretq

_timerTick:
	push rbp
	mov rbp,rsp
	push rax
	push rbx
	call timerTick
	
	mov al,0x20
	out 0x20,al
	pop rbx
	pop rax
	mov rsp,rbp
	pop rbp
	iretq

_keyboard:
	call keyboard
	mov al,0x20 ;EOI
	out 0x20,al ;
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