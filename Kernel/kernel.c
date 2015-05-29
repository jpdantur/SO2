#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <types.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
//DESCR_INT idt[0x81];
//IDTR idtr;
void _int80Handler(void);
void _timerTick(void);
void _keyboard(void);
void picMasterMask(char flag);
void picSlaveMask(char flag);
void _sti(void);

void write_port(int port,char data);

void setup_IDT_entry (DESCR_INT* idt, int index, word selector, ddword offset, byte access);

typedef int (*EntryPoint)();

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	ncPrint("[x64BareBones]");
	ncNewline();
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

int main()
{	
	/*ncPrint("[Kernel Main]");
	ncNewline();
	ncPrint("  Sample code module at 0x");
	ncPrintHex((uint64_t)sampleCodeModuleAddress);
	ncNewline();
	ncPrint("  Calling the sample code module returned: ");
	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	ncNewline();
	ncNewline();

	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char*)sampleDataModuleAddress);
	ncNewline();

	ncPrint("[Finished]");
        */
	ncClear();
	set_interrupts();
	((EntryPoint)sampleCodeModuleAddress)();
	//ncClear();
	//set_interrupts();
	while(1){
	}
	return 0;
}

void set_interrupts()
{	
	//idtr.base = 0;  
	//idtr.base +=(ddword) &idt;
	//idtr.limit = sizeof(idt)-1;
	//_lidt (&idtr);
	DESCR_INT* idt;
	idt=0;
	setup_IDT_entry(idt, 0x80,0x08,&_int80Handler,0x8E);
	setup_IDT_entry(idt, 0x20,0x08,&_timerTick,0x8E);
	setup_IDT_entry(idt, 0x21,0x08,&_keyboard,0x8E);
	picMasterMask(0xFC);
	picSlaveMask(0xFF);
	_sti();
}

void setup_IDT_entry (DESCR_INT *idt,int index, word selector, ddword offset, byte access)
{
	idt[index].selector = selector;
	idt[index].offset_l = offset & 0xFFFF;
	idt[index].offset_m = (offset & 0xFFFF0000) >> 16;
	idt[index].offset_h = (offset & 0xFFFFFFFF00000000)>> 32;
	idt[index].access = access;
	idt[index].cero = 0;
	idt[index].zero = 0;
}
