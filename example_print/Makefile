CC_64=x86_64-w64-mingw32-gcc

all: example_print.x64.o

#
# x64 targets
#
example_print.x64.o:
	$(CC_64) -DWIN_X64 -shared -masm=intel -Wall -Wno-pointer-arith -c example_print.c -o example_print.x64.o

#
# Other targets
#
clean:
	rm -f example_print.x64.o
