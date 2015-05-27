global _sysCall
global _timerTick
global _keyboard
extern write
extern timerTick
extern keyboard

_sysCall:
	call write
	iretq

;_timerTick:
;	call timerTick
;	iretq

;_keyboard:
;	call keyboard
;	iretq
