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

# Utility rule file for generateAdf.

# Include any custom commands dependencies for this target.
include CMakeFiles/generateAdf.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/generateAdf.dir/progress.make

CMakeFiles/generateAdf:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=F:\FlappyAceWin\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ADF file"
	F:\cmake-3.28.0-rc3-windows-x86_64\bin\cmake.exe -E make_directory F:/FlappyAceWin/build/adf/s
	F:\cmake-3.28.0-rc3-windows-x86_64\bin\cmake.exe -E copy F:/FlappyAceWin/build/FlappyAceWin.exe F:/FlappyAceWin/build/adf
	F:\cmake-3.28.0-rc3-windows-x86_64\bin\cmake.exe -E copy F:/FlappyAceWin/build/myacefont.fnt F:/FlappyAceWin/build/adf
	F:\cmake-3.28.0-rc3-windows-x86_64\bin\cmake.exe -E echo FlappyAceWin.exe > F:/FlappyAceWin/build/adf/s/startup-sequence
	exe2adf -l FlappyAceWin -a FlappyAceWin.adf -d F:/FlappyAceWin/build/adf
	F:\cmake-3.28.0-rc3-windows-x86_64\bin\cmake.exe -E rm -rf F:/FlappyAceWin/build/adf

generateAdf: CMakeFiles/generateAdf
generateAdf: CMakeFiles/generateAdf.dir/build.make
.PHONY : generateAdf

# Rule to build all files generated by this target.
CMakeFiles/generateAdf.dir/build: generateAdf
.PHONY : CMakeFiles/generateAdf.dir/build

CMakeFiles/generateAdf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\generateAdf.dir\cmake_clean.cmake
.PHONY : CMakeFiles/generateAdf.dir/clean

CMakeFiles/generateAdf.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" F:\FlappyAceWin F:\FlappyAceWin F:\FlappyAceWin\build F:\FlappyAceWin\build F:\FlappyAceWin\build\CMakeFiles\generateAdf.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/generateAdf.dir/depend

