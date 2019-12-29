/* -*- c++ -*-
 *
 * Copyright (c) 2019 Jörgen Grahn
 * All rights reserved.
 */
#ifndef ORCHIS_SPLIT_H
#define ORCHIS_SPLIT_H

#include <string>
#include <vector>

std::vector<std::string> split(const std::string& s, size_t limit = 0);
std::vector<std::string> split(const char* a, const char* b,
			       size_t limit = 0);

std::vector<std::string> split(const std::string& delimiter,
			       const std::string& s);

std::string join(char sep,
		 const std::vector<std::string>& v);
std::string join(const std::string& sep,
		 const std::vector<std::string>& v);

#endif
