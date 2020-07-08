#ifndef ERRORS_H_
#define ERRORS_H_

#include <stdio.h>

#define DEB 1

#if DEB
#define DEBUG(...) printf(__VA_ARGS__);
#else
#define DEBUG(...) 
#endif

#define CHECK_ERR(condition, error)\
do {\
	if ((condition)) {\
		DEBUG("\nFILE:%s, LINE:%d, FUNC:%s, ERR:%d", __FILE__, __LINE__, __PRETTY_FUNCTION__, error);\
		return error;\
	}\
} while (0);

#define CHECK(condition)\
do {\
	Error result = ((condition));\
	if (result) {\
		DEBUG("\nFILE:%s, LINE:%d, FUNC:%s, ERR:%d", __FILE__, __LINE__, __PRETTY_FUNCTION__, result);\
		return result;\
	}\
} while (0);

enum Error {
	ERR_NO_ERR,
	ERR_INV_PAR,
	ERR_INV_VAL,
	ERR_BOUNDS,
	ERR_ISTREAM,
	ERR_SHADER_COMPILE,
	ERR_SHADER_LINK
};

#endif
