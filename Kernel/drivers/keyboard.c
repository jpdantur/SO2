
#include <keyboard.h>
#include <video.h>
#define is_letter(x) (((x)>='a'&&(x)<='z')||(x>='A'&&x<='Z')?1:0)
unsigned char keyboard_map[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};
char buffer[256];
unsigned int w = 0;
unsigned int r = 0;
int shift1=0;
int shift2=0;
int control=0;
int alt=0;
void keyboard_buffer_write(){
	unsigned char status;
	char keycode;

	status = read_port(0x64);
	if (status & 0x01) {
    //__video_debug('A');
		keycode = read_port(0x60);
    //__video_debug(keycode & 0x7F);
    //__video_debug(keycode);
    if((unsigned char)keycode == (0x2a|0x80))
    {
      shift1 = 0;
      //__video_debug('X');
    }
    if ((unsigned char)keycode==(0x36|0x80))
    {
      shift2 = 0;
    }
		else if (keycode > 0)
		{
      switch (keycode)
      {
        case 0x2a:
          shift1=1;
          break;
        case 0x36:
          //__video_debug((0x80|0x2a)+'0');
          shift2=1;
          break;
        default:
          if ((shift1||shift2) && is_letter(keyboard_map[keycode]))
			       buffer[w%256]=keyboard_map[keycode]-('a'-'A');
          else
             buffer[w%256]=keyboard_map[keycode];
          //w++;
          video_write_byte(buffer[w%256]);
          w++;
          break;
      }
      //__video_debug('P');
      //__video_debug(keyboard_map[keycode]);
			//*((char*)0xB8000)=w+'0';
		}
	}

	
}

char keyboard_buffer_read(){
  //__video_debug(r+'0');
	while (r==w);
		//*((char*)0xB8004)='R'
  //__video_debug(r+'0');
  //__video_debug('N');
	//*((char*)0xB8004)='N';
	//*((char*)0xB8002)=r+'0';
  //*((char*)0xB800A)=buffer[r];
	char ret = buffer[r%256];
  r++;
  return ret;
}