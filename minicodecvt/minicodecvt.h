// 本文件为minicodecvt的一部分，具体授权参见LICENSE文件。
// Copyright 2014, CHU.
// 创建于 2015/04/27

// Some code reduced from ydef.h from http://bitbucket.org/FrankHB/yslib ,
//	(C) 2009-2015 FrankHB. Distributed as minicodecvt's license.

#pragma once

#include <cstdint>
#include <stdexcept>

#if (__cplusplus >= 201103L || (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + \
		__GNUC_PATCHLEVEL__) >= 40600)
#	define ynothrow noexcept
#elif (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) >= 30300
#	define ynothrow __attribute__ ((nothrow))
#else
#	define ynothrow throw()
#endif

