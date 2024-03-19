set(TARGET comm)

#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${platform})

#需要包含的头文件目录
include_directories(
    ../include
)

# 查找当前目录下的所有源文件，并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

#生成动态库 add_library要在target_link_libraries 之前
add_library(${TARGET} SHARED ${DIR_SRCS})
#add_executable(${TARGET} ${DIR_SRCS})

#需要链接的库名
target_link_libraries(${TARGET} )