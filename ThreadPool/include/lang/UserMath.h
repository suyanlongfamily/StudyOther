#ifndef _LANG_USERMATH_H
#define _LANG_USERMATH_H

#include <math>

namespace lang {

static const double MINDOUBLE = 1E-6;

class TUserMath
{
public:
	/*!
		10的N次方	
	*/
	static int pow10(int x) {
		int y = 1;
		if (x > 10) return 0;
		while(x-->0) y *= 10;
		return y;
	}

	/*!
		四舍五入
	*/
	static double rounded(double value, int digt) {
		double  rmd;
		rmd = fmod(value *= (digt=pow10(digt)), 1.0);
		return ((fabs(rmd)>=0.49999 ^ rmd<0.00) ? ceil(value) : floor(value)) / digt;
	}

    /*!
        判断是否为零值
    */
    static bool IsZero (double value)
    {
        return  (value < MINDOUBLE ) && ( value > (0-MINDOUBLE));
    }
};

} //namespace lang

#endif //_LANG_USERMATH_H
