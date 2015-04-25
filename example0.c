/* Copyright (c) 2014 Jörgen Grahn.
 * All rights reserved.
 */
#include <orchis.h>


void test_will_pass(void)
{
    ASSERT("ok");
}

void test_will_fail(void)
{
    ASSERT(!"foo");
}

void test_will_also_fail(void)
{
    ASSERT(2==3.14);
}
