// cache.cpp : �������̨Ӧ�ó������ڵ㡣
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
	int count;//ʹ�ô�����
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
	std::vector<PackData>::size_type tsize;//��С	
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
			cout<<"������"<<iter->count<<endl;
			return true;
	    }
	}
	return false;	
}

int _tmain(int argc, _TCHAR* argv[])
{
	int k = 0; 
	TCache cache(6);
	cout<<"����һ������"<<endl;
	cin>>k;
	for (int j = 1;k + j <= 19 && j <= 5; ++j )
	{  			
		cache.AddElment(k+j);
	}

	while (k != -1)//����-1ʱ�˳�
	{
		cin>>k;

		if (cache.Find(k))
		{
			cout<<"�������ҵ���"<<endl;
		}else
		{
			cout<<"�����ݿ����ң�"<<endl; 
			int values;
			int count = sizeof(arry);
			for(int i = 0; i < count ;++i)
			{
				if (k == arry[i])
				{
					cache.AddElment(k);
					cout<<"�ҵ������뵽������"<<endl;
				}
			}
		}	
	}		
	system("pause");
	return 0;
}

