# Makefile
# Author: Luis Colorado <luiscoloradourcola@gmail.com>
# Date: Tue Apr 18 13:29:01 CEST 2017
#
targets=mcm
mcm_objs = main.o

all: $(targets)
clean:
	$(RM) $(targets) $(mcm_objs)

mcm: $(mcm_objs)
	$(CXX) $(LDFLAGS) -o $@ $(mcm_objs) $(mcm_libs)

mcm.o main.o: mcm.cc mcm.h
