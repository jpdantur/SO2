global _int80Handler
global _timerTick
global _keyboard
extern int80
extern timerTick
extern keyboard

_int80Handler:
	call int80
	iretq

;_timerTick:
;	call timerTick
;	iretq

;_keyboard:
;	call keyboard
;	iretq
