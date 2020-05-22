#
# Makefile
# 
# Defines the make commands that compile/clean specific targets
#

EXE = re0icmp
OBJS = main.o icmp.o
CC = g++

$(EXE) : $(OBJS)
	$(CC) -o $@ $(OBJS)

icmp.o : icmp.cpp icmp.hpp
main.o : main.cpp icmp.cpp icmp.hpp

clean :
	rm $(EXE)
	rm -f $(OBJS)