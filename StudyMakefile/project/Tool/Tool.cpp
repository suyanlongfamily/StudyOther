
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
		for (int i = 0;i < d ; ++i)//���α���ÿһ��������
		{
			//���ÿһ���������ڲ���λ�á�
			for (int j = i + d; j < length ; j += d)
			{					
				int tmp = a[j];	//��׼λֵ
				int k = j;//��׼��λ������
				//if (a[k] > tmp)
				//{	
					while (k >= 0 && a[k-d] > tmp)
					{
						a[k] = a[k-d];
						k -= d;						
					}	
				//} 
				a[k] = tmp;	//�ҳ���׼ֵ��λ�á�
			}	
		}
	}


}
}


