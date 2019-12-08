#
# Copyright (c) 2007, 2008, 2015 Jörgen Grahn.
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local

CXXFLAGS=-W -Wall -pedantic -std=c++11 -g -Os
CFLAGS=-W -Wall -pedantic -std=c99 -g -Os
CPPFLAGS=-I.

.PHONY: all
all: orchis

orchis: orchis.o liborchis.a
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ orchis.o -L. -lorchis

liborchis.a: tests.o
liborchis.a: popen.o
	$(AR) -r $@ $^

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
install: testicle.h testicle.1
	install -d $(INSTALLBASE)/{bin,man/man1,include}
	install -m755 orchis $(INSTALLBASE)/bin/
	install -m644 orchis.1 $(INSTALLBASE)/man/man1/
	install -m644 orchis.h $(INSTALLBASE)/include/
	ln -f $(INSTALLBASE)/bin/{orchis,testicle}
	install -m644 testicle.1 $(INSTALLBASE)/man/man1/
	install -m644 testicle.h $(INSTALLBASE)/include/

.PHONY: clean
clean:
	$(RM) orchis
	$(RM) lib*.a
	$(RM) example test.cc *.[oa]
	$(RM) -r dep

love:
	@echo "not war?"

$(shell mkdir -p dep)
DEPFLAGS=-MT $@ -MMD -MP -MF dep/$*.Td
COMPILE.cc=$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.c=$(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o: %.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

%.o: %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

dep/%.d: ;
-include dep/*.d
