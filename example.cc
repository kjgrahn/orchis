/* Copyright (c) 2014 Jörgen Grahn.
 * All rights reserved.
 */
#include <orchis.h>

#include <set>


void in_global_namespace(orchis::TC)
{
    orchis::assert_true("ok");
}

namespace will_pass {

    void empty(orchis::TC)
    {
    }

    void underscore(orchis::TC)
    {
	orchis::assert_("ok");
    }

    void simple(orchis::TC)
    {
	orchis::assert_true("ok");
	orchis::assert_false(1==2);
    }

    void test_old_naming()
    {
	orchis::assert_true("ok");
    }

    void eq(orchis::TC)
    {
	orchis::assert_eq(false, 0);
	orchis::assert_neq(true, 0);
    }

    void relational(orchis::TC)
    {
	orchis::assert_lt(3, 3.141592654);
	orchis::assert_gt(4, 3.141592654);
	orchis::assert_le(0, 0);
	orchis::assert_ge(0, 0);
    }

    void membership(orchis::TC)
    {
	const int v[] = { 1, 3, 5, 7, 9 };
	std::set<int> s(v, v+5);
	orchis::assert_in(s, 3);
	orchis::assert_in(v, v+2, 3);
	orchis::assert_not_in(v, v+1, 3);
	orchis::assert_not_in(s, 4);
	orchis::assert_not_in(s, 3.141592654);
    }

    void not_a_test()
    {
	throw "I'm never called!";
    }
}

namespace will_fail {

    void fixed(orchis::TC)
    {
	throw orchis::Failure("Hello, world!");
    }

    void fail(orchis::TC)
    {
	orchis::fail("Hello, world!");
    }

    void simple(orchis::TC)
    {
	orchis::assert_false("foo");
    }

    void eq(orchis::TC)
    {
	const std::string s("foo");
	orchis::assert_eq(s, "bar");
    }

    void relational(orchis::TC)
    {
	orchis::assert_ge(3, 3.141592654);
    }

    void membership(orchis::TC)
    {
	const int v[] = { 1, 3, 5, 7, 9 };
	std::set<int> s(v, v+5);
	orchis::assert_in(s, 4);
    }
}

namespace will_give_error {

    void simple(orchis::TC)
    {
	throw "oops!";
    }
}
