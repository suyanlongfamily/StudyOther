//#include <iostream>
//#include <string>
//using namespace std;

#include <stdio.h>

/*!
 *2016/12/01 10:16:57
 *\author suyanlong
 *	1、是否能用作函数指针，或者能否用做回调与调用约定没有直接关系，而与函数指针类型有关系。
 *  2、因此stdcall  cdecl 等调用约定，都是可以用作回调、事件注册机制里面，只是说一定要符合函数类型声明。
 *  3、stdcall标准调用约定， cdecl 
 *  4、关于线程是一个函数，然而既然是一个函数，都有调用约定，windows API 基本都是_stdcall约定。其他函数库是默认cdecl约定，但是不会影响参数的出栈。
 *  
 */

typedef int (_stdcall *FUN)(int a,int b);

int  _stdcall add(int a,int b){	  
   return a+b;	
}

typedef int (_cdecl *FUN1)(int a,int b);

int  _cdecl add1(int a,int b){	  
	return a+b;	
}

int main(){	 

	FUN f = add;
	int k = f(2,3);
	return 0;
}