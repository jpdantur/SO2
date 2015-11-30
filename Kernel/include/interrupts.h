#ifndef INTERRUPTS_H
#define INTERRUPTS_H

void* sys_malloc(uint64 length);

void* get_sys_malloc();

void set_process_last_malloc(void* last);

void* get_process_malloc();

#endif