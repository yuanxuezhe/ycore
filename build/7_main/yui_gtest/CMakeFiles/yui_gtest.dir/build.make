# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = D:\software_work\msys64\mingw64\bin\cmake.exe

# The command to remove a file.
RM = D:\software_work\msys64\mingw64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\workspace\c++\yui

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\workspace\c++\yui\build

# Include any dependencies generated for this target.
include 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/compiler_depend.make

# Include the progress variables for this target.
include 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/progress.make

# Include the compile flags for this target's objects.
include 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/flags.make

7_main/yui_gtest/CMakeFiles/yui_gtest.dir/main.cpp.obj: 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/flags.make
7_main/yui_gtest/CMakeFiles/yui_gtest.dir/main.cpp.obj: 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/includes_CXX.rsp
7_main/yui_gtest/CMakeFiles/yui_gtest.dir/main.cpp.obj: D:/workspace/c++/yui/7_main/yui_gtest/main.cpp
7_main/yui_gtest/CMakeFiles/yui_gtest.dir/main.cpp.obj: 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\workspace\c++\yui\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/main.cpp.obj"
	cd /d D:\workspace\c++\yui\build\7_main\yui_gtest && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/main.cpp.obj -MF CMakeFiles\yui_gtest.dir\main.cpp.obj.d -o CMakeFiles\yui_gtest.dir\main.cpp.obj -c D:\workspace\c++\yui\7_main\yui_gtest\main.cpp

7_main/yui_gtest/CMakeFiles/yui_gtest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/yui_gtest.dir/main.cpp.i"
	cd /d D:\workspace\c++\yui\build\7_main\yui_gtest && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\workspace\c++\yui\7_main\yui_gtest\main.cpp > CMakeFiles\yui_gtest.dir\main.cpp.i

7_main/yui_gtest/CMakeFiles/yui_gtest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/yui_gtest.dir/main.cpp.s"
	cd /d D:\workspace\c++\yui\build\7_main\yui_gtest && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\workspace\c++\yui\7_main\yui_gtest\main.cpp -o CMakeFiles\yui_gtest.dir\main.cpp.s

# Object files for target yui_gtest
yui_gtest_OBJECTS = \
"CMakeFiles/yui_gtest.dir/main.cpp.obj"

# External object files for target yui_gtest
yui_gtest_EXTERNAL_OBJECTS =

D:/workspace/c++/yui/0_bin/x64/yui_gtest.exe: 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/main.cpp.obj
D:/workspace/c++/yui/0_bin/x64/yui_gtest.exe: 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/build.make
D:/workspace/c++/yui/0_bin/x64/yui_gtest.exe: D:/workspace/c++/yui/1_lib/x64/libcomm.dll.a
D:/workspace/c++/yui/0_bin/x64/yui_gtest.exe: 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/linkLibs.rsp
D:/workspace/c++/yui/0_bin/x64/yui_gtest.exe: 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/objects1.rsp
D:/workspace/c++/yui/0_bin/x64/yui_gtest.exe: 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\workspace\c++\yui\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable D:\workspace\c++\yui\0_bin\x64\yui_gtest.exe"
	cd /d D:\workspace\c++\yui\build\7_main\yui_gtest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\yui_gtest.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
7_main/yui_gtest/CMakeFiles/yui_gtest.dir/build: D:/workspace/c++/yui/0_bin/x64/yui_gtest.exe
.PHONY : 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/build

7_main/yui_gtest/CMakeFiles/yui_gtest.dir/clean:
	cd /d D:\workspace\c++\yui\build\7_main\yui_gtest && $(CMAKE_COMMAND) -P CMakeFiles\yui_gtest.dir\cmake_clean.cmake
.PHONY : 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/clean

7_main/yui_gtest/CMakeFiles/yui_gtest.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\workspace\c++\yui D:\workspace\c++\yui\7_main\yui_gtest D:\workspace\c++\yui\build D:\workspace\c++\yui\build\7_main\yui_gtest D:\workspace\c++\yui\build\7_main\yui_gtest\CMakeFiles\yui_gtest.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : 7_main/yui_gtest/CMakeFiles/yui_gtest.dir/depend
