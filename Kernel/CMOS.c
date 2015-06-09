#include <CMOS.h>

char get_time(char type)
{
	char t;
	switch (type){
		case SECONDS:
			t = get_seconds();
			break;
		case MINUTES:
			t = get_minutes();
			break;
		case HOUR:
			t = get_hours();
			break;
	}
	return t;
}

void set_time(char time, char offset)
{
	char write_type;

	switch (offset)
	{
		case SECONDS:
			write_type = TIME__TYPE_SECONDS;
			break;
			
		case MINUTES:
			write_type = TIME__TYPE_MINUTES;
			break;

		case HOUR:
			write_type = TIME__TYPE_HOUR;
			break;

		case WEEKDAY:
			write_type = TIME__TYPE_WEEKDAY;
			break;

		case MONTHDAY:
			write_type = TIME__TYPE_MONTHDAY;
			break;

		case MONTH:
			write_type = TIME__TYPE_MONTH;
			break;

		case YEAR:
			write_type = TIME__TYPE_YEAR;
			break;
	}

	write_time(time, write_type);
}