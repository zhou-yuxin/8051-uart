main.hex: main.rel uart.rel
	sdcc main.rel uart.rel
	packihx main.ihx > main.hex

uart.rel: src/uart.h src/uart.c
	sdcc -c src/uart.c -Isrc

main.rel: src/uart.h src/main.c
	sdcc -c src/main.c -Isrc