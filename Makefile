#
# Copyright (c) 2007, 2008, 2015, 2019 Jörgen Grahn.
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local

CXXFLAGS=-W -Wall -pedantic -std=c++11 -g -Os
CFLAGS=-W -Wall -pedantic -std=c99 -g -Os
CPPFLAGS=-I.

.PHONY: all
all: orchis
all: tests
all: example

orchis: orchis.o liborchis.a
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ orchis.o -L. -lorchis

liborchis.a: tests.o
liborchis.a: generate.o
liborchis.a: namespace.o
liborchis.a: popen.o
liborchis.a: split.o
	$(AR) -r $@ $^

orchis.o: CPPFLAGS+=-DINSTALLBASE=\"$(INSTALLBASE)\"

# Note that the building of example unit tests is a bit extra
# complicated, since it cannot assume orchis has been installed
# properly.
#
# See the orchis(1) man page for how to integrate unit tests
# into a typical Makefile.

.PHONY: check checkv
check: tests example
	./tests
	./example
checkv: tests example
	valgrind -q ./tests -v
	valgrind -q ./example -v

test.cc: orchis driver.template libtests.a
	./orchis -t driver.template -o$@ libtests.a

examples.cc: orchis driver.template libexample.a
	./orchis -t driver.template -o$@ libexample.a

tests: test.o libtests.a liborchis.a
	$(CXX) $(CXXFLAGS) -o $@ test.o -L. -ltests -lorchis

example: examples.o libexample.a liborchis.a
	$(CXX) $(CXXFLAGS) -o $@ examples.o -L. -lexample -lorchis

libexample.a: example.o
libexample.a: example0.o
	$(AR) -r $@ $^

libtests.a: test/split.o
libtests.a: test/namespace.o
	$(AR) -r $@ $^

.PHONY: install
install: orchis orchis.h orchis.1
install: driver.template
install: testicle.h testicle.1
	install -d $(INSTALLBASE)/{bin,man/man1,include,lib/orchis}
	install -m755 orchis $(INSTALLBASE)/bin/
	install -m644 orchis.1 $(INSTALLBASE)/man/man1/
	install -m644 orchis.h $(INSTALLBASE)/include/
	install -m644 driver.template $(INSTALLBASE)/lib/orchis/
	ln -f $(INSTALLBASE)/bin/{orchis,testicle}
	install -m644 testicle.1 $(INSTALLBASE)/man/man1/
	install -m644 testicle.h $(INSTALLBASE)/include/

.PHONY: clean
clean:
	$(RM) orchis
	$(RM) lib*.a
	$(RM) example tests test.cc examples.cc *.o test/*.o
	$(RM) -r dep

love:
	@echo "not war?"

$(shell mkdir -p dep/test)
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
dep/test/%.d: ;
-include dep/*.d
-include dep/test/*.d
