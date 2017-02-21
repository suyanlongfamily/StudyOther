#ifndef _LANG_EXCEPTION_H
#define _LANG_EXCEPTION_H

#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <stdlib.h>

namespace lang {

class Exception {
public:
	//explicit Exception(const char *_S = "unknown") throw()
	//		: _Exstr(_S) {}
	explicit Exception(const char *fmt, ...) throw()
	{
		char cp[1024 + 20];
		va_list ap;
		va_start(ap, fmt);
		_vsnprintf_s(cp, 1024, fmt, ap);
		//_vsnprintf(cp, 1024, fmt, ap);
		va_end(ap);
		cp[1024 + 20 - 1] = '\0';
		_Exstr = cp;
	}
	Exception(const std::string &_S) throw()
			: _Exstr(_S) {}
	Exception(const Exception& _R) throw()
			: _Exstr(_R._Exstr) {}
	Exception& operator = (const Exception& _R) throw() {
		_Exstr = _R._Exstr;
		return *this;
	}
	virtual ~Exception() {}

	virtual const char *what() const throw() {
		return _Exstr.c_str();
	}
protected:
	std::string _Exstr;
};

} //namespace lang

#endif //_LANG_EXCEPTION_H
