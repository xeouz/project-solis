# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/dev0/.local/lib/python3.8/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/dev0/.local/lib/python3.8/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dev0/Programming/project-solis

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dev0/Programming/project-solis/build

# Include any dependencies generated for this target.
include CMakeFiles/SOLIS.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/SOLIS.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/SOLIS.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SOLIS.dir/flags.make

CMakeFiles/SOLIS.dir/src/main.cc.o: CMakeFiles/SOLIS.dir/flags.make
CMakeFiles/SOLIS.dir/src/main.cc.o: /home/dev0/Programming/project-solis/src/main.cc
CMakeFiles/SOLIS.dir/src/main.cc.o: CMakeFiles/SOLIS.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dev0/Programming/project-solis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SOLIS.dir/src/main.cc.o"
	/home/dev0/Programming/llvm-project/build/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/SOLIS.dir/src/main.cc.o -MF CMakeFiles/SOLIS.dir/src/main.cc.o.d -o CMakeFiles/SOLIS.dir/src/main.cc.o -c /home/dev0/Programming/project-solis/src/main.cc

CMakeFiles/SOLIS.dir/src/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SOLIS.dir/src/main.cc.i"
	/home/dev0/Programming/llvm-project/build/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dev0/Programming/project-solis/src/main.cc > CMakeFiles/SOLIS.dir/src/main.cc.i

CMakeFiles/SOLIS.dir/src/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SOLIS.dir/src/main.cc.s"
	/home/dev0/Programming/llvm-project/build/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dev0/Programming/project-solis/src/main.cc -o CMakeFiles/SOLIS.dir/src/main.cc.s

# Object files for target SOLIS
SOLIS_OBJECTS = \
"CMakeFiles/SOLIS.dir/src/main.cc.o"

# External object files for target SOLIS
SOLIS_EXTERNAL_OBJECTS =

SOLIS: CMakeFiles/SOLIS.dir/src/main.cc.o
SOLIS: CMakeFiles/SOLIS.dir/build.make
SOLIS: libsolis-image.a
SOLIS: CMakeFiles/SOLIS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dev0/Programming/project-solis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable SOLIS"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SOLIS.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SOLIS.dir/build: SOLIS
.PHONY : CMakeFiles/SOLIS.dir/build

CMakeFiles/SOLIS.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SOLIS.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SOLIS.dir/clean

CMakeFiles/SOLIS.dir/depend:
	cd /home/dev0/Programming/project-solis/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dev0/Programming/project-solis /home/dev0/Programming/project-solis /home/dev0/Programming/project-solis/build /home/dev0/Programming/project-solis/build /home/dev0/Programming/project-solis/build/CMakeFiles/SOLIS.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SOLIS.dir/depend
