include Makefile.inc

KERNEL=kernel.bin
SOURCES=drivers/keyboard.c $(wildcard *.c) drivers/video.c 
OBJECTS=$(SOURCES:.c=.o)
LOADERSRC=loader.asm
ASMSOURCES=$(filter-out $(LOADERSRC), $(wildcard *.asm)) drivers/driversasm.asm
ASMOBJECTS=$(ASMSOURCES:.asm=.o)

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(ASMOBJECTS) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(ASMOBJECTS) $(OBJECTS) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@
%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf *.o *.bin

.PHONY: all clean
