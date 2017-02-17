// operator_new_delete_study.cpp : 定义控制台应用程序的入口点。
//


/************************************************************************/
/*    

new 、operator new 和 placement new 区别！

如果存在 operator new 重载的函数，new关键字就不能使用了。这个要注意。
以及 operator new函数重载时必须第一个参数是size_t size，否则都不能重载成功，为无法编译。
operator new函数重载时必须第一个参数是size_t”

（1）new ：不能被重载，其行为总是一致的。它先调用operator new分配内存，然后调用构造函数初始化那段内存。
new 操作符的执行过程：
1. 调用operator new分配内存；
2. 调用构造函数生成类对象；
3. 返回相应指针。

（2）operator new：要实现不同的内存分配行为，应该重载operator new，而不是new。
operator new就像operator + 一样，是可以重载的。如果类中没有重载operator new，那么调用的就是全局的::operator new来完成堆的分配。同理，operator new[]、operator delete、operator delete[]也是可以重载的。

（3）placement new：只是operator new重载的一个版本。它并不分配内存，只是返回指向已经分配好的某段内存的一个指针。因此不能删除它，但需要调用对象的析构函数。

如果你想在已经分配的内存中创建一个对象，使用new时行不通的。也就是说placement new允许你在一个已经分配好的内存中（栈或者堆中）构造一个新的对象。原型中void* p实际上就是指向一个已经分配好的内存缓冲区的的首地址。


 (4) <new> 头文件中，是重载的函数列表。可以不需要，自己进行重载的！！！

 Placement new使用步骤

 在很多情况下，placement new的使用方法和其他普通的new有所不同。这里提供了它的使用步骤。

 第一步  缓存提前分配

 有三种方式：

 1.为了保证通过placement new使用的缓存区的memory alignment(内存队列)正确准备，使用普通的new来分配它：在堆上进行分配
 class Task ;
 char * buff = new [sizeof(Task)]; //分配内存
 (请注意auto或者static内存并非都正确地为每一个对象类型排列，所以，你将不能以placement new使用它们。)

 2.在栈上进行分配
 class Task ;
 char buf[N*sizeof(Task)]; //分配内存

 3.还有一种方式，就是直接通过地址来使用。(必须是有意义的地址)
 void* buf = reinterpret_cast<void*> (0xF00F);

 第二步：对象的分配

 在刚才已分配的缓存区调用placement new来构造一个对象。
 Task *ptask = new (buf) Task

 第三步：使用

 按照普通方式使用分配的对象：

 ptask->memberfunction();

 ptask-> member;

 //...

 第四步：对象的析构

 一旦你使用完这个对象，你必须调用它的析构函数来毁灭它。按照下面的方式调用析构函数：
 ptask->~Task(); //调用外在的析构函数

 第五步：释放

 你可以反复利用缓存并给它分配一个新的对象（重复步骤2，3，4）如果你不打算再次使用这个缓存，你可以象这样释放它：delete [] buf;

 跳过任何步骤就可能导致运行时间的崩溃，内存泄露，以及其它的意想不到的情况。如果你确实需要使用placement new，请认真遵循以上的步骤。



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

	//这个是函数，是对全局函数重载   
	void* operator new(size_t size)
	{
		//首先这个 size 是编译自己默认传递过来的，不需要也不能做显示传递，否次无法编译。
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

	X* px12 = new X(); //调用了  void* operator new(size_t size)

	//X* px1 = new(sizeof(X));首先这个 size 是编译自己默认传递过来的，不需要也不能做显示传递，否次无法编译。

	X *px = new("A new class") X; //void* operator new(size_t size,string str)
	
	char memeX[sizeof(X)];

	X* iptr29 = new (memeX) X;	

	char mem[sizeof(int)];


	//调用了定制的 placement new：只是operator new重载的一个版本。它并不分配内存，只是返回指向已经分配好的某段内存的一个指针。因此不能删除它，但需要调用对象的析构函数。
	//如果你想在已经分配的内存中创建一个对象，使用new时行不通的。也就是说placement new允许你在一个已经分配好的内存中（栈或者堆中）构造一个新的对象。
	//原型中void* p实际上就是指向一个已经分配好的内存缓冲区的的首地址。
	//inline void *__CRTDECL operator new(size_t, void *_Where) _THROW0()
	//{	// construct array with placement at _Where
	//	return (_Where);
	//}
	//这个也可以自己从新重载的，但是不能包含头文件<new> 否则会冲突的。存在作用域的时候也是可以的，只要不是std空间里面就可以了。	 
	//否则会冲突的！就像上面一样，在类里面也是可以的，不过只能有关这个X类操作的时候，才会调用X定义的placement new。
	// new operator 
	int* iptr2 = new (mem) int;	
	//delete(iptr2,iptr2);       // Whoops, segmentation fault! 呜啊，段错误啦！

	return 0;

}