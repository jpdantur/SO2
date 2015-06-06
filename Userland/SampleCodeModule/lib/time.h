#ifndef _TIME_H
#define _TIME_H

#define SECONDS 0
#define MINUTES 1
#define HOUR 	2

char get_time(int type);
void set_time(int hour, int min, int sec);
void set_time_att(char type, char att);

#endif