/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 */
#ifndef ORCHIS_TESTS_H
#define ORCHIS_TESTS_H

#include <iosfwd>
#include <vector>
#include <string>

/**
 * Tests, as found as symbols in a set of object files or archives.
 *
 * test_foo T                 C test
 * foo::bar(orchis::TC) T     normal C++ test
 * foo::test_bar() T          old-style C++ test
 */
class Tests {
public:
    Tests(std::ostream& err, const std::vector<std::string>& objects);
    bool bad = false;
};

void generate(std::ostream& os, const Tests& tests);

#endif
