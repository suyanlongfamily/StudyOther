#ifndef _LANG_LIST_H
#define _LANG_LIST_H

#include <vector>
#include "lang/XStream.h"

namespace lang {

template<class T>
class List {
public:
	typedef typename std::vector<T>::iterator iterator;

	List() {}
	virtual ~List() {}

	inline iterator begin() {
		return data.begin();
	}
	inline iterator end() {
		return data.end();
	}
	inline void resize(int sz) {
		data.resize(sz);
	}
	inline void reserve(int sz) {
		data.reserve(sz);
	}	
	inline void erase(int sz) {
		iterator iter = data.begin()+sz;
		data.erase(iter);
	}
	inline int size() {
		return data.size();
	}
	inline T& operator[](int index) {
		return data[index];
	}
	inline void add(T &t) {
		data.push_back(t);
	}
	inline void clear() {
		data.clear();
	}
	inline XStream & serialize(XStream &ar) {
		unsigned int count = data.size();
		ar << count;
		for(unsigned  int i = 0; i < count ; i ++)
			ar << data[i];
		return ar;
	}
	inline void deserialize(XStream &ar) {
		unsigned int count = 0;	
		ar >> count;
		data.resize(count);
		for(unsigned int i = 0; i < count; i ++) {
			ar >> data[i];
		}
	}

private:
	std::vector<T> data;
}; //namespace lang

}
#endif //_LANG_LIST_H
