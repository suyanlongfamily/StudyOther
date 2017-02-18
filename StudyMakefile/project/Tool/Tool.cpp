
#include "Tool.h"	   

namespace Tool{

int TAdd(int a, int b){

	return a+b;
}

int TSub(int a,int b){
	return a - b;
} 

void SellSort(int a[],int length){

	for (int d=length/2; d>0 ; d /= 2)//setup length
	{
		for (int i = 0;i < d ; ++i)//依次遍历每一个子序列
		{
			//针对每一个子序列内部排位置。
			for (int j = i + d; j < length ; j += d)
			{					
				int tmp = a[j];	//基准位值
				int k = j;//基准的位置索引
				//if (a[k] > tmp)
				//{	
					while (k >= 0 && a[k-d] > tmp)
					{
						a[k] = a[k-d];
						k -= d;						
					}	
				//} 
				a[k] = tmp;	//找出基准值的位置。
			}	
		}
	}


}
}


