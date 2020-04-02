#
# Makefile
# 
# Defines the make commands that compile/clean specific targets
#

EXE = server.out
OBJS = server.o router.o
CC = g++

$(EXE) : $(OBJS)
	$(CC) -o $@ $(OBJS)

server.o : server.cpp router.o
router.o : router.cpp server.h constants.h

clean :
	rm $(EXE)
	rm -f $(OBJS)