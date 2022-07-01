# reference
# $HOME/Applications/IBM/ILOG/CPLEX_Studio1262/cplex/examples/x86-64_osx/static_pic/Makefile

SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic

CPLEXDIR = /opt/ibm/ILOG/CPLEX_Studio1263/cplex
CONCERTDIR = /opt/ibm/ILOG/CPLEX_Studio1263/concert

CPLEXLIBDIR = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CPLEXINCDIR = $(CPLEXDIR)/include
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTINCDIR = $(CONCERTDIR)/include
INCLUDES = ../include
INC=$(INCLUDES) -DIL_STD
CC       = gcc
CPP      = g++ -O3 -fpermissive -Wno-deprecated -Dnullptr=NULL -DIL_STD -DILOUSESTL

SRCS=main.cpp IPSolver.cpp SequentialAlgorithm.cpp CPUTimer.cpp
PROGNAME=bomip

#CCOPT = -stdlib=libstdc++
#CCOPT = -std=c++11
CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR)

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) -I$(INC)
CCLNFLAGS = -lconcert -lilocplex -lcplex -lm -lpthread

$(PROGNAME) :
	$(CPP) $(CCFLAGS) $(CCLNDIRS) $(SRCS) -o $(PROGNAME) $(CCLNFLAGS)
