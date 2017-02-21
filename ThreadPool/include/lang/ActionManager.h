#ifndef _LANG_ACTIONMANAGER_H
#define _LANG_ACTIONMANAGER_H

#include <vector>
#include "Delegate.h"

namespace lang {

struct ActionMap {

	int			command;	//��������
	std::string trancode; 	//������;
	handle 		*action;	//���״���
	short 		type;		//���׷�ʽ
};

class ActionManager {

public:
	ActionManager(){ }	//���캯��
	/*!
		ע��Action
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
		�������ͺ�TITLE,����Action
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
	std::vector<ActionMap*>actionList;	// action�б�
};

} //namespace lang

#endif //_LANG_ACTIONMANAGER_H
