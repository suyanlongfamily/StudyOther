// cache.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include "lang/DateUtil.h"

using namespace lang;
using namespace std;

typedef struct 
{
	int key;
	int values;
	int beginTime;
	int endTime;
	int count;//使用次数。
} PackData;

int arry[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};	
#define KEY
class TCache 
{
public:
	TCache(int size){
		tcontianPack.clear();
		tsize = size;
	}
	KEY void AddElment(int value);
	bool Find(int value);

protected:
private:
	std::vector<PackData> tcontianPack;
	std::vector<PackData>::size_type tsize;//大小	
};


void TCache::AddElment(int value)
{
	PackData p;
	p.beginTime = lang::TDateUtil::GetLocalTime();
	p.count = 0;
	p.values = value;
	//p.key = ;
	if(tcontianPack.size() > tsize)
	{
		tcontianPack.erase(tcontianPack.begin());
	}
	tcontianPack.insert(tcontianPack.end(),p); 	
}

bool TCache::Find(int value)
{	 
	std::vector<PackData>::iterator iter = tcontianPack.begin();
	for (;iter != tcontianPack.end();++iter)
	{
	    if (iter->values == value)
	    {
			iter->count++;
			cout<<"次数："<<iter->count<<endl;
			return true;
	    }
	}
	return false;	
}

int _tmain(int argc, _TCHAR* argv[])
{
	int k = 0; 
	TCache cache(6);
	cout<<"输入一个数："<<endl;
	cin>>k;
	for (int j = 1;k + j <= 19 && j <= 5; ++j )
	{  			
		cache.AddElment(k+j);
	}

	while (k != -1)//等于-1时退出
	{
		cin>>k;

		if (cache.Find(k))
		{
			cout<<"缓存中找到！"<<endl;
		}else
		{
			cout<<"到数据库中找！"<<endl; 
			int values;
			int count = sizeof(arry);
			for(int i = 0; i < count ;++i)
			{
				if (k == arry[i])
				{
					cache.AddElment(k);
					cout<<"找到并插入到缓存中"<<endl;
				}
			}
		}	
	}		
	system("pause");
	return 0;
}

