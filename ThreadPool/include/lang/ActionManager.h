#ifndef _LANG_ACTIONMANAGER_H
#define _LANG_ACTIONMANAGER_H

#include <vector>
#include "Delegate.h"

namespace lang {

struct ActionMap {

	int			command;	//交易类型
	std::string trancode; 	//交易码;
	handle 		*action;	//交易处理
	short 		type;		//交易方式
};

class ActionManager {

public:
	ActionManager(){ }	//构造函数
	/*!
		注册Action
	*/
	void addAction(int command, std::string trancode, handle * handle) 
	{
		ActionMap *actionMap = new ActionMap;
		actionMap->command = command;
		actionMap->trancode = trancode;
		actionMap->action = handle; 
		actionList.push_back( actionMap );
	}

	/*!
		根据类型和TITLE,查找Action
	*/
	ActionMap *findAction(int command, std::string trancode ) 
	{
		for (int i = 0; i < actionList.size(); i ++) {
			if( actionList[i]->command == command && (actionList[i]->trancode.compare(trancode)== 0) )
				return actionList[i];
		}
		return NULL;
	}

private:
	std::vector<ActionMap*>actionList;	// action列表
};

} //namespace lang

#endif //_LANG_ACTIONMANAGER_H
