/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 */
#include "tests.h"

#include "popen.h"
#include "split.h"

#include <iostream>

namespace {

    std::vector<std::string> joinv(std::vector<std::string> a,
				   const std::vector<std::string>& b)
    {
	a.insert(end(a), begin(b), end(b));
	return a;
    }
}

Tests::Tests(std::ostream&, const std::vector<std::string>& objects)
{
    Popen nm {joinv({"nm", "-CP"}, objects)};
    std::string s;
    while (nm.getline(s)) {
	const auto v = split(s, 3);
	if (v.size() < 2 || v[1] != "T") continue;
	const Function fun {v[0]};
	if (fun.is_test()) tt.push_back(fun);
    }
    bad = nm.wait();
}

void generate(std::ostream& os, const Tests& tests) {}
