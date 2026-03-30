encina:
	gcc parking.c lib/libparking.a -o out/parking_encina -lm

linux:
	gcc parking.c lib/libparking.a -o out/parking_linux -lm -m32

test:
	cd out/ && ./parking_linux 1 1 D 2>test_log