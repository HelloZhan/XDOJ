# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (xdoj)

set(CMAKE_CXX_STANDARD 11)

# 查找库的路径
link_directories(${CMAKE_SOURCE_DIR}/../lib)

# 查找头文件的路径
include_directories(../include)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(xdoj ${DIR_SRCS})

# 链接库
target_link_libraries(xdoj jsoncpp pthread mysqlclient judger)

# 指定编译参数
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

