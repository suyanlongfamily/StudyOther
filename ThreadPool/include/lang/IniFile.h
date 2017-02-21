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
		读取[Section]中Ident的值
	*/
	std::string ReadString(const std::string &Section, const std::string &Ident); 

	/*!
		读取[Section]中Ident的值,如果没有取到,就返回Default值
	*/
	std::string ReadString(const std::string &Section, const std::string &Ident, 
			const std::string &Default);

	/*!
		读取[Section]中Ident的值,并转换为Int
	*/
	int ReadInteger(const std::string &Section, const std::string &Ident);

	/*!
		读取[Section]中Ident的值,如果没有找到,就返回Default
	*/
	int ReadInteger(const std::string &Section, const std::string &Ident, int Default);


	/*!
		更新[Section]中Ident的值,如果Section或者Ident不存在,自动会增加
	*/
	void WriteInteger(const std::string &Section, const std::string &Ident, int value);

	/*!
		更新[Section]中Ident的值,如果Section或者Ident不存在,自动会增加
	*/
	void WriteString(const std::string &Section, const std::string &Ident, 
			const std::string &Default);

	/*!
		找到section下的所有值
	*/
	std::map<std::string, std::string> &ReadSectionValues(const std::string &Section,
			std::map<std::string, std::string> &resmap); 

	/*!
		保存ini文件
	*/
	void SaveToFile();
};

} //namespace lang

#endif //_LANG_INIFILE_H
