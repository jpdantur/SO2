#include <video.h>
#include <stdint.h>

char * video = VIDEO_START;
char * video_debug = VIDEO_LAST_LINE_START;
int video_bff_counter = 0;
char backup[VIDEO_HEIGHT*VIDEO_WIDTH*2];

char screen_saver_active = 0;
int screen_saver_counter = 0;
int screen_saver_drawer = 0;

void backup_screen()
{
	int i;
	for (i = 0; i < VIDEO_SIZE; i++)
	{
		backup[i] = *(VIDEO_START + i);
	}
}

void restore(void)
{
	int i;
	for (i = 0; i < VIDEO_SIZE; i++)
	{
		*((VIDEO_START + i)) = backup[i];
	}
	screen_saver_reset();
}

void video_screen_saver_check_count(void)
{
	screen_saver_count();

	if (screen_saver_counter == 1000)
	{
		screen_saver_enable();
	}
	if (screen_saver_active)
		screen_saver_draw();
}

void video_screen_saver_check_restore(void)
{
	if (screen_saver_active)
	{
		screen_saver_disable();
	}
	screen_saver_reset_counter();
}

void screen_saver_reset_counter(void){
	screen_saver_counter = 0;
}

void screen_saver_enable(void)
{
	screen_saver_active = 1;
	screen_saver();
}

void screen_saver_disable(void)
{
	screen_saver_active = 0;
	restore();
}

void screen_saver_count(void)
{
	screen_saver_counter++;
}

void screen_saver_reset()
{
	screen_saver_drawer = 0;
}

void screen_saver()
{
	backup_screen();
	int i;
	for (i = 0; i < VIDEO_SIZE; i++)
	{
		*((VIDEO_START + i)) = 'F';
	}
}

void screen_saver_draw()
{
	screen_saver_drawer = screen_saver_drawer % (VIDEO_HEIGHT * VIDEO_WIDTH * 2);
	VIDEO_START[screen_saver_drawer] = screen_saver_drawer;
	screen_saver_drawer++;
}

void video_write_byte(char c)
{
	if (c == BACKSPACE_BYTE){
		video_backspace();
		return;
	}

	if (c == '\n'){
		video_new_line();
		video_reset_bff_counter();
		return;
	}


	if (c == 0){
		video_reset_bff_counter();
		return;
	}

	if (video > VIDEO_END)
	{
		video_scroll();
	}
	
	*video = c;
	video_next_char();
	video_bff_counter_inc();
}

void video_next_char()
{
	video += 2;
}

void video_prev_char(){
	video -= 2;
}

void video_clear_screen()
{
	int i;

	for (i = 0; i < VIDEO_SIZE; i++)
		video[i] = 0;

	video = VIDEO_START;
}

void video_new_line()
{
	do
	{
		*video = 0;
		video += 2;
	}
	while((uint64_t)(video - VIDEO_START) % (VIDEO_WIDTH * 2) != 0);

	if (video == VIDEO_END)
		video_scroll();
}

void video_scroll()
{
	char * copy_from = (char *) (VIDEO_START + VIDEO_WIDTH * 2);
	video = (char *)VIDEO_START;

	do
	{
		*video = *copy_from;
		video += 2;
		copy_from += 2;
	}
	while ((uint64_t)copy_from != FIRST_POS_AFTER_VIDEO);

	video = (char *) (VIDEO_START + (VIDEO_HEIGHT - 1)*VIDEO_WIDTH * 2);

	do
	{
		*video = 0;
		video += 2;
	}
	while(video != FIRST_POS_AFTER_VIDEO);

	video = (char *) (VIDEO_START + (VIDEO_WIDTH * (VIDEO_HEIGHT-1) * 2));


}

void video_backspace(){
	if (video_bff_counter > 0){
		video_bff_counter--;
		video_prev_char();
		*video = 0;
	}
}

void video_reset_bff_counter(){
	video_bff_counter = 0;
}
void video_bff_counter_inc(){
	video_bff_counter++;
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