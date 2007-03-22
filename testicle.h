/* -*- c++ -*- */
#ifndef TESTICLE_H
#define TESTICLE_H

namespace testicle {

    class AssertionError {};

    inline void assert_(bool res) {
	if(!res) throw AssertionError();
    };
}

#endif
