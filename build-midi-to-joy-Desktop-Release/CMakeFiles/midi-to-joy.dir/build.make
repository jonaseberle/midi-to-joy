# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_SOURCE_DIR = /home/jonas/src/github.com/jonaseberle/midi-to-joy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jonas/src/github.com/jonaseberle/midi-to-joy/build-midi-to-joy-Desktop-Release

# Include any dependencies generated for this target.
include CMakeFiles/midi-to-joy.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/midi-to-joy.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/midi-to-joy.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/midi-to-joy.dir/flags.make

CMakeFiles/midi-to-joy.dir/main.c.o: CMakeFiles/midi-to-joy.dir/flags.make
CMakeFiles/midi-to-joy.dir/main.c.o: /home/jonas/src/github.com/jonaseberle/midi-to-joy/main.c
CMakeFiles/midi-to-joy.dir/main.c.o: CMakeFiles/midi-to-joy.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jonas/src/github.com/jonaseberle/midi-to-joy/build-midi-to-joy-Desktop-Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/midi-to-joy.dir/main.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/midi-to-joy.dir/main.c.o -MF CMakeFiles/midi-to-joy.dir/main.c.o.d -o CMakeFiles/midi-to-joy.dir/main.c.o -c /home/jonas/src/github.com/jonaseberle/midi-to-joy/main.c

CMakeFiles/midi-to-joy.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/midi-to-joy.dir/main.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jonas/src/github.com/jonaseberle/midi-to-joy/main.c > CMakeFiles/midi-to-joy.dir/main.c.i

CMakeFiles/midi-to-joy.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/midi-to-joy.dir/main.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jonas/src/github.com/jonaseberle/midi-to-joy/main.c -o CMakeFiles/midi-to-joy.dir/main.c.s

# Object files for target midi-to-joy
midi__to__joy_OBJECTS = \
"CMakeFiles/midi-to-joy.dir/main.c.o"

# External object files for target midi-to-joy
midi__to__joy_EXTERNAL_OBJECTS =

midi-to-joy: CMakeFiles/midi-to-joy.dir/main.c.o
midi-to-joy: CMakeFiles/midi-to-joy.dir/build.make
midi-to-joy: CMakeFiles/midi-to-joy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jonas/src/github.com/jonaseberle/midi-to-joy/build-midi-to-joy-Desktop-Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable midi-to-joy"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/midi-to-joy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/midi-to-joy.dir/build: midi-to-joy
.PHONY : CMakeFiles/midi-to-joy.dir/build

CMakeFiles/midi-to-joy.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/midi-to-joy.dir/cmake_clean.cmake
.PHONY : CMakeFiles/midi-to-joy.dir/clean

CMakeFiles/midi-to-joy.dir/depend:
	cd /home/jonas/src/github.com/jonaseberle/midi-to-joy/build-midi-to-joy-Desktop-Release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jonas/src/github.com/jonaseberle/midi-to-joy /home/jonas/src/github.com/jonaseberle/midi-to-joy /home/jonas/src/github.com/jonaseberle/midi-to-joy/build-midi-to-joy-Desktop-Release /home/jonas/src/github.com/jonaseberle/midi-to-joy/build-midi-to-joy-Desktop-Release /home/jonas/src/github.com/jonaseberle/midi-to-joy/build-midi-to-joy-Desktop-Release/CMakeFiles/midi-to-joy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/midi-to-joy.dir/depend
