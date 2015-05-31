
#include <keyboard.h>
#include <video.h>

void keyboard(){
	unsigned char status;
	char keycode;

	status = read_port(0x64);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = read_port(0x60);
		if(keycode < 0)
			return;
		switch(keyboard_map[keycode]){
			case '\n':
				video_new_line();
				break;
			default:
				video_write_byte(keyboard_map[keycode]);
				break;
		}	
	}
	
}