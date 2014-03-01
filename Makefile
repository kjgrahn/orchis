# $Id$
#
# Makefile
#
# Copyright (c) 2007, 2008 Jörgen Grahn.
# All rights reserved.

SHELL=/bin/sh
INSTALLBASE=/usr/local

CXXFLAGS=-W -Wall -pedantic -std=c++98 -g -Os
CPPFLAGS=-I.

.PHONY: all
all:

# Note that the building of example unit tests is a bit extra
# complicated since it cannot use a properly installed testicle.
# See the manual for a suggested Makefile entry.

.PHONY: check checkv
check: example
	./example
checkv: example
	valgrind -q ./example -v

test.cc: example.o testicle
	./testicle -o$@ example.o

example: test.o example.o
	$(CXX) $(CXXFLAGS) -o $@ test.o example.o

example.o: testicle.h

.PHONY: install
install: testicle testicle.h testicle.1
	install -m755 testicle $(INSTALLBASE)/bin/
	install -m644 testicle.1 $(INSTALLBASE)/man/man1/
	install -m644 testicle.h $(INSTALLBASE)/include/

.PHONY: clean
clean:
	$(RM) *.pyc *.pyo
	$(RM) example test.cc *.o

love:
	@echo "not war?"
