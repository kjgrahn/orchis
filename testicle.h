/* -*- c++ -*- */
#ifndef TESTICLE_H
#define TESTICLE_H

#include <algorithm>

namespace testicle {

    class AssertionError {};

    inline
    void assert_(bool val) {
	if(val) return;
	throw AssertionError();
    };

    template<class M, class N>
    void assert_eq(M m, N n) {
	if(m == n) return;
	throw AssertionError();
    };

    template<class M, class N>
    void assert_neq(M m, N n) {
	if(m != n) return;
	throw AssertionError();
    };

    template<class M, class N>
    void assert_lt(M m, N n) {
	if(m < n) return;
	throw AssertionError();
    };

    template<class M, class N>
    void assert_gt(M m, N n) {
	if(m > n) return;
	throw AssertionError();
    };

    template<class T, class C>
    void assert_in(C container, T t) {
	if(container.find(t) != container.end()) return;
	throw AssertionError();
    };

    template<class T, class Iter>
    void assert_in(Iter first, Iter last, T t) {
	if(std::find(first, last, t) != last) return;
	throw AssertionError();
    };

    template<class T, class C>
    void assert_not_in(C container, T t) {
	if(container.find(t) == container.end()) return;
	throw AssertionError();
    };

    template<class T, class Iter>
    void assert_not_in(Iter first, Iter last, T t) {
	if(std::find(first, last, t) == last) return;
	throw AssertionError();
    };
}

#endif
