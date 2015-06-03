#include <stdint.h>

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

//TODO: Timefile
char get_time(char type)
{
	char t;
	switch (type){
		case 0:
			t = get_seconds();
			break;
		case 1:
			t = get_minutes();
			break;
		case 2:
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

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}
