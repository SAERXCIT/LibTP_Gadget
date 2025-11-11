CC_64=x86_64-w64-mingw32-gcc

all: libtp_gadget.x64.zip

bin:
	mkdir bin

libtp_gadget.x64.zip: bin
	$(CC_64) -DWIN_X64 -shared -Wall -Wno-pointer-arith -fno-ident -c src/tp_gadget.c -o bin/tp_gadget.x64.o
	$(CC_64) -DWIN_X64 -shared -Wall -Wno-pointer-arith -fno-ident -c src/hooks.c -o bin/hooks.x64.p
	zip -q -j libtp_gadget.x64.zip bin/tp_gadget.x64.o bin/hooks.x64.p

clean:
	rm -rf bin/
	rm -f libtp_gadget.x64.zip
