CFLAGS += -std=c23 -D_DEFAULT_SOURCE
LDFLAGS += -lm

all: diffcat framescat

diffcat: diffcat.o comparator.o pbm.o dllist.o T.o
framescat: framescat.o pbm.o dllist.o T.o 

.PHONY: all clean
clean:
	rm -f *.o diffcat framescat
