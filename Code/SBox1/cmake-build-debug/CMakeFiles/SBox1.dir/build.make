# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\Yevhenii\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\203.4203.24\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\Yevhenii\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\203.4203.24\bin\cmake\win\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Diploma\Code\SBox1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Diploma\Code\SBox1\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/SBox1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SBox1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SBox1.dir/flags.make

CMakeFiles/SBox1.dir/main.cpp.obj: CMakeFiles/SBox1.dir/flags.make
CMakeFiles/SBox1.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Diploma\Code\SBox1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SBox1.dir/main.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\SBox1.dir\main.cpp.obj -c D:\Diploma\Code\SBox1\main.cpp

CMakeFiles/SBox1.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SBox1.dir/main.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Diploma\Code\SBox1\main.cpp > CMakeFiles\SBox1.dir\main.cpp.i

CMakeFiles/SBox1.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SBox1.dir/main.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Diploma\Code\SBox1\main.cpp -o CMakeFiles\SBox1.dir\main.cpp.s

# Object files for target SBox1
SBox1_OBJECTS = \
"CMakeFiles/SBox1.dir/main.cpp.obj"

# External object files for target SBox1
SBox1_EXTERNAL_OBJECTS =

SBox1.exe: CMakeFiles/SBox1.dir/main.cpp.obj
SBox1.exe: CMakeFiles/SBox1.dir/build.make
SBox1.exe: CMakeFiles/SBox1.dir/linklibs.rsp
SBox1.exe: CMakeFiles/SBox1.dir/objects1.rsp
SBox1.exe: CMakeFiles/SBox1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Diploma\Code\SBox1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable SBox1.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\SBox1.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SBox1.dir/build: SBox1.exe

.PHONY : CMakeFiles/SBox1.dir/build

CMakeFiles/SBox1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\SBox1.dir\cmake_clean.cmake
.PHONY : CMakeFiles/SBox1.dir/clean

CMakeFiles/SBox1.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Diploma\Code\SBox1 D:\Diploma\Code\SBox1 D:\Diploma\Code\SBox1\cmake-build-debug D:\Diploma\Code\SBox1\cmake-build-debug D:\Diploma\Code\SBox1\cmake-build-debug\CMakeFiles\SBox1.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SBox1.dir/depend

