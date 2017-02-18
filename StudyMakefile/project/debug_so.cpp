#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h> 
#include "Tool/Tool.h"	 

using namespace std;
using namespace Tool;
int main(){	  
	
	while (1)
	{
		cout<<"程序开始部分---------------------"<<endl;
		sleep(10);
		int a = 0;
		int b = 3;
		int ret = TAdd(a,b); 
		ret = TSub(a,b);
		int arry[] = {12,43,2,1,54,3,6,4,5,7,2,6,43,235,567,213};
		//SellSort(arry,sizeof(arry)/sizeof(arry[0]));
		cout<<"程序开始结束---------------------"<<endl;
		
	}
	return 0;  
}

