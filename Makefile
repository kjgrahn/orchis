# $Id$
#
# Makefile
#
# Copyright (c) 2007, 2008 Jörgen Grahn.
# All rights reserved.

SHELL = /bin/sh
INSTALLBASE = /usr/local

.PHONY: all
all:

.PHONY: install
install: testicle testicle.h testicle.1
	install -m755 testicle $(INSTALLBASE)/bin/
	install -m644 testicle.1 $(INSTALLBASE)/man/man1/
	install -m644 testicle.h $(INSTALLBASE)/include/

.PHONY: clean
clean:
	rm -f *.pyc *.pyo

love:
	@echo "not war?"
