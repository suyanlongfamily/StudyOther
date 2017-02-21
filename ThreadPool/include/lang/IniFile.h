#ifndef _LANG_INIFILE_H
#define _LANG_INIFILE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

namespace lang {

class TIniFile
{
private:
	std::ifstream infFile;
	std::vector<std::string> FList;	
	std::string FFileName;
	int FSectionIndex, FValueIndex;
	void InitList();
	bool FindValue(const std::string &Section, const std::string &Ident, std::string &value);

public:
	TIniFile(const std::string &filename);
	virtual ~TIniFile();

	/*!
		��ȡ[Section]��Ident��ֵ
	*/
	std::string ReadString(const std::string &Section, const std::string &Ident); 

	/*!
		��ȡ[Section]��Ident��ֵ,���û��ȡ��,�ͷ���Defaultֵ
	*/
	std::string ReadString(const std::string &Section, const std::string &Ident, 
			const std::string &Default);

	/*!
		��ȡ[Section]��Ident��ֵ,��ת��ΪInt
	*/
	int ReadInteger(const std::string &Section, const std::string &Ident);

	/*!
		��ȡ[Section]��Ident��ֵ,���û���ҵ�,�ͷ���Default
	*/
	int ReadInteger(const std::string &Section, const std::string &Ident, int Default);


	/*!
		����[Section]��Ident��ֵ,���Section����Ident������,�Զ�������
	*/
	void WriteInteger(const std::string &Section, const std::string &Ident, int value);

	/*!
		����[Section]��Ident��ֵ,���Section����Ident������,�Զ�������
	*/
	void WriteString(const std::string &Section, const std::string &Ident, 
			const std::string &Default);

	/*!
		�ҵ�section�µ�����ֵ
	*/
	std::map<std::string, std::string> &ReadSectionValues(const std::string &Section,
			std::map<std::string, std::string> &resmap); 

	/*!
		����ini�ļ�
	*/
	void SaveToFile();
};

} //namespace lang

#endif //_LANG_INIFILE_H
