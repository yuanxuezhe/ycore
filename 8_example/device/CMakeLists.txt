message(STATUS "CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR}")
#动态库名设置(获取当前文件夹作为要编译的库名)
STRING( REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} )
set(TARGET ${TARGET_PREFIX}${LBM_NAME}${CURRENT_FOLDER})

#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${platform})

# 查找当前目录下的所有源文件，并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

#需要包含的头文件目录
include_directories(
${Comm_include_path}
${Modules_include_path}
${Source_include_path}
)

#链接库目录
# link_directories(
# ${FL_lib_path}
# )

#生成动态库 add_library要在target_link_libraries 之前
#add_library(${TARGET} SHARED ${DIR_SRCS})
add_executable(${TARGET} ${DIR_SRCS})

#需要链接的库名
target_link_libraries(${TARGET} fltk yscore)