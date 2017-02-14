//// rapidxml.cpp : 定义控制台应用程序的入口点。
////
//
//#include "stdafx.h"
//#include "stdlib.h"
//#include <iostream>
//
////下面三个文件是本段代码需要的库文件
//#include "rapidxml/rapidxml.hpp"       
//#include "rapidxml/rapidxml_utils.hpp"
//#include "rapidxml/rapidxml_print.hpp"
//
//int CreateXml();
//int ReadAndChangeXml();
//int _tmain(int argc, _TCHAR* argv[])
//{
//	int length = sizeof(void *);
//	//char a = '[';
//	//char b = char(']');
//
//	////测试用例
//	CreateXml();
//	//测试用例
//	//ReadAndChangeXml();
//	system("pause");
//	return 0;
//}
////创建一个名称为config2.xml文件
//int CreateXml()
//{
//	//节点、属性的创建都是靠xml_document 成员函数创建,主要用来管理节点、属性的指针，用来释放。
//	rapidxml::xml_document<> doc;
//	rapidxml::xml_node<>* rot = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
//	doc.append_node(rot);//整个文档就是节点
//	
//	rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element,"config","information");
//	doc.append_node(node);
//
//	rapidxml::xml_node<>* color = doc.allocate_node(rapidxml::node_element,"color",NULL);
//	node->append_node(color);
//	color->append_node(doc.allocate_node(rapidxml::node_element,"red","0.1"));
//	color->append_node(doc.allocate_node(rapidxml::node_element,"green","0.1"));
//	color->append_node(doc.allocate_node(rapidxml::node_element,"blue","0.1"));
//	color->append_node(doc.allocate_node(rapidxml::node_element,"alpha","1.0"));
//
//	rapidxml::xml_node<>* size = doc.allocate_node(rapidxml::node_element,"size",NULL);	 
//	size->append_node(doc.allocate_node(rapidxml::node_element,"x","640"));
//	size->append_node(doc.allocate_node(rapidxml::node_element,"y","480"));
//	node->append_node(size);
//
//	rapidxml::xml_node<>* mode = doc.allocate_node(rapidxml::node_element,"mode","screen mode");
//	mode->append_attribute(doc.allocate_attribute("fullscreen","false"));
//	node->append_node(mode);
//
//	std::string text;
//	rapidxml::print(std::back_inserter(text), doc, 0);
//	std::cout<<text<<std::endl;
//	std::ofstream out("../config.xml");
//	out << doc;
//	
//	return 0;
//}
//
//
////读取并修改config1.xml
//int ReadAndChangeXml()
//{
//	rapidxml::file<> fdoc("../config1.xml");
//	std::cout<<fdoc.data()<<std::endl;
//	rapidxml::xml_document<> doc;
//	
//	doc.parse<0>(fdoc.data());	  //模版偏特化！！
//	//doc.parse<1>(fdoc.data());
//	std::cout<<doc.name()<<std::endl;
//	//! 获取根节点
//	rapidxml::xml_node<>* root = doc.first_node();
//	std::cout<<root->name()<<std::endl;
//	//! 获取根节点第一个节点
//	rapidxml::xml_node<>* node1 = root->first_node();
//	std::cout<<node1->name()<<std::endl;
//	rapidxml::xml_node<>* node11 = node1->first_node();
//	std::cout<<node11->name()<<std::endl;
//	std::cout<<node11->value()<<std::endl;
//
//	//! 修改之后再次保存
//	rapidxml::xml_node<>* size = root->first_node("size");
//	size->append_node(doc.allocate_node(rapidxml::node_element,"w","1"));
//	size->append_node(doc.allocate_node(rapidxml::node_element,"h","1"));
//	std::string text;
//	rapidxml::print(std::back_inserter(text),doc,0);
//	std::cout<<text<<std::endl;
//	std::ofstream out("../config1.xml");
//	out << doc;
//	doc.clear();
//	return 0;
//}


