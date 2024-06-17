# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8...3.13)

# #默认编译Release
# set(default_build_type RELEASE)

# 项目信息
project(fltk-demo)

#生成库名的前缀ma
#set(TARGET_PREFIX wy)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)  # 你可以根据需要选择不同的C++标准

# 设置C++编译器
set(CMAKE_CXX_COMPILER g++)

# 添加一个编译定义，例如 DEBUG_MODE
add_compile_definitions(INTERNAL_LINKAGE)
add_compile_definitions(FL_LIBRARY)

#找当前目录的子目录列表
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
  #排除.svn文件夹
    IF(NOT (child STREQUAL .svn) AND IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

#判断编译器类型,如果是gcc编译器,添加编译选项
if(CMAKE_COMPILER_IS_GNUCXX)
	#set(platform x64)
	set(platform Lnx64)
	#输出make的过程
	#set(CMAKE_VERBOSE_MAKEFILE ON)
	
	#set(CMAKE_SKIP_RPATH ON)强制cmake不在构建期间和install期间加上它所认为的rpath, 即-Wl,-rpath参数
	#set(CMAKE_SKIP_RPATH ON)
    
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -march=x86-64 -fpermissive -w -O2")
	#set(CMAKE_SHARED_LINKER_FLAGS "-lGL")
	#set(CMAKE_SHARED_LINKER_FLAGS "-lrt -ldl -pthread")
	
	#add_definitions(-DOS_IS_LINUX -DKD_NDEBUG -DGS_COS -DUSE_FOLLY_MAP)
endif(CMAKE_COMPILER_IS_GNUCXX)

#设置so默认输出路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/0_bin/${platform})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/0_bin/${platform})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/1_lib/${platform})

#设置包含目录
set(Comm_include_path ${PROJECT_SOURCE_DIR}/5_Comm)
set(Tables_include_path ${PROJECT_SOURCE_DIR}/tables)
set(Modules_include_path ${PROJECT_SOURCE_DIR}/6_Modules)
set(Source_include_path ${PROJECT_SOURCE_DIR}/8_source)
#分别保存Sdk下的所有头文件和库文件路径
SUBDIRLIST(sdk_sub_dirs ${PROJECT_SOURCE_DIR}/3_Sdk)
FOREACH(sub_dir ${sdk_sub_dirs})
	set(${sub_dir}_include_path ${PROJECT_SOURCE_DIR}/3_Sdk/${sub_dir}/include)
	set(${sub_dir}_lib_path ${PROJECT_SOURCE_DIR}/3_Sdk/${sub_dir}/lib/${platform})
	message(STATUS "${sub_dir}_include_path: ${${sub_dir}_include_path}")
	message(STATUS "${sub_dir}_lib_path: ${${sub_dir}_lib_path}")
ENDFOREACH()


#需要包含的头文件目录
include_directories(
${PROJECT_SOURCE_DIR}/4_Public
${FL_include_path}
${FL_include_path}
${spdlog_include_path}
${Comm_include_path}
${Tables_include_path}
)

#链接库目录
link_directories(
${PROJECT_SOURCE_DIR}/0_bin/${platform}
${spdlog_lib_path}
${Comm_lib_path}
)

add_subdirectory(${PROJECT_SOURCE_DIR}/5_Comm)

# 编译路径下所有文件夹
#FOREACH(dir 6_Modules 7_main)
FOREACH(dir 7_main)
	SUBDIRLIST(sub_dirs ${PROJECT_SOURCE_DIR}/${dir})

	#排除不需要编译的文件夹
 	FOREACH(exclude spider testdemo yui_gtest)
 		list(REMOVE_ITEM sub_dirs ${exclude})
 	ENDFOREACH()

 	FOREACH(sub_dir ${sub_dirs})
 		message(STATUS "add_subdirectory:${dir}/${sub_dir}")
 		add_subdirectory(${dir}/${sub_dir})
 	ENDFOREACH() 
ENDFOREACH()

#add_subdirectory(${PROJECT_SOURCE_DIR}/5_Comm)
#add_subdirectory(${PROJECT_SOURCE_DIR}/7_main/server)
#add_subdirectory(${PROJECT_SOURCE_DIR}/7_main/libevent_demo)
#add_subdirectory(${PROJECT_SOURCE_DIR}/7_main/amqp_demo)
#add_subdirectory(${PROJECT_SOURCE_DIR}/7_main/tinyxml_demo)
#add_subdirectory(${PROJECT_SOURCE_DIR}/Fl_ext)
