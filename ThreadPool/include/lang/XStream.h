#ifndef _LANG_XSTREAM_H
#define _LANG_XSTREAM_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

namespace lang {

class XStream 
{
public:
	typedef XStream _Myt;
	/*!
		每次内存申请的最小尺寸
	*/
	enum { BLOCK_LEN = 1024 };

public:
	XStream(int _L = BLOCK_LEN) 
		: _ptr( NULL), _ptrlen(_L), _length(0), _iter(0) {
		_ptr = new char[_L + 1];	
	}
	XStream(const char *_S, const int &_L) 
		: _ptr(NULL), _ptrlen(_L), _length(_L),_iter(0) {  
		_ptr = new char[_L + 1];
		memcpy( _ptr, _S, _L);
	}

	virtual ~XStream()  {
		if( _ptr )
			delete _ptr;
	}

	/*!
		序列化基本数据
	*/
	template<class T>
	inline _Myt& operator<<(T &_X){
		_X.serialize(*this);
		return *this;	
	}	
	/*!
		布尔型
	*/
	_Myt& operator<<(bool &_X) {
		renew(20);
		sprintf((_ptr + _length), "%d", _X);
		_length += (strlen( _ptr + _length +_iter ) + 1);
		return *this;
	}
	/*!
		字符型
	*/
	_Myt& operator<<(char &_X) {
		renew(20);
		sprintf((_ptr + _length), "%c", _X);		
		_length += (strlen( _ptr + _length +_iter ) + 1);
		return *this;
	}
	/*!
		整形
	*/
	_Myt& operator<<(int &_X) {
		renew(20);
		sprintf((_ptr + _length), "%d", _X);		
		_length += (strlen( _ptr + _length +_iter ) + 1);
		return *this;
	}
	/*!
		无符号整形
	*/
	_Myt& operator<<(unsigned int &_X) {
		renew(20);
		sprintf((_ptr + _length), "%d", _X);		
		_length += (strlen( _ptr + _length +_iter ) + 1);
		return *this;
	}
	/*!
		短整形
	*/
	_Myt& operator<<(short &_X) {
		renew(20);
		sprintf((_ptr + _length), "%hd", _X);		
		_length += (strlen( _ptr + _length +_iter ) + 1);
		return *this;
	}
	/*!
		长整形
	*/
	_Myt& operator<<(unsigned long &_X) {
		renew(20);
		sprintf((_ptr + _length), "%ld", _X);		
		_length += (strlen( _ptr + _length + _iter ) + 1);
		return *this;
	}
	/*!
		长整形
	*/
	_Myt& operator<<(long &_X) {
		renew(20);
		sprintf((_ptr + _length), "%ld", _X);		
		_length += (strlen( _ptr + _length + _iter ) + 1);
		return *this;
	}
	/*!
		长精度浮点型
	*/
	_Myt& operator<<(double &_X) {
		renew(20);
		sprintf((_ptr + _length), "%.13g", _X);		
		_length += (strlen( _ptr + _length + _iter ) + 1);
		return *this;
	}
	/*!
		浮点型
	*/
	_Myt& operator<<(float &_X) {
		renew(20);
		sprintf((_ptr + _length), "%g", _X);		
		_length += (strlen( _ptr + _length + _iter ) + 1);
		return *this;
	}
	/*!
		字符串
	*/
	_Myt& operator<<(std::string &_X) {
		renew(_X.length());
		sprintf((_ptr + _length), "%*s", _X.length(), _X.data());		
		_length += (_X.length() + 1);
		return *this;
	}
	/*!
		字符串特例序列化
	*/
	_Myt& operator<<(char* _X){
		renew( strlen(_X) );
		sprintf((_ptr + _length), "%s", _X);		
		_length += (strlen(_X) + 1);
		return *this;
	}
	/*!
		序列化数组
	*/
	template<class T>
	_Myt& write (T _X[], int _L){
		operator<<( _L );
		for(int i = 0; i < _L; i ++) {
			operator<<( _X[i] );
		}
		return *this;	
	}
	/*!  
		含非可见字符的字符串特例序列化
	*/
	_Myt& write (char _X[], int _L){
		renew(_L + 20);
		sprintf( (_ptr + _length), "%d", _L);		
		_length += (strlen(_ptr+_length) + 1 );
		memcpy( _ptr + _length + _iter, _X, _L);
		_length += (_L + 1);
		return *this;	
	}

