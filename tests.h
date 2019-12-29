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
 * A T symbol as seen by ld(1).
 */
class Function {
public:
    explicit Function(const std::string& s);
    bool is_test() const;

    std::string ns_f;
    std::vector<std::string> ns;
    std::string fun;
    char kind;
};

/**
 * Tests, as found as symbols in a set of object files or archives.
 *
 * test_foo               C test
 * foo::bar(orchis::TC)   normal C++ test
 * foo::test_bar()        old-style C++ test
 */
class Tests {
public:
    Tests(std::ostream& err, const std::vector<std::string>& objects);
    bool bad = false;

    std::vector<Function>::const_iterator begin() const { return tt.begin(); }
    std::vector<Function>::const_iterator end() const { return tt.end(); }

private:
    std::vector<Function> tt;
};

void generate(std::ostream& os,
	      const std::string& drivert, const Tests& tests);

#endif
