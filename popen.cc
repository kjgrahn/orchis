/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 */
#include "popen.h"

#include <iostream>
#include <algorithm>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

namespace {
    std::vector<char*> argv_of(const std::vector<std::string>& argv)
    {
	std::vector<char*> v(argv.size()+1, nullptr);
	auto it = begin(v);
	for (const auto& s: argv) {
	    *it++ = const_cast<char*>(s.c_str());
	}
	return v;
    }
}

/**
 * execvp() argv[0] with no standard input, a pipe as stdout, and
 * stderr shared with the parent.
 */
Popen::Popen(const std::vector<std::string>& argv)
{
    int pipefd[2];
    if (pipe(pipefd)) {
	child = 0;
	return;
    }

    child = fork();
    if (child==-1) {
	close(pipefd[0]);
	close(pipefd[1]);
	child = 0;
    }
    else if (child==0) {
	int writefd = pipefd[1];
	close(pipefd[0]);
	close(0);
	const auto args = argv_of(argv);
	dup2(writefd, 1);
	execvp(args[0], &args[0]);
	std::cerr << "error: failed to execute " << argv[0]
		  << ": " << std::strerror(errno) << '\n';
	std::exit(1);
    }
    else {
	readfd = pipefd[0];
	close(pipefd[1]);
    }
}

Popen::~Popen()
{
    if (child) {
	wait();
	close(readfd);
    }
}

int Popen::wait()
{
    int status;
    waitpid(child, &status, 0);
    child = 0;
    return status;
}

namespace {

    /**
     * Consume the first line of text in cont, a container of char, and
     * put it in 's'.  Returns false if that's not possible (no \n in
     * cont).
     */
    template <class Cont>
    bool eat_line(Cont& c, std::string& s)
    {
	auto a = begin(c);
	auto b = std::find(a, end(c), '\n');
	if (b==end(c)) return false;

	s.assign(a, b);
	c.erase(a, next(b));
	return true;
    }

    template <class Cont>
    bool eat_end(Cont& c, std::string& s)
    {
	if (c.empty()) return false;

	s.assign(begin(c), end(c));
	c.clear();
	return true;
    }

    template <class Cont>
    void append(Cont& c, const char* p, size_t n)
    {
	c.insert(std::end(c), p, p+n);
    }
}

/**
 * Read a line of text and return it in 's', without the \n if any.
 * If we've reached EOF nothing is filled in and the function returns
 * false, so we can do:
 *
 *    while(p.getline(s)) {
 *       process(s);
 *    }
 */
bool Popen::getline(std::string& s)
{
    if (eat_line(buf, s)) return true;

    if (eof) {
	return eat_end(buf, s);
    }

    char cbuf[8192];
    const ssize_t res = read(readfd, cbuf, sizeof cbuf);
    if (res==-1) return false;
    const size_t n = res;
    if (n) {
	append(buf, cbuf, n);
	if (eat_line(buf, s)) return true;
	// bogus: really long lines will smash the stack
	// due to recursion
	return getline(s);
    }
    else {
	eof = true;
	return eat_end(buf, s);
    }
}
