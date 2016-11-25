// StudyMakefile.cpp : 定义控制台应用程序的入口点。
//


#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "list"
#include "./Tool/Tool.h"


using namespace std;
using namespace Tool;

int main(int argc, char* argv[])
{
	
	int ret = Tool::TAdd(2,4);
	cerr<< "输出值2222222222222222222222222222222222222222222：" << ret <<endl; 
	//flush(cerr);
	printf("----%d\n",ret);
	//system("pause");
	getchar();
	return 0;
}

