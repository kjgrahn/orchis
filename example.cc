/* Copyright (c) 2014 Jörgen Grahn.
 * All rights reserved.
 */
#include <testicle.h>

#include <set>


namespace will_pass {

    void empty(testicle::TC)
    {
    }

    void simple(testicle::TC)
    {
	testicle::assert_("ok");
    }

    void eq(testicle::TC)
    {
	testicle::assert_eq(false, 0);
	testicle::assert_neq(true, 0);
    }

    void relational(testicle::TC)
    {
	testicle::assert_lt(3, 3.141592654);
	testicle::assert_gt(4, 3.141592654);
	testicle::assert_le(0, 0);
	testicle::assert_ge(0, 0);
    }

    void membership(testicle::TC)
    {
	const int v[] = { 1, 3, 5, 7, 9 };
	std::set<int> s(v, v+5);
	testicle::assert_in(s, 3);
	testicle::assert_in(v, v+2, 3);
	testicle::assert_not_in(v, v+1, 3);
	testicle::assert_not_in(s, 4);
	testicle::assert_not_in(s, 3.141592654);
    }

    void not_a_test()
    {
	throw "I'm never called!";
    }
}

namespace will_fail {

    void fixed(testicle::TC)
    {
	throw testicle::Failure("Hello, world!");
    }

    void simple(testicle::TC)
    {
	testicle::assert_(!"foo");
    }

    void eq(testicle::TC)
    {
	const std::string s("foo");
	testicle::assert_eq(s, "bar");
    }

    void relational(testicle::TC)
    {
	testicle::assert_ge(3, 3.141592654);
    }

    void membership(testicle::TC)
    {
	const int v[] = { 1, 3, 5, 7, 9 };
	std::set<int> s(v, v+5);
	testicle::assert_in(s, 4);
    }
}

namespace will_give_error {

    void simple(testicle::TC)
    {
	throw "oops!";
    }
}
