/*
 * Copyright (c) 2007, 2008, 2011, 2013-2015, 2019 Jörgen Grahn.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cerrno>

#include <getopt.h>

#include "tests.h"
#include "split.h"


int main(int argc, char ** argv)
{
    const std::string prog = argv[0] ? argv[0] : "orchis";
    const std::string usage = std::string("usage: ")
	+ prog + " [-o outfile] [-t template] infile ...\n"
	"       "
	+ prog + " --help\n"
	"       "
	+ prog + " --version";
    const char optstring[] = "o:t:";
    const struct option long_options[] = {
	{"help", 0, 0, 'H'},
	{"version", 0, 0, 'V'},
	{0, 0, 0, 0}
    };

    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    std::string outfile = "test.cc";
    std::string drivert = join('/', {INSTALLBASE, "lib", "orchis", "driver.template"});

    int ch;
    while ((ch = getopt_long(argc, argv,
			     optstring,
			     &long_options[0], 0)) != -1) {
	switch(ch) {
	case 't':
	    drivert = optarg;
	    break;
	case 'o':
	    outfile = optarg;
	    break;
	case 'H':
	    std::cout << usage << '\n';
	    return 0;
	    break;
	case 'V':
	    std::cout << "orchis 3.1\n"
		      << "Copyright (c) 2007, 2008, 2011, 2013--2015, 2019--2020 Jörgen Grahn.\n"
		      << "All rights reserved.\n";
	    return 0;
	    break;
	case ':':
	case '?':
	default:
	    std::cerr << usage << '\n';
	    return 1;
	    break;
	}
    }

    const std::vector<std::string> objects(argv+optind, argv+argc);
    if (objects.empty()) {
	    std::cerr << usage << '\n';
	    return 1;
    }

    const Tests tests{std::cerr, objects};
    if (tests.bad) return 1;

    std::ofstream out{outfile};
    if (!out) {
	std::cerr << "error: cannot open " << outfile << " for writing: "
		  << std::strerror(errno) << '\n';
    }
    generate(out, drivert, tests);

    return 0;
}
