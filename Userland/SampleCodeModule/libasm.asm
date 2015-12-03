global sys_write
global sys_read
global sys_time
global sys_time_write
global sys_screen_saver_set
global sys_malloc
global sys_free
global sys_newproc
global sys_fore
global sys_kill
global sys_list
global sys_getpid
global sys_getppid
global sys_sem_up
global sys_sem_down
global sys_get_mem
global sys_sleep


sys_sleep:
	push rbp
	mov rbp,rsp
	push rax
	push rdx

	mov rax, 18 ; sys_call_sleep
	mov rdx, rdi
	mov rcx, rsi
	int 80h

	pop rdx
	pop rax
	mov rsp,rbp
	pop rbp
	ret

sys_get_mem:
	push rbp
	mov rbp,rsp
	push rbx
	
	mov rax,17 ;get_mem
	mov rcx,rdi
	int 80h
	
	pop rbx
	mov rsp,rbp
	pop rbp
	ret
sys_sem_down:
	push rbp
	mov rbp,rsp
	push rbx
	
	mov rax,16 ;sem_down
	int 80h
	
	pop rbx
	mov rsp,rbp
	pop rbp
	ret
sys_sem_up:
	push rbp
	mov rbp,rsp
	push rbx
	
	mov rax,15 ;sem_up
	int 80h
	
	pop rbx
	mov rsp,rbp
	pop rbp
	ret
sys_getppid:
	push rbp
	mov rbp,rsp
	push rbx
	
	mov rax,14 ;sys_getppid
	mov rcx,rdi
	int 80h
	
	pop rbx
	mov rsp,rbp
	pop rbp
	ret
sys_getpid:
	push rbp
	mov rbp,rsp
	push rbx
	
	mov rax,13 ;sys_getpid
	mov rcx,rdi
	int 80h
	
	pop rbx
	mov rsp,rbp
	pop rbp
	ret
sys_write:
	push rbp
	mov rbp,rsp
	push rbx

	mov rbx,1 	; STDOUT	
	mov rax,4 	; sys_call write
	mov rcx,rdi ; char
	mov rdx,rsi ; len
	int 80h

	pop rbx
	mov rsp,rbp
	pop rbp
	ret

sys_read:
	push rbp
	mov rbp,rsp
	push rbx

	mov rcx, rdi
	mov rdx,rsi ; len
	mov rax,3 	; sys_call read
	int 80h

	pop rbx
	mov rsp,rbp
	pop rbp
	ret

sys_time:
	push rbp
	mov rbp,rsp
	push rbx

	mov rbx,rsi
	mov rcx, rdi
	mov rax,2 	; sys_call get_time
	int 80h

	pop rbx
	mov rsp,rbp
	pop rbp
	ret

sys_time_write:
	push rax
	push rbx
	push rdx

	mov rax, 5	; sys_call write_time 
	mov rbx, rdi
	mov rdx, rsi
	int 80h

	pop rdx
	pop rbx
	pop rax
	ret

sys_screen_saver_set:
	push rax
	push rdx

	mov rax, 6 ; sys_call screen_saver_set
	mov rdx, rdi
	int 80h

	pop rdx
	pop rax
	ret

sys_malloc:
	push rbp
	mov rbp,rsp
	push rbx
	
	mov rax,7 ;malloc sys_call
	mov rcx,rdi
	int 80h
	
	pop rbx
	mov rsp,rbp
	pop rbp
	ret

sys_free:
	push rbp
	mov rbp,rsp
	push rbx
	
	mov rax,8 ;free sys_call
	mov rcx,rdi
	int 80h
	
	pop rbx
	mov rsp,rbp
	pop rbp
	ret
sys_newproc:
	push rbp
	mov rbp,rsp
	push rbx
	
	mov rax,9 ;new process
	mov rbx,rdx
	mov rcx,rdi
	mov rdx,rsi
	int 80h
	
	pop rbx
	mov rsp,rbp
	pop rbp
	ret
sys_fore:
	push rbp
	mov rbp,rsp
	push rbx
	
	mov rax,10 ;fore
	mov rcx,rdi
	int 80h
	
	pop rbx
	mov rsp,rbp
	pop rbp
	ret
sys_kill:
	push rbp
	mov rbp, rsp
	push rbx

	mov rax,11 ;kill
	mov rcx,rdi
	int 80h

	pop rbx
	mov rsp,rbp
	pop rbp
	ret
sys_list:
	push rbp
	mov rbp,rsp
	push rbx
	
	mov rax,12 ;list
	int 80h
	
	pop rbx
	mov rsp,rbp
	pop rbp
	ret
