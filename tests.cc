/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 */
#include "tests.h"

#include "popen.h"
#include "split.h"

#include <iostream>
#include <algorithm>

namespace {

    bool starts_with(const std::string& haystack,
		     const std::string& needle)
    {
	auto i = std::search(begin(haystack), end(haystack),
			     begin(needle), end(needle));
	return i==begin(haystack);
    }
}

Function::Function(const std::string& s)
    : kind(0)
{
    auto lpar = std::find(begin(s), end(s), '(');
    ns_f = {begin(s), lpar};
    const std::string arg {lpar, end(s)};
    ns = split("::", ns_f);
    if (ns.empty()) return;
    fun = ns.back();
    ns.pop_back();

    if (arg=="") {
	if (starts_with(fun, "test")) kind = 'c';
    }
    else if (arg=="()") {
	if (starts_with(fun, "test")) kind = 'O';
    }
    else if (arg=="(orchis::TC)") {
	kind = 'o';
    }
}

bool Function::is_test() const
{
    switch (kind) {
    case 'o':
    case 'O':
    case 'c':
	return true;
    default:
	return false;
    }
}

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
