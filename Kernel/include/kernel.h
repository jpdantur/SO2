#ifndef KERNEL_H
#define KERNEL_H

#define INTERRUPTION_BIT (1<<9)


void* stackInit() ;
bool SetInterruptions(bool on);

#endif