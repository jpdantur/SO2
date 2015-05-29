
#include <keyboard.h>
#include <video.h>

void keyboard(){
	char c=read_port(0x60);
	video_write_byte('f');
	
}