;;
;;	Time
;;
global get_seconds
global get_minutes
global get_hours
global write_time

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

; void set_time(time, type)
write_time:
	push rax

	mov rax, rsi ;Set the attribute of the time to write
	out 70h, al
	
	mov rax, rdi ;write the value
	out 71h, al

	pop rax
	ret
