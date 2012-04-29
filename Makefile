all: run nva3_pdaemon.out.fuc
.PHONY: clean

ET=../new_envytools
RM=/bin/rm
M4=/usr/bin/m4

CFLAGS= -I$(ET)/include -Wall -Wno-unused-function -O2 
LDFLAGS= -L$(ET)/build/nva -lnva -lpciaccess
ED2AS=$(ET)/build/envydis/envyas -w -m fuc
ED2DIS=$(ET)/build/envydis/envydis -w -m fuc -V nva3

clean_nva3:
	-$(RM) nva3_pdaemon.fuc.h
	-$(RM) nva3_pdaemon.out.fuc
	-$(RM) nva3_pdaemon.fuc

clean_nvd9:
	-$(RM) nvd9_pdaemon.fuc.h
	-$(RM) nvd9_pdaemon.out.fuc
	-$(RM) nvd9_pdaemon.fuc

clean: clean_nva3 clean_nvd9

nva3_pdaemon.fuc: pdaemon.fuc
	$(M4) -D NVA3 pdaemon.fuc > nva3_pdaemon.fuc

nvd9_pdaemon.fuc: pdaemon.fuc
	$(M4) pdaemon.fuc > nvd9_pdaemon.fuc

nva3_pdaemon.fuc.h: clean_nva3 nva3_pdaemon.fuc
	$(ED2AS) -V nva3 -a nva3_pdaemon.fuc -o nva3_pdaemon.fuc.h

nva3_pdaemon.out.fuc: nva3_pdaemon.fuc
	$(ED2AS) -V nva3 nva3_pdaemon.fuc | $(ED2DIS) -d 24 > nva3_pdaemon.out.fuc
	
nvd9_pdaemon.fuc.h: clean_nvd9 nvd9_pdaemon.fuc
	$(ED2AS) -V nvd9 -a nvd9_pdaemon.fuc -o nvd9_pdaemon.fuc.h

nvd9_pdaemon.out.fuc: nvd9_pdaemon.fuc
	$(ED2AS) -V nvd9 nvd9_pdaemon.fuc | $(ED2DIS) -d 24 > nvd9_pdaemon.out.fuc

run: run.c nva3_pdaemon.fuc.h nvd9_pdaemon.fuc.h
	$(CC) run.c -g -o run $(CFLAGS) $(LDFLAGS)
