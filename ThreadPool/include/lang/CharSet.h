#ifndef _LANG_CHARSET_H
#define _LANG_CHARSET_H

#include <list>
#include <sstream>
#include <limits>
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <string> 
#include <memory.h>
#include "Exception.h"


namespace lang {

class TCharSet
{
public:
	/*!
		�޳�ǰ��ո� 
	*/
	static char* TrimLeft(char *strbuf) {
		int len = strlen(strbuf);
		if (len <= 0)
			return strbuf;
		for (int i = 0; i < len; i ++) {
			if(!isspace(strbuf[i]))
				return &strbuf[i];
		}
		return &strbuf[len];
	}

	static std::string &TrimLeft(std::string &s) {
		std::string::iterator iter = s.begin();
		for (; iter != s.end(); iter++) {
			if( *iter != ' ')
				break;
		}
		std::copy(iter, s.end(), s.begin());
		s.resize(s.end() - iter);
		return s;
	}

	/*!
		�޳�����ո�
	*/
	static char* TrimRight(char *strbuf) {
		int len = strlen(strbuf);
		if( len <= 0)
			return strbuf;
		while (len-- && isspace(strbuf[len]));
		strbuf[len+1] = '\0';
		return strbuf;
	}

	static std::string &TrimRight(std::string &s) {
		std::string::iterator iter = s.end();
		for (; iter != s.begin(); ) {
			if (*(--iter) != ' ')
				break;
		}
		s.resize(iter - s.begin() + 1);
		return s;
	}

	/*!
		�޳�ǰ��ո�
	*/
	static char* Trim(char *strbuf) {
		TrimRight(strbuf);
		return TrimLeft(strbuf);
	}

	static std::string& Trim(std::string &s) {
		TrimRight(s);
		return TrimLeft(s);
	}

	/*!
		�ж��ַ�����ǰnλ�Ƿ�������
	*/
	static bool isndigit(char *s, int n) {
		if (s == 0)
			return  false;
		int i = 0;
		while (isdigit(s[i]) && ++i < n);
		return i>=n;
	}

	/*!
		��ʽ���ַ�������
	*/
	static std::string Format(const char *fmt, ...) {
		char buf[1024];
		memset(buf, 0, 1024);
		va_list ap;
		va_start(ap, fmt);
		_vsnprintf_s(buf, sizeof(buf) - 1, fmt, ap);
		//_vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
		buf[1024-1] = '\0';
		va_end(ap);
		return std::string(buf);
	}

	/*!
		�Լ��ܵ��ַ�������
	*/
	static std::string Decode(const std::string &text) {
		int length = text.size();
		std::string returnstr(length/2, '0');
		int i = 0;
		while (i < length)
		{
			if (text[i] < 33 || text[i] > 126)
				throw Exception("ֻ�����ַ���������");
			if (text[i+1] >= 'A' && text[i+1] < 'a')
				returnstr[i/2] = text[i] - i/2;
			else if (text[i+1] >= 'a')
				returnstr[i/2] = text[i] + i/2;
			i = i + 2;
		}
		return returnstr;
	}

	/*!
		���ַ������м���
	*/
	static std::string Encode(const std::string &text) {
		int length = text.size();
		std::string returnstr(2 * length, '0');
		for (int i = 0; i < length; i ++)
		{
			if (text[i] < 33 || text[i] > 126)
				throw Exception("ֻ�����ַ���������");
			if (text[i] + i > 126)
			{
				returnstr[2*i] = text[i] - i;
				returnstr[2*i+1] = 'a' + i;
			}
			else
			{
				returnstr[2*i] = text[i] + i;
				returnstr[2*i+1] = 'A' + i;
			}
		}
		return returnstr;
	}

	/*!
		�ַ����滻
	*/
	static void Replace(std::string &_big, const std::string &_src, const std::string &_dst) {
		std::string::size_type pos = 0;
		std::string::size_type srclen = _src.size();
		std::string::size_type dstlen = _dst.size();
		while ((pos=_big.find(_src, pos)) != std::string::npos)
		{
			_big.replace(pos, srclen, _dst);
			pos += dstlen;
		}
	}

	/*!
		����ַ���
	*/
	static void Split(const std::string &str, std::list<std::string> &_list, char sep) {
		std::string s = str;
		if (str.length() == 0)
			return;
		if (s[s.length()-1] != sep)
			s.append(1, sep);

		while (s.length())
		{
			std::string subs = s.substr(0, s.find(sep, 1));
			s = s.substr(subs.length() + 1, s.length());
			_list.push_back(subs);
		}
	}


	/*!
		ת��ΪСд
	*/
	static std::string Lower(std::string &text) {
		for (unsigned int i = 0; i < text.length(); i ++)
		{
			if (text[i] >= 'A' && text[i] <= 'Z')
				text[i] = text[i] + 32;
		}
		return text;
	}


	/*!
		ת��Ϊ��д
	*/
	static std::string Upper(std::string &text) {
		for (unsigned int i = 0; i < text.length(); i ++)
		{
			if (text[i] >= 'a' && text[i] <= 'z')
				text[i] = text[i] - 32;
		}
		return text;
	}


	/*!
		�ǲ��ǿո�
	*/
	static bool IsWhite(const std::string &text) {
		for (unsigned int i = 0; i < text.length(); i++)
		{
			if (!isspace(text[i]))
				return false;
		}
		return true;
	}


	/*!
		��ȡ�ַ��� count��0��ʼ
	*/
	static std::string ExtractStr(const std::string &str, char splitchar, int count) {
		std::string outstr(str);
		std::string::size_type pos = 0;
		std::string::size_type lastpos = 0;
		int index = 0;
		if (str.empty())
			return str;
		if (outstr[str.length()-1] == splitchar)
			outstr = outstr.substr(0, str.length() - 1);

		while ((pos=outstr.find(splitchar, lastpos)) != std::string::npos)
		{
			if (index == count)
				return outstr.substr(lastpos, pos - lastpos);
			index ++;
			lastpos = pos+1;
		}
		return outstr.substr(lastpos, outstr.length() - lastpos);
	}



	/*!
		�Ƿ�������
	*/
	static bool isAllDigit(const std::string &str) {
		for (unsigned int i = 0; i < str.length(); i++)
		{
			if (str[i] - '0' < 0 || str[i] - '0' > 9)
				return false;
		}
		return true;
	}

};

} //namespace lang

#endif //_LANG_CHARSET_H
