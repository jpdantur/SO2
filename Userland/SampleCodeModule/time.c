char time(int type)
{
	char c;
	sys_time(&c, type);
	return c;
}

void set_time(int hour, int min, int sec)
{
    set_time_att(0, dtoh(sec));
    set_time_att(1, dtoh(min));
    set_time_att(2, dtoh(hour));
}

void set_time_att(char type, char att)
{
    sys_time_write(type, att);
}