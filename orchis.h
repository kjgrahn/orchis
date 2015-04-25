/* -*- c++ -*-
 *
 * orchis - build C++ or C unit tests
 *
 * The orchis::Failure exception, and assertions. See
 * orchis(1) for documentation.
 *
 * Copyright (c) 2011, 2012, 2013-2015 Jörgen Grahn
 * All rights reserved.
 */
#ifndef ORCHIS_H
#define ORCHIS_H

#ifdef __cplusplus

#include <algorithm>
#include <iostream>
#include <sstream>

namespace orchis {

    /* marker; void foo(TC) is a test case */
    struct TC {};

    struct Failure {
	Failure()
	    : what("true")
	{}
	explicit Failure(const std::string& message)
	    : what(message)
	{}
	explicit Failure(const std::ostringstream& message)
	    : what(message.str())
	{}
	template<class T>
	explicit Failure(const T& message)
	{
	    std::ostringstream oss;
	    oss << message;
	    what = oss.str();
	}

	std::string what;
    };

    inline
    std::ostream& operator<< (std::ostream& os, const Failure& e) {
	return os << "Failure: " << e.what;
    }

    template<class Iter>
    std::ostream& put_seq(std::ostream& os,
			  Iter first,
			  Iter last) {
	os << '[';
	Iter i = first;
	while(i!=last) {
	    if(i!=first) {
		os << ", ";
	    }
	    os << *i;
	    ++i;
	}
	return os << ']';
    }


    template<class T>
    void assert_(const T& val) {
	if(val) return;
	std::ostringstream ss;
	ss << val;
	throw Failure(ss);
    }

    template<class M, class N>
    void assert_eq(const M& m, const N& n) {
	if(m == n) return;
	std::ostringstream ss;
	ss << m << " == " << n;
	throw Failure(ss);
    }

    template<class M, class N>
    void assert_neq(const M& m, const N& n) {
	if(m != n) return;
	std::ostringstream ss;
	ss << m << " != " << n;
	throw Failure(ss);
    }

    template<class M, class N>
    void assert_lt(const M& m, const N& n) {
	if(m < n) return;
	std::ostringstream ss;
	ss << m << " < " << n;
	throw Failure(ss);
    }

    template<class M, class N>
    void assert_gt(const M& m, const N& n) {
	if(m > n) return;
	std::ostringstream ss;
	ss << m << " > " << n;
	throw Failure(ss);
    }

    template<class M, class N>
    void assert_le(const M& m, const N& n) {
	if(m <= n) return;
	std::ostringstream ss;
	ss << m << " <= " << n;
	throw Failure(ss);
    }

    template<class M, class N>
    void assert_ge(const M& m, const N& n) {
	if(m >= n) return;
	std::ostringstream ss;
	ss << m << " >= " << n;
	throw Failure(ss);
    }

    template<class T, class Iter>
    void assert_in(Iter first, Iter last, const T& t) {
	if(std::find(first, last, t) != last) return;
	std::ostringstream ss;
	ss << t << " in ";
	put_seq(ss, first, last);
	throw Failure(ss);
    }

    template<class T, class C>
    void assert_in(const C& container, const T& t) {
	assert_in(container.begin(),
		  container.end(),
		  t);
    }

    template<class T, class Iter>
    void assert_not_in(Iter first, Iter last, const T& t) {
	if(std::find(first, last, t) == last) return;
	std::ostringstream ss;
	ss << t << " not in ";
	put_seq(ss, first, last);
	throw Failure(ss);
    }

    template<class T, class C>
    void assert_not_in(const C& container, const T& t) {
	assert_not_in(container.begin(),
		      container.end(),
		      t);
    }
}

#else

/* The primitive C interface */

void orchis_escape(const char* msg);

#define ASSERT(cond) do { if(!(cond)) orchis_escape(#cond); } while(0)

#endif

#endif
