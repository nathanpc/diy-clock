NAME = bedside-clock
DEVICE= msp430g2553
PROJECT = $(NAME)-$(DEVICE)
CC = msp430-gcc -std=c99 -Os -mmcu=$(DEVICE)
MSPDEBUG = sudo mspdebug rf2500
SRC = main.c PCD8544.c rtc.c adc.c buzzer.c delay.c bitio.c ftoa.c

all: $(PROJECT).elf

$(PROJECT).elf: $(SRC)
	$(CC) -o $(PROJECT).elf $(SRC)

write: $(PROJECT).elf
	$(MSPDEBUG) "prog $(PROJECT).elf"

run: $(PROJECT).elf
	$(MSPDEBUG) "prog $(PROJECT).elf" "reset" "run"

reset:
	$(MSPDEBUG) "reset"

erase:
	$(MSPDEBUG) "erase"

clean:
	rm -f *.elf
