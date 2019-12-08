/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 */
#ifndef ORCHIS_TESTS_H
#define ORCHIS_TESTS_H

#include <iosfwd>

class Tests {
public:
    template<class T>
	Tests(std::ostream&, T) : bad(false) {}
    bool bad;
};

#endif
