/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 */
#ifndef ORCHIS_POPEN_H
#define ORCHIS_POPEN_H

#include <string>
#include <vector>
#include <deque>

/**
 * Like popen(3) but with argv as a vector and only for text input.
 */
class Popen {
public:
    explicit Popen(const std::vector<std::string>& argv);
    Popen(const Popen&) = delete;
    Popen& operator= (const Popen&) = delete;
    ~Popen();

    bool getline(std::string& s);
    int wait();

private:
    pid_t child;
    int readfd;

    std::deque<char> buf;
    bool eof = false;
};

#endif
