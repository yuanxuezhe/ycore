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
include 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/compiler_depend.make

# Include the progress variables for this target.
include 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/progress.make

# Include the compile flags for this target's objects.
include 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/flags.make

6_Modules/TestCallback/CMakeFiles/TestCallback.dir/callback.cpp.obj: 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/flags.make
6_Modules/TestCallback/CMakeFiles/TestCallback.dir/callback.cpp.obj: 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/includes_CXX.rsp
6_Modules/TestCallback/CMakeFiles/TestCallback.dir/callback.cpp.obj: D:/workspace/c++/yui/6_Modules/TestCallback/callback.cpp
6_Modules/TestCallback/CMakeFiles/TestCallback.dir/callback.cpp.obj: 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\workspace\c++\yui\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/callback.cpp.obj"
	cd /d D:\workspace\c++\yui\build\6_Modules\TestCallback && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/callback.cpp.obj -MF CMakeFiles\TestCallback.dir\callback.cpp.obj.d -o CMakeFiles\TestCallback.dir\callback.cpp.obj -c D:\workspace\c++\yui\6_Modules\TestCallback\callback.cpp

6_Modules/TestCallback/CMakeFiles/TestCallback.dir/callback.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TestCallback.dir/callback.cpp.i"
	cd /d D:\workspace\c++\yui\build\6_Modules\TestCallback && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\workspace\c++\yui\6_Modules\TestCallback\callback.cpp > CMakeFiles\TestCallback.dir\callback.cpp.i

6_Modules/TestCallback/CMakeFiles/TestCallback.dir/callback.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TestCallback.dir/callback.cpp.s"
	cd /d D:\workspace\c++\yui\build\6_Modules\TestCallback && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\workspace\c++\yui\6_Modules\TestCallback\callback.cpp -o CMakeFiles\TestCallback.dir\callback.cpp.s

# Object files for target TestCallback
TestCallback_OBJECTS = \
"CMakeFiles/TestCallback.dir/callback.cpp.obj"

# External object files for target TestCallback
TestCallback_EXTERNAL_OBJECTS =

D:/workspace/c++/yui/0_bin/x64/libTestCallback.dll: 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/callback.cpp.obj
D:/workspace/c++/yui/0_bin/x64/libTestCallback.dll: 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/build.make
D:/workspace/c++/yui/0_bin/x64/libTestCallback.dll: 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/linkLibs.rsp
D:/workspace/c++/yui/0_bin/x64/libTestCallback.dll: 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/objects1.rsp
D:/workspace/c++/yui/0_bin/x64/libTestCallback.dll: 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\workspace\c++\yui\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library D:\workspace\c++\yui\0_bin\x64\libTestCallback.dll"
	cd /d D:\workspace\c++\yui\build\6_Modules\TestCallback && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\TestCallback.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
6_Modules/TestCallback/CMakeFiles/TestCallback.dir/build: D:/workspace/c++/yui/0_bin/x64/libTestCallback.dll
.PHONY : 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/build

6_Modules/TestCallback/CMakeFiles/TestCallback.dir/clean:
	cd /d D:\workspace\c++\yui\build\6_Modules\TestCallback && $(CMAKE_COMMAND) -P CMakeFiles\TestCallback.dir\cmake_clean.cmake
.PHONY : 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/clean

6_Modules/TestCallback/CMakeFiles/TestCallback.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\workspace\c++\yui D:\workspace\c++\yui\6_Modules\TestCallback D:\workspace\c++\yui\build D:\workspace\c++\yui\build\6_Modules\TestCallback D:\workspace\c++\yui\build\6_Modules\TestCallback\CMakeFiles\TestCallback.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : 6_Modules/TestCallback/CMakeFiles/TestCallback.dir/depend

