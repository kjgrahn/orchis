/* Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 */
#include "namespace.h"

#include <algorithm>
#include <iostream>

Namespace::Namespace(std::ostream& os)
    : os(os)
{}

Namespace::~Namespace()
{
    open({});
}

namespace {

    std::string indent(unsigned n)
    {
	return std::string(n*4, ' ');
    }
}

/**
 * Begin a namespace, closing old levels and opening new levels as necessary.
 * Also returns an indentation string suitable for lines inside the namespace.
 */
std::string Namespace::open(const std::vector<std::string>& ns)
{
    auto a = begin(now);
    const auto b = end(now);

    auto c = begin(ns);
    const auto d = end(ns);

    /*   * * * * * * * * *
     *  a,c     ` # # #   b
     *                 d
     */
    while (a!=b && c!=d) {
	if (*a != *c) break;
	a++; c++;
    }
    // close [a, b)
    unsigned n = now.size();
    while (a!=b) {
	n--;
	os << indent(n) << "}\n";
	a++;
    }
    // open [c, d)
    while (c!=d) {
	os << indent(n) << "namespace " << *c << " {\n";
	n++; c++;
    }

    now = ns;
    return indent(n);
}
