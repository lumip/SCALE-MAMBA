include ../CONFIG.mine

CC = g++

LIBNAME = MPC
CFLAGS = -Wall -std=c++11 -pedantic -Wextra -pthread -I$(ROOT)/src -maes -mpclmul -msse4.1 -march=core2 $(FLAGS) $(OPT)
CPPFLAGS = $(CFLAGS) 
LDLIBS = -L.. -l$(LIBNAME) -L/$(OSSL)/lib -lm -lssl -lcrypto -lmpirxx -lmpir -lcryptopp $(LDFLAGS)

all: 
	$(CC) $(CFLAGS) -o convert.x convert.cpp $(LIB) $(LDLIBS)
	$(CC) $(CFLAGS) -o process.x process.cpp $(LIB) $(LDLIBS)

clean:
	-rm *.x
	-cd VHDL; $(MAKE) clean

