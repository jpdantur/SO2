global _int80Handler
global _timerTick
global _keyboard
extern int80
extern timerTick
extern keyboard
global picMasterMask
global picSlaveMask
global _sti

_int80Handler:
	call int80
	iretq

_timerTick:
	call timerTick
	mov al,0x20
	out 0x20,al
	iretq

_keyboard:
	call keyboard
	mov al,0x20
	out 0x20,al
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