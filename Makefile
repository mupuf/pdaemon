all: run nva3_pdaemon.out.fuc
.PHONY: clean

RM=/bin/rm
M4=/usr/bin/m4

ET=../envytools
CFLAGS= -I$(ET)/include -Wall -Wno-unused-function -O2
LDFLAGS= -L$(ET)/build/nva -lnva -lpciaccess
ED2AS=envyas -w -m fuc
ED2DIS=envydis -w -m fuc

clean_nva3:
	-@$(RM) nva3_pdaemon.fuc.h 2> /dev/null
	-@$(RM) nva3_pdaemon.out.fuc 2> /dev/null
	-@$(RM) nva3_pdaemon.fuc 2> /dev/null

clean_nvd9:
	-@$(RM) nvd9_pdaemon.fuc.h 2> /dev/null
	-@$(RM) nvd9_pdaemon.out.fuc 2> /dev/null
	-@$(RM) nvd9_pdaemon.fuc 2> /dev/null

clean: clean_nva3 clean_nvd9

nva3_pdaemon.fuc: clean_nva3 pdaemon.fuc
	$(M4) -D NVA3 pdaemon.fuc > nva3_pdaemon.fuc

nvd9_pdaemon.fuc: clean_nvd9 pdaemon.fuc
	$(M4) pdaemon.fuc > nvd9_pdaemon.fuc

nva3_pdaemon.fuc.h: nva3_pdaemon.fuc
	$(ED2AS) -V fuc3 -a nva3_pdaemon.fuc -o nva3_pdaemon.fuc.h

nva3_pdaemon.out.fuc: nva3_pdaemon.fuc
	$(ED2AS) -V fuc3 nva3_pdaemon.fuc | $(ED2DIS) -d 24 > nva3_pdaemon.out.fuc

nvd9_pdaemon.fuc.h: nvd9_pdaemon.fuc
	$(ED2AS) -V fuc4 -a nvd9_pdaemon.fuc -o nvd9_pdaemon.fuc.h

nvd9_pdaemon.out.fuc: nvd9_pdaemon.fuc
	$(ED2AS) -V fuc4 nvd9_pdaemon.fuc | $(ED2DIS) -d 24 > nvd9_pdaemon.out.fuc

run: run.c nva3_pdaemon.fuc.h nvd9_pdaemon.fuc.h
	$(CC) run.c -g -o run $(CFLAGS) $(LDFLAGS)
