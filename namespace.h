/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 */
#ifndef ORCHIS_NAMESPACE_H
#define ORCHIS_NAMESPACE_H

#include <iosfwd>
#include <vector>
#include <string>

/**
 * For nesting things in namespaces and indenting them:
 *
 * namespace foo {
 *     something;
 *     namepace bar {
 *         something;
 *     }
 * }
 * namespace baz {
 *     something;
 * }
 */
class Namespace {
public:
    explicit Namespace(std::ostream& os);
    ~Namespace();
    std::string open(const std::vector<std::string>& ns);

private:
    std::ostream& os;
    std::vector<std::string> now;
};

#endif
