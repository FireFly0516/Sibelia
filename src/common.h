#ifndef _COMMON_H_
#define _COMMON_H_

#include <ctime>
#include <cstdio>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <set>
#include <map>
#include <list>
#include <memory>
#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <sparsehash/sparse_hash_map>
#include <sparsehash/sparse_hash_set>
#include <sparsehash/dense_hash_set>
#include <sparsehash/dense_hash_map>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&); \
	void operator=(const TypeName&)

namespace SyntenyBuilder
{
}

#endif