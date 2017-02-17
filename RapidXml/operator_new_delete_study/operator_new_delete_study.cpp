// operator_new_delete_study.cpp : �������̨Ӧ�ó������ڵ㡣
//


/************************************************************************/
/*    

new ��operator new �� placement new ����

������� operator new ���صĺ�����new�ؼ��־Ͳ���ʹ���ˡ����Ҫע�⡣
�Լ� operator new��������ʱ�����һ��������size_t size�����򶼲������سɹ���Ϊ�޷����롣
operator new��������ʱ�����һ��������size_t��

��1��new �����ܱ����أ�����Ϊ����һ�µġ����ȵ���operator new�����ڴ棬Ȼ����ù��캯����ʼ���Ƕ��ڴ档
new ��������ִ�й��̣�
1. ����operator new�����ڴ棻
2. ���ù��캯�����������
3. ������Ӧָ�롣

��2��operator new��Ҫʵ�ֲ�ͬ���ڴ������Ϊ��Ӧ������operator new��������new��
operator new����operator + һ�����ǿ������صġ��������û������operator new����ô���õľ���ȫ�ֵ�::operator new����ɶѵķ��䡣ͬ��operator new[]��operator delete��operator delete[]Ҳ�ǿ������صġ�

��3��placement new��ֻ��operator new���ص�һ���汾�������������ڴ棬ֻ�Ƿ���ָ���Ѿ�����õ�ĳ���ڴ��һ��ָ�롣��˲���ɾ����������Ҫ���ö��������������

����������Ѿ�������ڴ��д���һ������ʹ��newʱ�в�ͨ�ġ�Ҳ����˵placement new��������һ���Ѿ�����õ��ڴ��У�ջ���߶��У�����һ���µĶ���ԭ����void* pʵ���Ͼ���ָ��һ���Ѿ�����õ��ڴ滺�����ĵ��׵�ַ��


 (4) <new> ͷ�ļ��У������صĺ����б����Բ���Ҫ���Լ��������صģ�����

 Placement newʹ�ò���

 �ںܶ�����£�placement new��ʹ�÷�����������ͨ��new������ͬ�������ṩ������ʹ�ò��衣

 ��һ��  ������ǰ����

 �����ַ�ʽ��

 1.Ϊ�˱�֤ͨ��placement newʹ�õĻ�������memory alignment(�ڴ����)��ȷ׼����ʹ����ͨ��new�����������ڶ��Ͻ��з���
 class Task ;
 char * buff = new [sizeof(Task)]; //�����ڴ�
 (��ע��auto����static�ڴ沢�Ƕ���ȷ��Ϊÿһ�������������У����ԣ��㽫������placement newʹ�����ǡ�)

 2.��ջ�Ͻ��з���
 class Task ;
 char buf[N*sizeof(Task)]; //�����ڴ�

 3.����һ�ַ�ʽ������ֱ��ͨ����ַ��ʹ�á�(������������ĵ�ַ)
 void* buf = reinterpret_cast<void*> (0xF00F);

 �ڶ���������ķ���

 �ڸղ��ѷ���Ļ���������placement new������һ������
 Task *ptask = new (buf) Task

 ��������ʹ��

 ������ͨ��ʽʹ�÷���Ķ���

 ptask->memberfunction();

 ptask-> member;

 //...

 ���Ĳ������������

 һ����ʹ�������������������������������������������������ķ�ʽ��������������
 ptask->~Task(); //�������ڵ���������

 ���岽���ͷ�

 ����Է������û��沢��������һ���µĶ����ظ�����2��3��4������㲻�����ٴ�ʹ��������棬������������ͷ�����delete [] buf;

 �����κβ���Ϳ��ܵ�������ʱ��ı������ڴ�й¶���Լ����������벻��������������ȷʵ��Ҫʹ��placement new����������ѭ���ϵĲ��衣



*/
/************************************************************************/


#include <string>
//#include <cassert>
#include <iostream>

using namespace std;
class X
{
public:
	X() { cout<<"constructor of X"<<endl; }
	~X() { cout<<"destructor of X"<<endl;}

	//����Ǻ������Ƕ�ȫ�ֺ�������   
	void* operator new(size_t size)
	{
		//������� size �Ǳ����Լ�Ĭ�ϴ��ݹ����ģ�����ҪҲ��������ʾ���ݣ�����޷����롣
		cout<<"operator new size 00"<<size<<" with string "<<endl;
		return ::operator new(size);
	}

	void* operator new(size_t size,string str)
	{
		cout<<"operator new size "<<size<<" with string "<<str<<endl;
		return ::operator new(size);
	}

	void * operator new(size_t, void *_Where) _THROW0()
	{	// construct array with placement at _Where
		return (_Where);
	}

	void operator delete(void* pointee)
	{
		cout<<"operator delete"<<endl;
		::operator delete(pointee);
	}
private:
	int num;
};

int main(int argc, char * argv[]) {

	//fun(123.123);

	X* px12 = new X(); //������  void* operator new(size_t size)

	//X* px1 = new(sizeof(X));������� size �Ǳ����Լ�Ĭ�ϴ��ݹ����ģ�����ҪҲ��������ʾ���ݣ�����޷����롣

	X *px = new("A new class") X; //void* operator new(size_t size,string str)
	
	char memeX[sizeof(X)];

	X* iptr29 = new (memeX) X;	

	char mem[sizeof(int)];


	//�����˶��Ƶ� placement new��ֻ��operator new���ص�һ���汾�������������ڴ棬ֻ�Ƿ���ָ���Ѿ�����õ�ĳ���ڴ��һ��ָ�롣��˲���ɾ����������Ҫ���ö��������������
	//����������Ѿ�������ڴ��д���һ������ʹ��newʱ�в�ͨ�ġ�Ҳ����˵placement new��������һ���Ѿ�����õ��ڴ��У�ջ���߶��У�����һ���µĶ���
	//ԭ����void* pʵ���Ͼ���ָ��һ���Ѿ�����õ��ڴ滺�����ĵ��׵�ַ��
	//inline void *__CRTDECL operator new(size_t, void *_Where) _THROW0()
	//{	// construct array with placement at _Where
	//	return (_Where);
	//}
	//���Ҳ�����Լ��������صģ����ǲ��ܰ���ͷ�ļ�<new> ������ͻ�ġ������������ʱ��Ҳ�ǿ��Եģ�ֻҪ����std�ռ�����Ϳ����ˡ�	 
	//������ͻ�ģ���������һ������������Ҳ�ǿ��Եģ�����ֻ���й����X�������ʱ�򣬲Ż����X�����placement new��
	// new operator 
	int* iptr2 = new (mem) int;	
	//delete(iptr2,iptr2);       // Whoops, segmentation fault! �ذ����δ�������

	return 0;

}