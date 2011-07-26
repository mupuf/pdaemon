TARGET = pdaemon

CLANG = clang
CLANG_FLAGS = -m32 -emit-llvm -S -O1
ENVYAS = envyas
ENVYAS_FLAGS = -m fuc
LLC = llc
LLC_FLAGS = -march=nvfuc
LLVM2ENVYAS = ./fucc-scripts/llvm2envyas.pl
HEX2BIN = ./fucc-scripts/hex2bin.pl
CAT = cat
BOOT_S = ./fucc-scripts/boot.S

all: main.c
	$(CLANG) $< -o $(TARGET).bc $(CLANG_FLAGS)
	$(LLC) $(TARGET).bc -o $(TARGET).s $(LLC_FLAGS)
	$(LLVM2ENVYAS) $(TARGET).s -c $(TARGET).tmp -d $(TARGET).data.S
	$(CAT) $(BOOT_S) $(TARGET).tmp > $(TARGET).S
	$(ENVYAS) -i $(TARGET).S $(ENVYAS_FLAGS) > $(TARGET).bin
	$(HEX2BIN) $(TARGET).data.S $(TARGET).data.bin

clean:
	rm -f $(TARGET).* *~ 
