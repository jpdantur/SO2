#ifndef _CMOS_H
#define _CMOS_H

#define SECONDS 	0
#define MINUTES 	1
#define HOUR 		2
#define WEEKDAY 	3
#define MONTHDAY 	4
#define MONTH 		5
#define YEAR 		6

#define TIME__TYPE_SECONDS 	0x00
#define TIME__TYPE_MINUTES 	0x02
#define TIME__TYPE_HOUR 	0x04
#define TIME__TYPE_WEEKDAY 	0x06
#define TIME__TYPE_MONTHDAY 0x07
#define TIME__TYPE_MONTH 	0x08
#define TIME__TYPE_YEAR 	0x09

char get_hours();
char get_seconds();
char get_minutes();
void write_time(char time, char type);
void set_time(char time, char offset);

#endif