#ifndef _LIB_H
#define _LIB_H

#define MAX_NUMBER_PRINT_LEN 20

void print(char * str);
void putchar(char c);
char getchar();
int scan(char * str, int len);
char * itoa(int i, char b[], int len);
int pow(int x, int y);
int dtoh(int h);
int htod(int h);
int strcmp(const char * str1, const char * str2);
void *malloc();
void free(void* p);
int newproc(void * entry, char *name);
void fore();
void kill(int pid);
void list();

#endif