#include "rapidxml/rapidxml.hpp"
#include <string>
#include <cassert>
#include <iostream>
int main(int argc, char * argv[]) {
	


	using namespace rapidxml;
	xml_document<> doc;
	try {
		char doc_text[] = "<single-element/>";
		doc.parse<0>(doc_text);

		auto node = doc.first_node();
		assert(std::string("single-element") == node->name());
		doc.validate();
	} catch(parse_error & e) {
		std::cout << "Parse error: " << e.what() << std::endl << "At: " << e.where<char>() << std::endl;
	} catch(validation_error & e) {
		std::cout << "Validation error: " << e.what() << std::endl;
	}
	try {
		char doc_text[] = "<pfx:single-element/>";
		doc.parse<0>(doc_text);

		auto node = doc.first_node();
		assert(std::string("single-element") == node->name());
		std::cout << "Expecting unbound prefix (element):\n";
		doc.validate();
	} catch(parse_error & e) {
		std::cout << "Parse error: " << e.what() << std::endl << "At: " << e.where<char>() << std::endl;
	} catch(validation_error & e) {
		std::cout << "Validation error: " << e.what() << std::endl;
	}
	try {
		char doc_text[] = "<single-element attr='one' attr=\"two\"/>";
		doc.parse<0>(doc_text);

		auto node = doc.first_node();
		assert(std::string("single-element") == node->name());
		std::cout << "Expecting doubled attribute: \n";
		doc.validate();
	} catch(parse_error & e) {
		std::cout << "Parse error: " << e.what() << std::endl << "At: " << e.where<char>() << std::endl;
	} catch(validation_error & e) {
		std::cout << "Validation error: " << e.what() << std::endl;
	}
	try {
		char doc_text[] = "<single-element pfx1:attr='one' attr=\"two\"/>";
		doc.parse<0>(doc_text);

		auto node = doc.first_node();
		assert(std::string("single-element") == node->name());
		auto attr = node->first_attribute();
		assert(attr->xmlns() == 0);
		std::cout << "Expecting unbound prefix (attr): \n";
		doc.validate();
	} catch(parse_error & e) {
		std::cout << "Parse error: " << e.what() << std::endl << "At: " << e.where<char>() << std::endl;
	} catch(validation_error & e) {
		std::cout << "Validation error: " << e.what() << std::endl;
	}
	try {
		char doc_text[] = "<single-element pfx1:attr='one' pfx2:attr=\"two\" xmlns:pfx1='urn:fish' xmlns:pfx2='urn:fish'/>";
		doc.parse<0>(doc_text);

		auto node = doc.first_node();
		assert(std::string("single-element") == node->name());
		std::cout << "Expecting doubled attr (XMLNS): \n";
		doc.validate();
	} catch(parse_error & e) {
		std::cout << "Parse error: " << e.what() << std::endl << "At: " << e.where<char>() << std::endl;
	} catch(validation_error & e) {
		std::cout << "Validation error: " << e.what() << std::endl;
	}
	try {
		char doc_text[] = "<pfx:single xmlns:pfx='urn:xmpp:example'/>";
		doc.parse<0>(doc_text);

		auto node = doc.first_node();
		std::cout << "Simple: <" << node->prefix() << ":" << node->name() << "/> " << node->xmlns() << std::endl;
		assert(std::string("single") == node->name());
		doc.validate();
	} catch(parse_error & e) {
		std::cout << "Parse error: " << e.what() << std::endl << "At: " << e.where<char>() << std::endl;
	} catch(validation_error & e) {
		std::cout << "Validation error: " << e.what() << std::endl;
	}
	try {
		char doc_text[] = "<pfx:single xmlns:pfx='urn:xmpp:example'><pfx:firstchild/><child xmlns='urn:potato'/><pfx:child/></pfx:single>";
		doc.parse<0>(doc_text);

		std::cout << "Test three, full parse" << std::endl;
		auto node = doc.first_node();
		//doc.fixup<0>(node, true);
		//std::cout << "<" << node->prefix() << ":" << node->name() << "/> " << node->xmlns() << std::endl;
		assert(std::string("single") == node->name());
		auto child = node->first_node(0, "urn:potato");
		assert(child);
		assert(std::string("child") == child->name());
		assert(std::string("urn:potato") == child->xmlns());
		child = node->first_node();
		assert(std::string("firstchild") == child->name());
		assert(std::string("urn:xmpp:example") == child->xmlns());
		//std::cout << "<" << node->prefix() << ":" << node->name() << "/> " << node->xmlns() << std::endl;
		child = node->first_node("child");
		assert(std::string("child") == child->name());
		assert(std::string("urn:xmpp:example") == child->xmlns());
		//std::cout << "<" << node->prefix() << ":" << node->name() << "/> " << node->xmlns() << std::endl;
		doc.validate();
	} catch(parse_error & e) {
		std::cout << "Parse error: " << e.what() << std::endl << "At: " << e.where<char>() << std::endl;
	} catch(validation_error & e) {
		std::cout << "Validation error: " << e.what() << std::endl;
	}

	try {
		char doc_text[] = "<pfx:single xmlns:pfx='urn:xmpp:example'><pfx:firstchild/><child xmlns='urn:potato'/><pfx:child/></pfx:single>";
		doc.parse<parse_fastest>(doc_text);

		std::cout << "Test three, fastest parse" << std::endl;
		auto node = doc.first_node();
		//doc.fixup<0>(node, true);
		//std::cout << "<" << node->prefix() << ":" << node->name() << "/> " << node->xmlns() << std::endl;
		assert("single" == std::string(node->name(), node->name_size()));
		assert("urn:xmpp:example" == std::string(node->xmlns(), node->xmlns_size()));
		auto child = node->first_node(0, "urn:potato");
		assert(child);
		assert("child" == std::string(child->name(), child->name_size()));
		assert("urn:potato" == std::string(child->xmlns(), child->xmlns_size()));
		child = node->first_node();
		assert("firstchild" == std::string(child->name(), child->name_size()));
		assert("urn:xmpp:example" == std::string(child->xmlns(), child->xmlns_size()));
		//std::cout << "<" << node->prefix() << ":" << node->name() << "/> " << node->xmlns() << std::endl;
		child = node->first_node("child");
		assert("child" == std::string(child->name(), child->name_size()));
		assert("urn:xmpp:example" == std::string(child->xmlns(), child->xmlns_size()));
		//std::cout << "<" << node->prefix() << ":" << node->name() << "/> " << node->xmlns() << std::endl;
		doc.validate();
	} catch(parse_error & e) {
		std::cout << "Parse error: " << e.what() << std::endl << "At: " << e.where<char>() << std::endl;
	} catch(validation_error & e) {
		std::cout << "Validation error: " << e.what() << std::endl;
	}
	try {
		char doc_text[] = "<pfx:single xmlns:pfx='urn:xmpp:example'><pfx:firstchild/><child xmlns='urn:potato'/><pfx:child/></pfx:single>";
		doc.parse<parse_fastest>(doc_text);

		std::cout << "Test three, fixup." << std::endl;
		auto node = doc.first_node();
		doc.fixup<0>(node, true);
		//std::cout << "<" << node->prefix() << ":" << node->name() << "/> " << node->xmlns() << std::endl;
		assert("single" == std::string(node->name(), node->name_size()));
		assert("urn:xmpp:example" == std::string(node->xmlns(), node->xmlns_size()));
		auto child = node->first_node(0, "urn:potato");
		assert(child);
		assert("child" == std::string(child->name(), child->name_size()));
		assert("urn:potato" == std::string(child->xmlns(), child->xmlns_size()));
		child = node->first_node();
		assert("firstchild" == std::string(child->name(), child->name_size()));
		assert("urn:xmpp:example" == std::string(child->xmlns(), child->xmlns_size()));
		//std::cout << "<" << node->prefix() << ":" << node->name() << "/> " << node->xmlns() << std::endl;
		child = node->first_node("child");
		assert("child" == std::string(child->name(), child->name_size()));
		assert("urn:xmpp:example" == std::string(child->xmlns(), child->xmlns_size()));
		//std::cout << "<" << node->prefix() << ":" << node->name() << "/> " << node->xmlns() << std::endl;
		doc.validate();
	} catch(parse_error & e) {
		std::cout << "Parse error: " << e.what() << std::endl << "At: " << e.where<char>() << std::endl;
	} catch(validation_error & e) {
		std::cout << "Validation error: " << e.what() << std::endl;
	}
	try {
		char doc_text[] = "<pfx:single xmlns:pfx='urn:xmpp:example'>";
		doc.parse<parse_open_only>(doc_text);

		auto node = doc.first_node();
		std::cout << "<" << node->prefix() << ":" << node->name() << "/> " << node->xmlns() << std::endl;
		assert(std::string("single") == node->name());
		doc.validate();
	} catch(parse_error & e) {
		std::cout << "Parse error: " << e.what() << std::endl << "At: " << e.where<char>() << std::endl;
	} catch(validation_error & e) {
		std::cout << "Validation error: " << e.what() << std::endl;
	}
	try {
		char doc_text[] = "<pfx:single xmlns='jabber:client' xmlns:pfx='urn:xmpp:example'><pfx:features><feature1/><feature2/></pfx:features><message to='me@mydomain.com' from='you@yourdomcina.com' xml:lang='en'><body>Hello!</body></message>";
		char * text = doc.parse<parse_open_only>(doc_text);

		auto node = doc.first_node();
		std::cout << "<" << node->prefix() << ":" << node->name() << "/> " << node->xmlns() << std::endl;
		assert(std::string("single") == node->name());
		std::cout << text << std::endl;
		doc.validate();
		while (*text) {
			xml_document<> subdoc;
			text = subdoc.parse<parse_parse_one>(text, doc);
			auto node = subdoc.first_node();
			auto xmlns = node->xmlns();
			std::cout << "<" << node->name() << " xmlns='" << node->xmlns() << "'/>" << std::endl;
			subdoc.validate();
		}
	} catch(parse_error & e) {
		std::cout << "Parse error: " << e.what() << std::endl << "At: " << e.where<char>() << std::endl;
	} catch(validation_error & e) {
		std::cout << "Validation error: " << e.what() << std::endl;
	}
	try {
		char doc_text[] = "<pfx:single xmlns:pfx='urn:xmpp:example'><pfx:features><feature1/><feature2/></p";
		char * text = doc.parse<parse_open_only|parse_fastest>(doc_text);

		auto node = doc.first_node();
		std::cout << "<" << node->prefix() << ":" << node->name() << "/> " << node->xmlns() << std::endl;
		assert(std::string("single") == std::string(node->name(), node->name_size()));
		std::cout << text << std::endl;
		doc.validate();
		while (*text) {
			xml_document<> subdoc;
			text = subdoc.parse<parse_parse_one>(text, doc);
			subdoc.validate();
			auto node = doc.first_node();
			std::cout << "<" << node->name() << " xmlns='" << node->xmlns() << "'/>" << std::endl;
		}
	} catch(parse_error & e) {
		std::cout << "Parse error: " << e.what() << std::endl << "At: " << e.where<char>() << std::endl;
	} catch(validation_error & e) {
		std::cout << "Validation error: " << e.what() << std::endl;
	}
	return 0;
}
