encina:
	gcc parking.c lib/libparking.a -o out/parking_encina -lm

linux:
	gcc parking.c lib/libparking.a -o out/parking_linux -lm -m32