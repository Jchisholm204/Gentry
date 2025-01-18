# Jacob Chisholm

.PHONY: all build cmake clean format

OBJECTS= usbtest.c
HEADERS= -Iusb_packet.h

PRJ=usbtest

all: build

build:
	gcc $(OBJECTS) $(HEADERS) -o $(PRJ) -lusb-1.0

clean:
	rm $(PRJ)

