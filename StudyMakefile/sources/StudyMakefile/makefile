#########################################################################################################################
#											学习makefile文件
# 0、 对于变量，可以理解成宏
# 1、 “=” 是定义并对变量赋值。
# 2、 引用已经定义并赋值的变量，$(var) 形式引用，$(函数名 参数1,参数2...)函数也是这么调用的，通过指定函数名，以及实参。
# 3、 注意目标文件。
# 4、 %,* 这两个符号都是通配符,但是在一条规则中，不要同时使用这两个，很容易出问题！
# 5、 依赖文件、头文件依赖的实现，通过-MMD 选项，在编译生产的时候生产*.d文件，里面就是头文件的依赖关系列表（注意其思想！！！）。
# 6、 自动化编译时：*.d文件不再同一个文件里面问题的（cenos7）解决办法：添加 -MT $@ -MF $(patsubst %.cpp,%d $<)
# 7、 cenos7 里面也不是生产一个*.d文件，而是目标对应名字在后面加了一个d字母。
# 8、 子目录的支持：foreach 循环遍历。虽然类是for循环，但是这个是一个函数。函数！！！！
#########################################################################################################################
# 9、wildcard： 列出指定的文件列表。
# 10、foreach的调用： $(foreach 迭代器,迭代器要遍历的列表（容器）,对当前迭代器的操作)
# 11、类是多个值（字符串数组）的，值与值之间空格分开。
# 12、脚本是动态类型的，意味着变量也是动态性的。

EXE=StudyMakefile 

#要包含所有要编译源文件的目录，也要包括main函数文件所在的那个目录。
SUBDIR=./ ./Tool 

#所有Cpp文件。
#DIRTREE=$(foreach dir,$(SUBDIR),$(wildcard $(dir)/*.cpp))
#SOURCESFILE=$(wildcard *.cpp)

#列出所有的cpp文件。
SOURCESFILE=$(foreach dir,$(SUBDIR),$(wildcard $(dir)/*.cpp))

#对应的.o文件。
OBJECTFILE=$(patsubst %.cpp, %.o, $(SOURCESFILE))

#依赖文件、头文件
DEPFILE=$(patsubst %.o, %d, $(OBJECTFILE))

# 生成目标文件
$(EXE): $(OBJECTFILE) 
	g++ $(OBJECTFILE) -o $(EXE)
	
# 如果有很多的选项，是怎么实现循环，或者说是什么实现一一对应的？？？。
%.o:%.cpp
	#g++ -c -MMD $< -o $@
	#cenos7 里面需要添加一段指令这个才能显示*.d 文件。
	g++ -c -MMD -MT $@ -MF $(patsubst %.cpp,%d,$<) $< -o $@

-include $(DEPFILE)
	
test:
	echo $(EXE) 
	
test1:
	echo $(SOURCESFILE)
	
test2:
	echo $(OBJECTFILE)	

clean:
	rm -rf  *.d *.o *.i $(OBJECTFILE) $(DEPFILE) $(EXE) 
