# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hailrake/media/code/cpcs

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hailrake/media/code/cpcs/build

# Include any dependencies generated for this target.
include CMakeFiles/cpcs.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cpcs.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cpcs.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cpcs.dir/flags.make

CMakeFiles/cpcs.dir/main.c.o: CMakeFiles/cpcs.dir/flags.make
CMakeFiles/cpcs.dir/main.c.o: /home/hailrake/media/code/cpcs/main.c
CMakeFiles/cpcs.dir/main.c.o: CMakeFiles/cpcs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/hailrake/media/code/cpcs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/cpcs.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/cpcs.dir/main.c.o -MF CMakeFiles/cpcs.dir/main.c.o.d -o CMakeFiles/cpcs.dir/main.c.o -c /home/hailrake/media/code/cpcs/main.c

CMakeFiles/cpcs.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/cpcs.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hailrake/media/code/cpcs/main.c > CMakeFiles/cpcs.dir/main.c.i

CMakeFiles/cpcs.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/cpcs.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hailrake/media/code/cpcs/main.c -o CMakeFiles/cpcs.dir/main.c.s

CMakeFiles/cpcs.dir/COM.c.o: CMakeFiles/cpcs.dir/flags.make
CMakeFiles/cpcs.dir/COM.c.o: /home/hailrake/media/code/cpcs/COM.c
CMakeFiles/cpcs.dir/COM.c.o: CMakeFiles/cpcs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/hailrake/media/code/cpcs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/cpcs.dir/COM.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/cpcs.dir/COM.c.o -MF CMakeFiles/cpcs.dir/COM.c.o.d -o CMakeFiles/cpcs.dir/COM.c.o -c /home/hailrake/media/code/cpcs/COM.c

CMakeFiles/cpcs.dir/COM.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/cpcs.dir/COM.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hailrake/media/code/cpcs/COM.c > CMakeFiles/cpcs.dir/COM.c.i

CMakeFiles/cpcs.dir/COM.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/cpcs.dir/COM.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hailrake/media/code/cpcs/COM.c -o CMakeFiles/cpcs.dir/COM.c.s

# Object files for target cpcs
cpcs_OBJECTS = \
"CMakeFiles/cpcs.dir/main.c.o" \
"CMakeFiles/cpcs.dir/COM.c.o"

# External object files for target cpcs
cpcs_EXTERNAL_OBJECTS =

cpcs: CMakeFiles/cpcs.dir/main.c.o
cpcs: CMakeFiles/cpcs.dir/COM.c.o
cpcs: CMakeFiles/cpcs.dir/build.make
cpcs: CMakeFiles/cpcs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/hailrake/media/code/cpcs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable cpcs"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cpcs.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cpcs.dir/build: cpcs
.PHONY : CMakeFiles/cpcs.dir/build

CMakeFiles/cpcs.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cpcs.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cpcs.dir/clean

CMakeFiles/cpcs.dir/depend:
	cd /home/hailrake/media/code/cpcs/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hailrake/media/code/cpcs /home/hailrake/media/code/cpcs /home/hailrake/media/code/cpcs/build /home/hailrake/media/code/cpcs/build /home/hailrake/media/code/cpcs/build/CMakeFiles/cpcs.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/cpcs.dir/depend
