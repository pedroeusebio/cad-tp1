

## Main config file
include make.config


## Objects and sources
CSRCS	= $(wildcard *.cc)
COBJS	= $(CSRCS:.cc=.o)

CONFIG_EXEC = wave.exe


all:$(COBJS)
	$(CONFIG_CC) $(CONFIG_LDFLAGS) $(COBJS) $(CONFIG_LIBS) -o $(CONFIG_EXEC)

clean:
	$(CONFIG_RM) $(COBJS) $(CONFIG_EXEC) *.o
