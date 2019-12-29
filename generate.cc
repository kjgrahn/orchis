/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 */
#include "tests.h"

#include "split.h"
#include <fstream>

namespace {

    void prototypes(std::ostream& os, const Tests& tests) {}
    void proxies(std::ostream& os, const Tests& tests) {}
    void entries(std::ostream& os, const Tests& tests) {}
}

/**
 * Write the test driver to 'os', based on the list of tests and the
 * name of a file containing a template for the driver (main() and so
 * on).
 */
void generate(std::ostream& os,
	      const std::string& drivert, const Tests& tests)
{
    std::ifstream is(drivert);
    std::string s;

    while(getline(is, s)) {
	const auto v = split(s, 2);
	if(v.size()==2 && v[0] == "#include") {
	    const auto& section = v[1];

	    if (section=="prototypes") {
		prototypes(os, tests);
		continue;
	    }

	    if (section=="proxies") {
		proxies(os, tests);
		continue;
	    }

	    if (section=="entries") {
		entries(os, tests);
		continue;
	    }
	}

	os << s << '\n';
    }
}
