# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (xdoj)

set(CMAKE_CXX_STANDARD 17)

# 查找库的路径
link_directories(${CMAKE_SOURCE_DIR}/lib)

# 查找头文件的路径
include_directories(./include)
# 查找头文件的路径 mongocxx
include_directories(/usr/local/include/mongocxx/v_noabi /usr/local/include/libmongoc-1.0)
include_directories(/usr/local/include/bsoncxx/v_noabi /usr/local/include/libbson-1.0)
# 查找头文件的路径 redis-plus-plus
include_directories(hiredis sw)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src DIR_SRCS)
aux_source_directory(./src/buffer DIR_SRCS_BUFFER)
aux_source_directory(./src/log DIR_SRCS_LOG)
aux_source_directory(./src/utils DIR_SRCS_UTILS)

# 指定生成目标
add_executable(xdoj ${DIR_SRCS} ${DIR_SRCS_BUFFER} ${DIR_SRCS_LOG} ${DIR_SRCS_UTILS})

# 链接库 pthread在hiredis redis++后面才能正常编译？
target_link_libraries(xdoj hiredis redis++ jsoncpp pthread judger mongocxx bsoncxx)

# 指定编译参数
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c++17")

