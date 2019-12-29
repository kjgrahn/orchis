/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 */
#include "tests.h"

#include "split.h"
#include "namespace.h"
#include <fstream>

namespace {

    void prototypes(std::ostream& os, const Tests& tests)
    {
	Namespace ns {os};

	for (const auto f: tests) {
	    switch (f.kind) {
	    case 'o':
		os << ns.open(f.ns)
		   << "void " << f.fun << "(orchis::TC);\n";
		break;
	    case 'O':
		os << ns.open(f.ns)
		   << "void " << f.fun << "();\n";
		break;
	    case 'c':
		os << ns.open(f.ns)
		   << "extern \"C\" void " << f.fun << "(void);\n";
		break;
	    }
	}
    }

    void proxies(std::ostream& os, const Tests& tests)
    {
	Namespace ns {os};
	std::string indent;

	for (const auto f: tests) {
	    switch (f.kind) {
	    case 'O':
		indent = ns.open(f.ns);
		os << indent << "static void " << f.fun << "(orchis::TC) {\n"
		   << indent << "    " << f.fun << "();\n"
		   << indent << "}\n";
		break;
	    case 'c':
		indent = ns.open(f.ns);
		os << indent << "static void " << f.fun << "(orchis::TC) {\n"
		   << indent << "    if(setjmp(g.jmp)) throw orchis::Failure(g.msg);\n"
		   << indent << "    " << f.fun << "();\n"
		   << indent << "}\n";
		break;
	    }
	}
    }

    void entries(std::ostream& os, const Tests& tests)
    {
	for (const auto f: tests) {
	    os << "\t{ \"" << f.ns_f
	       << "\", " << f.ns_f << " },\n";
	}
    }
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