	/*!
		反序列化基本数据
	*/
	template<class T>
	inline _Myt& operator>>(T &_X){
		_X.deserialize(*this);
		return *this;
	}
	/*!
		布尔型
	*/
	_Myt& operator>>(bool &_X) {
		_X = atoi( _ptr + _iter );
		_iter += strlen( _ptr + _iter ) + 1;
		return *this;
	}
	/*!
		字符型
	*/
	_Myt& operator>>(char &_X) {
		_X = ( _ptr + _iter )[0];
		_iter += strlen( _ptr + _iter ) + 1;
		return *this;
	}
	/*!
		整形
	*/
	_Myt& operator>>(int &_X) {
		_X = atoi( _ptr + _iter );
		_iter += strlen( _ptr + _iter ) + 1;
		return *this;
	}
	/*!
		无符号整形
	*/
	_Myt& operator>>(unsigned int &_X) {
		_X = atoi( _ptr + _iter );
		_iter += strlen( _ptr + _iter ) + 1;
		return *this;
	}
	/*!
		短整形
	*/
	_Myt& operator>>(short &_X) {
		_X = atoi( _ptr + _iter );
		_iter += strlen( _ptr + _iter ) + 1;
		return *this;
	}
	/*!
		长整形
	*/
	_Myt& operator>>(unsigned long &_X) {
		_X = atol( _ptr + _iter );
		_iter += strlen( _ptr + _iter ) + 1;
		return *this;
	}
	/*!
		长整形
	*/
	_Myt& operator>>(long &_X) {
		_X = atol( _ptr + _iter );
		_iter += strlen( _ptr + _iter ) + 1;
		return *this;
	}
	/*!
		长精度浮点型
	*/
	_Myt& operator>>(double &_X) {
		_X = atof( _ptr + _iter );
		_iter += strlen( _ptr + _iter ) + 1;
		return *this;
	}
	/*!
		浮点型
	*/
	_Myt& operator>>(float &_X) {
		_X = atof( _ptr + _iter );
		_iter += strlen( _ptr + _iter ) + 1;
		return *this;
	}
	/*!
		字符串型
	*/
	_Myt& operator>>(std::string &_X) {
		_X =  (_ptr + _iter);	
		_iter += _X.length() + 1; 
		return *this;
	}
	/*!
		字符串特例反序列化
	*/
	_Myt& operator>>(char* _X){ 
		strcpy(_X, (_ptr + _iter) );		
		_iter += (strlen( _ptr + _iter ) + 1);
		return *this;
	}
	/*!
		反序列化数组
		数组的内存由用户自己申请,因此应该防止溢出
	*/
	template<class T>
	_Myt& read (T _X[], int &_L){
		operator>>(_L);
		for(int i = 0; i < _L; i ++) {
			operator>>( _X[i] );
		}
		return *this;	
	}	
	/*
		含非可见字符的字符串特例序列化
		数组的内存由用户自己申请,因此应该防止溢出
	*/
	_Myt& read (char _X[], int &_L){
		_L = atoi( _ptr + _iter );
		_iter += strlen( _ptr + _iter ) + 1;
		memcpy(_X, _ptr + _iter, _L);
		return *this;
	}
	/*!
		返回字符流的地址
	*/
	inline char *str() { return _ptr; }

	/*!
		返回字符流的长度
	*/
	inline int size() { return _length; }

	/*!
		返回容积
	*/
	inline int capacity() { return _ptrlen; }

	/*!
		显示数据内容
	*/
	inline void display() {
		for(unsigned int i = 0; i < _length; i ++)
			std::cout << (_ptr[i] ? _ptr[i] : ' ');
		std::cout << std::endl;
	}

private:
	char*			_ptr;		//缓冲区内容
	unsigned int	_ptrlen;	//缓冲长度	
	unsigned int	_length;	//长度
	unsigned int 	_iter;		//指针
	/*!
		分配内存
	*/
	void renew(unsigned int len) 
	{
		if( _length + len >= _ptrlen ) 
		{
			char *tmp = _ptr;
			_ptrlen += (BLOCK_LEN * (len / BLOCK_LEN + 1) );
			_ptr = new char[ _ptrlen + 1];
			if( _ptr == NULL)
				throw std::bad_alloc(); //内存分配出错
			memcpy(_ptr, tmp, _length);  
			delete [] tmp;
		}
	}
	/*!
		禁止拷贝构造函数
	*/
	XStream(XStream &x);
};

} //namespace lang

#endif //_LANG_XSTREAM_H 
