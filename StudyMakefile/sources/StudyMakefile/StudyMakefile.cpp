#include <iostream>
#include <string>
using namespace std;

#include <stdio.h>

/*!
 *2016/12/01 10:16:57
 *\author suyanlong
 *	1���Ƿ�����������ָ�룬�����ܷ������ص������Լ��û��ֱ�ӹ�ϵ�����뺯��ָ�������й�ϵ��
 *  2�����stdcall  cdecl �ȵ���Լ�������ǿ��������ص����¼�ע��������棬ֻ��˵һ��Ҫ���Ϻ�������������
 *  3��stdcall��׼����Լ���� cdecl 
 *  4�������߳���һ��������Ȼ����Ȼ��һ�����������е���Լ����windows API ��������_stdcallԼ����������������Ĭ��cdeclԼ�������ǲ���Ӱ������ĳ�ջ��
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

	//�鿴���Դ��֪�����������
	unsigned int a=20;	//01161478  mov         dword ptr [a],14h 
	signed int b=-130;	//0116147F  mov         dword ptr [b],0FFFFFF7Eh  

	int kk = -0x9A;	//������ʲô���ƣ�������������
	
	int values = 100;
	values &= -0xffffffffffffffff;

	std::cout<<a+b<<std::endl;	
	return 0;
}