/* -*- c++ -*- */
#ifndef TESTICLE_H
#define TESTICLE_H

#include <algorithm>
#include <iostream>
#include <sstream>

namespace testicle {

    struct AssertionError {
	AssertionError()
	    : what("false")
	{}
	explicit AssertionError(const std::string& message)
	    : what(message)
	{}
	explicit AssertionError(const std::ostringstream& message)
	    : what(message.str())
	{}

	std::string what;
    };

    inline
    std::ostream& operator<< (std::ostream& os, const AssertionError& e) {
	return os << e.what;
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
	throw AssertionError(ss);
    }

    template<class M, class N>
    void assert_eq(const M& m, const N& n) {
	if(m == n) return;
	std::ostringstream ss;
	ss << m << " == " << n;
	throw AssertionError(ss);
    }

    template<class M, class N>
    void assert_neq(const M& m, const N& n) {
	if(m != n) return;
	std::ostringstream ss;
	ss << m << " != " << n;
	throw AssertionError(ss);
    }

    template<class M, class N>
    void assert_lt(const M& m, const N& n) {
	if(m < n) return;
	std::ostringstream ss;
	ss << m << " < " << n;
	throw AssertionError(ss);
    }

    template<class M, class N>
    void assert_gt(const M& m, const N& n) {
	if(m > n) return;
	std::ostringstream ss;
	ss << m << " > " << n;
	throw AssertionError(ss);
    }

    template<class T, class Iter>
    void assert_in(Iter first, Iter last, const T& t) {
	if(std::find(first, last, t) != last) return;
	std::ostringstream ss;
	ss << t << " in ";
	put_seq(ss, first, last);
	throw AssertionError(ss);
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
	throw AssertionError(ss);
    }

    template<class T, class C>
    void assert_not_in(const C& container, const T& t) {
	assert_not_in(container.begin(),
		      container.end(),
		      t);
    }
}

#endif
