# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = F:\cmake-3.28.0-rc3-windows-x86_64\bin\cmake.exe

# The command to remove a file.
RM = F:\cmake-3.28.0-rc3-windows-x86_64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = F:\FlappyAceWin

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = F:\FlappyAceWin\build

# Utility rule file for generateZip.

# Include any custom commands dependencies for this target.
include CMakeFiles/generateZip.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/generateZip.dir/progress.make

CMakeFiles/generateZip:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=F:\FlappyAceWin\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating FlappyAceWin 23_11_30.zip"
	F:\cmake-3.28.0-rc3-windows-x86_64\bin\cmake.exe -E tar cf "FlappyAceWin 23_11_30.zip" --format=zip F:/FlappyAceWin/build/FlappyAceWin.exe F:/FlappyAceWin/build/myacefont.fnt

generateZip: CMakeFiles/generateZip
generateZip: CMakeFiles/generateZip.dir/build.make
.PHONY : generateZip

# Rule to build all files generated by this target.
CMakeFiles/generateZip.dir/build: generateZip
.PHONY : CMakeFiles/generateZip.dir/build

CMakeFiles/generateZip.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\generateZip.dir\cmake_clean.cmake
.PHONY : CMakeFiles/generateZip.dir/clean

CMakeFiles/generateZip.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" F:\FlappyAceWin F:\FlappyAceWin F:\FlappyAceWin\build F:\FlappyAceWin\build F:\FlappyAceWin\build\CMakeFiles\generateZip.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/generateZip.dir/depend

