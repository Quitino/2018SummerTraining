CMAKE_MINIMUM_REQUIRED(VERSION 2.6) #CMake最低版本要求

PROJECT(CircleDetect) #定义工程名

add_definitions(-std=c++11)
SET(CMAKE_BUILE_TYPE Realease) #指定编译类型，Debug或Realease
			    #Debug会生成相关调试信息，可以使用GDB进行
			    #Realease不会生成调试信息。当无法进行调试时可查看此处是否设置为Debug。

SET(CMAKE_CXX_FLAGS_DEBUG "-g-Wall")  #指定编译器
				     # CMAKE_C_FLAGS_DEBUG —— C 编译器
				     # CMAKE_CXX_FLAGS_DEBUG —— C++ 编译器
				     #-g:只是编译器，在编译的时候，产生调试信息。
				     #-Wall：生成所有警告信息。
#ADD_SUBDIRECTORY(utility)  #添加子目录

FIND_PACKAGE(OpenCV REQUIRED)  #查找opencv包

if(WIN32)
ADD_EXECUTABLE(CircleDetect main.cpp CircleDetec.cpp SerialPort.cpp DataProcess.cpp )
else()
ADD_EXECUTABLE(CircleDetect main.cpp CircleDetec.cpp ManiFold_UART1.cpp DataProcess.cpp )  #这里括号里面的两个参数分别是工程项目名和我们要编译文件名
endif()

include_directories(${OpenCV_INCLUDE_DIRS})			#头文件
TARGET_LINK_LIBRARIES(CircleDetect ${OpenCV_LIBS})  #链接到OpenCV库，第一个参数为工程项目名

