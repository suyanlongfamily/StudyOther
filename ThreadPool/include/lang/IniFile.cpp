#include <errno.h>
#include <stdlib.h>
#include "lang/IniFile.h"
#include "lang/CharSet.h"
#include "lang/Lex.h"

namespace lang {

TIniFile::TIniFile(const std::string &filename) : FFileName(filename)
{
	infFile.open(FFileName.c_str());
	if (infFile.fail())
	{
		int _err = errno;
		std::string _etext = TCharSet::Format("%s打开失败[errno=%d]",FFileName.c_str(),_err);
		throw Exception(_etext);
	}
	InitList();
}

TIniFile::~TIniFile()
{
	infFile.close();
}

void TIniFile::InitList()
{
	char buff[1024];
	FList.clear();
	infFile.seekg(0);
	while (!infFile.eof())
	{
		memset(buff,0,1024);
		infFile.getline(buff,1024);
		//这个读取的字符如果为中文，会出现乱码，需要转换一下。
		//要么配置文件里面不能有中文。
		//std::string stringLine(buff,1024);
		FList.push_back(TCharSet::Trim(buff));
	}
}

bool TIniFile::FindValue(const std::string &Section, const std::string &Ident, std::string &value)
{
	std::vector<std::string>::iterator iter = FList.begin();
	std::string LineText, _section;
	bool isFound = false;
	int index = -1;
	_section.append("[").append(Section).append("]");
	//在WRITE的时候要用到
	FValueIndex = -1, FSectionIndex = -1; 
	
	for (iter = FList.begin(); iter != FList.end(); ++iter)
	{
		index ++;
		LineText = (*iter).c_str();
		if (LineText.length() == 0 || LineText[0] == ';' || LineText[0] == '\n')
			continue;
		
		if (isFound == false && !LineText.compare(_section))
		{
			isFound = true;
			FSectionIndex = index;
			continue;
		}

		if (isFound)
		{
			if (LineText[0] == '[')
				break;
			else 
			{
				std::string::size_type loc = LineText.find_first_of("=");
				if (loc != std::string::npos)
				{
					std::string left = LineText.substr(0, loc);
					if (!left.compare(Ident)) 
					{
						FValueIndex = index;
						std::string strbuf = LineText.substr(loc + 1, LineText.length() - loc - 1);
						value = TCharSet::Trim(strbuf);
						return true;
					}
				}
			}
		}
	}
	return false;
}

std::map<std::string, std::string> &TIniFile::ReadSectionValues(const std::string &Section,
			std::map<std::string, std::string> &resmap)
{
	std::vector<std::string>::iterator iter = FList.begin();
	std::string LineText, _section;
	bool isFound = false;
	_section.append("[").append(Section).append("]");
	
	for (iter = FList.begin(); iter != FList.end(); ++iter)
	{
		LineText = *iter;
		if (LineText.length() == 0 || LineText[0] == ';' || LineText[0] == '\n')
			continue;
		
		if (isFound == false && !LineText.compare(_section))
		{
			isFound = true;
			continue;
		}

		if (isFound)
		{
			if (LineText[0] == '[')
				break;
			else 
			{
				std::string::size_type loc = LineText.find_first_of("=");
				if (loc != std::string::npos)
				{
					std::string left = LineText.substr(0, loc);
					resmap[left] = LineText.substr(loc + 1, LineText.length() - loc - 1);
				}
				else
					resmap[LineText] = "";
			}
		}
	}
	return resmap;
}

void TIniFile::SaveToFile()
{
	infFile.close();
	std::ofstream _savefile(FFileName.c_str());
	if (_savefile.fail())
	{
		int _err = errno;
		std::string _etext = TCharSet::Format("%s打开失败[errno=%d]",FFileName.c_str(),_err);
		throw Exception(_etext);
	}

	std::vector<std::string>::iterator iter;
	for (iter = FList.begin(); iter != FList.end(); ++iter)
	{
		std::string str = (*iter).c_str();
		if (iter == FList.begin() )
			_savefile << (*iter);
		else
			_savefile << '\n' << (*iter);
	}
	_savefile.close();
}

std::string TIniFile::ReadString(const std::string &Section, const std::string &Ident) 
{
	std::string value;
	if (FindValue(Section, Ident, value))
		return value;
	else
	{
		std::string _etext = TCharSet::Format("没有找到[%s]中%s的值",Section.c_str(),Ident.c_str());
		throw Exception(_etext);
	}
}

std::string TIniFile::ReadString(const std::string &Section, const std::string &Ident, 
		const std::string &Default)
{
	std::string value;
	if (FindValue(Section, Ident, value))
		return value;
	else
		return Default;
}

int TIniFile::ReadInteger(const std::string &Section, const std::string &Ident)
{
	std::string value;
	if (FindValue(Section, Ident, value))
		return atoi(value.c_str());
	else
	{
		std::string _etext = TCharSet::Format("没有找到[%s]中%s的值",Section.c_str(),Ident.c_str());
		throw Exception(_etext);
	}
}

int TIniFile::ReadInteger(const std::string &Section, const std::string &Ident, int Default)
{
	std::string value;
	if (FindValue(Section, Ident, value))
		Default = lang::lex_cast<int>(value);
	return Default;
}

void TIniFile::WriteInteger(const std::string &Section, const std::string &Ident, int value)
{
	std::string str = TCharSet::Format("%d",value);
	WriteString(Section, Ident, str); 
}

void TIniFile::WriteString(const std::string &Section, const std::string &Ident, 
		const std::string &Default)
{
	std::string str;
	std::vector<std::string>::iterator iter = FList.begin();

	if (FindValue(Section, Ident, str) && FValueIndex > -1)
	{
		str = FList[FValueIndex];
		std::string::size_type loc = str.find_first_of("=");
		FList[FValueIndex] = str.substr(0, loc) + "=" + Default;
	}
	else
	{
		str = Ident + "=" + Default;
		if (FSectionIndex == -1)
		{
			FList.push_back("");
			FList.push_back("[" + Section + "]");
			FList.push_back(str);
		}
		else
		{
			FList.insert(iter+FSectionIndex+1, str);
		}
	}
}

}
