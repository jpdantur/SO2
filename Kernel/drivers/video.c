#include <video.h>
#include <stdint.h>

char * video = VIDEO_START;
char * video_debug = VIDEO_LAST_LINE_START;

void video_write_byte(char c)
{
	if (video == VIDEO_END)
		video_scroll();
	
	*video = c;
	video_next_char();
}

void video_next_char()
{
	video += 2;
}

void video_clear_screen()
{
	int i;

	for (i = 0; i < VIDEO_HEIGHT * VIDEO_WIDTH; i++)
		video[i * 2] = 0;

	video = VIDEO_START;
}

void video_new_line()
{
	do
	{
		video_write_byte(0);
	}
	while((uint64_t)(video - VIDEO_START) % (VIDEO_WIDTH * 2) != 0);
}

void video_scroll()
{
	char * copy_from = VIDEO_START + VIDEO_WIDTH * 2;
	video = VIDEO_START;

	do
	{
		*video = *copy_from;
		video += 2;
		copy_from += 2;
	}
	while ((uint64_t)copy_from != VIDEO_END);

	video = VIDEO_START + (VIDEO_HEIGHT-1)*VIDEO_WIDTH*2;

	do
	{
		*video = 0;
		video += 2;
	}while(video != VIDEO_END);

	video = VIDEO_START + (VIDEO_WIDTH * (VIDEO_HEIGHT-1) * 2);


}
void __video_debug(char c)
{
	*video_debug = c;
	video_debug += 2;
	if (video_debug == VIDEO_END){
		video_debug = VIDEO_LAST_LINE_START;
		while (video_debug != VIDEO_END){
			*video_debug = 0;
			video_debug += 2;
		}
		video_debug = VIDEO_LAST_LINE_START;
	}

	
}