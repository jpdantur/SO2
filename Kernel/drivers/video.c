#include "video.h"

char * video = VIDEO_START;

void video_write_byte(char c)
{
	*video = c;
	video_next_char();
}

void video_next_char()
{
	video += (char *)2;
}