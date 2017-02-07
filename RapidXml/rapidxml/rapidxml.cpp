// rapidxml.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdlib.h"
#include <iostream>

//下面三个文件是本段代码需要的库文件
#include "rapidxml/rapidxml.hpp"       
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

int CreateXml();
int ReadAndChangeXml();
int _tmain(int argc, _TCHAR* argv[])
{
	int length = sizeof(void *);
	//char a = '[';
	//char b = char(']');
	////测试用例
	//CreateXml();
	//测试用例
	ReadAndChangeXml();
	system("pause");
	return 0;
}
//创建一个名称为config2.xml文件
int CreateXml()
{
	//节点、属性的创建都是靠xml_document 成员函数创建,主要用来管理节点、属性的指针，用来释放。
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<>* rot = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
	doc.append_node(rot);
	
	rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element,"config","information");
	doc.append_node(node);

	rapidxml::xml_node<>* color = doc.allocate_node(rapidxml::node_element,"color",NULL);
	node->append_node(color);

	color->append_node(doc.allocate_node(rapidxml::node_element,"red","0.1"));
	color->append_node(doc.allocate_node(rapidxml::node_element,"green","0.1"));
	color->append_node(doc.allocate_node(rapidxml::node_element,"blue","0.1"));
	color->append_node(doc.allocate_node(rapidxml::node_element,"alpha","1.0"));

	rapidxml::xml_node<>* size = doc.allocate_node(rapidxml::node_element,"size",NULL);

	size->append_node(doc.allocate_node(rapidxml::node_element,"x","640"));
	size->append_node(doc.allocate_node(rapidxml::node_element,"y","480"));
	node->append_node(size);

	rapidxml::xml_node<>* mode = doc.allocate_node(rapidxml::node_element,"mode","screen mode");
	mode->append_attribute(doc.allocate_attribute("fullscreen","false"));
	node->append_node(mode);

	std::string text;
	rapidxml::print(std::back_inserter(text), doc, 0);
	std::cout<<text<<std::endl;
	std::ofstream out("../config1.xml");
	out << doc;

	return 0;
}


//读取并修改config3.xml
int ReadAndChangeXml()
{
	rapidxml::file<> fdoc("../config.xml");
	std::cout<<fdoc.data()<<std::endl;
	rapidxml::xml_document<> doc;
	
	doc.parse<0>(fdoc.data());	  //模版偏特化！！
	//doc.parse<1>(fdoc.data());
	std::cout<<doc.name()<<std::endl;
	//! 获取根节点
	rapidxml::xml_node<>* root = doc.first_node();
	std::cout<<root->name()<<std::endl;
	//! 获取根节点第一个节点
	rapidxml::xml_node<>* node1 = root->first_node();
	std::cout<<node1->name()<<std::endl;
	rapidxml::xml_node<>* node11 = node1->first_node();
	std::cout<<node11->name()<<std::endl;
	std::cout<<node11->value()<<std::endl;

	//! 修改之后再次保存
	rapidxml::xml_node<>* size = root->first_node("size");
	size->append_node(doc.allocate_node(rapidxml::node_element,"w","1"));
	size->append_node(doc.allocate_node(rapidxml::node_element,"h","1"));
	std::string text;
	rapidxml::print(std::back_inserter(text),doc,0);
	std::cout<<text<<std::endl;
	std::ofstream out("../config1.xml");
	out << doc;
	doc.clear();
	//doc.clear();
	return 0;
}