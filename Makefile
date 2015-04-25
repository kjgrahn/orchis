#
# Copyright (c) 2007, 2008, 2015 Jörgen Grahn.
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local

CXXFLAGS=-W -Wall -pedantic -std=c++98 -g -Os
CFLAGS=-W -Wall -pedantic -std=c99 -g -Os
CPPFLAGS=-I.

.PHONY: all
all:

# Note that the building of example unit tests is a bit extra
# complicated, since it cannot assume orchis has been installed
# properly.
#
# See the orchis(1) man page for how to integrate unit tests
# into a typical Makefile.

.PHONY: check checkv
check: example
	./example
checkv: example
	valgrind -q ./example -v

test.cc: libtests.a orchis
	./orchis -o$@ libtests.a

example: test.o libtests.a
	$(CXX) $(CXXFLAGS) -o $@ test.o -L. -ltests

libtests.a: example.o
libtests.a: example0.o
	$(AR) -r $@ $^

example.o: orchis.h
example0.o: orchis.h

.PHONY: install
install: orchis orchis.h orchis.1
	install -d $(INSTALLBASE)/{bin,man/man1,include}
	install -m755 orchis $(INSTALLBASE)/bin/
	install -m644 orchis.1 $(INSTALLBASE)/man/man1/
	install -m644 orchis.h $(INSTALLBASE)/include/

.PHONY: clean
clean:
	$(RM) *.pyc *.pyo
	$(RM) example test.cc *.[oa]

love:
	@echo "not war?"
