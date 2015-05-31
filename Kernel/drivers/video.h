#define VIDEO_START 0xB8000

void video_write_byte(char c);
void video_next_char();
void video_clear_screen();
void video_new_line();
void video_scroll();