#include <time.h>
#include <lib.h>

char get_time(int type)
{
	char c;
	sys_time(&c, type);
	return htod(c);
}

void set_time(int hour, int min, int sec)
{
	printint(sec);
    set_time_att(0, dtoh(sec));
    set_time_att(1, dtoh(min));
    set_time_att(2, dtoh(hour));
}

void set_time_att(char type, char att)
{
    sys_time_write(type, att);
}