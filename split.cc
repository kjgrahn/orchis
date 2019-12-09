/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 */
#include "split.h"

#include <cctype>
#include <iostream>
#include <sstream>

std::vector<std::string> split(const std::string& s, size_t limit)
{
    auto a = s.data();
    auto b = a + s.size();
    return split(a, b, limit);
}

namespace {
    inline bool isspace(char ch)
    {
        return std::isspace(static_cast<unsigned char>(ch));
    }

    /**
     * Trim whitespace to the left in [a, b).
     */
    inline
    const char* ws(const char* a, const char* b)
    {
        while(a!=b && isspace(*a)) a++;
        return a;
    }

    /**
     * Trim non-whitespace to the left in [a, b).
     */
    inline
    const char* non_ws(const char* a, const char* b)
    {
        while(a!=b && !isspace(*a)) a++;
        return a;
    }

    /**
     * Trim whitespace to the right in [a, b), so that
     * it's either empty or ends with non-whitespace.
     */
    inline
    const char* trimr(const char* a, const char* b)
    {
        while(a!=b && isspace(*(b-1))) b--;
        return b;
    }
}

/**
 * Split the string [a, b) on whitespace, kind of like the Perl split
 * function.
 *
 * If limit is non-zero, the string is split into max 'limit' pieces,
 * and the last one may contain whitespace.
 *
 * No piece will ever have leading or trailing whitespace, or be
 * empty.
 */
std::vector<std::string> split(const char* a, const char* b,
			       const size_t limit)
{
    std::vector<std::string> acc;

    a = ws(a, b);
    while (a!=b) {
	if (acc.size() + 1 == limit) {
	    acc.emplace_back(a, trimr(a, b));
	    break;
	}
	auto c = non_ws(a, b);
	acc.emplace_back(a, c);
	a = ws(c, b);
    }

    return acc;
}

namespace {
    template <class Sep, class Cont>
    std::ostream& jointo(std::ostream& os, const Sep& sep, const Cont& v)
    {
	bool first = true;
	for (const auto& item: v) {
	    if (first) {
		first = false;
		os << item;
	    }
	    else {
		os << sep << item;
	    }
	}
	return os;
    }
}

std::string join(char sep,
		 const std::vector<std::string>& v)
{
    std::ostringstream oss;
    jointo(oss, sep, v);
    return oss.str();
}

std::string join(const std::string& sep,
		 const std::vector<std::string>& v)
{
    std::ostringstream oss;
    jointo(oss, sep, v);
    return oss.str();
}
