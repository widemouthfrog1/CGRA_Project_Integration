# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /am/embassy/vol/x6/jetbrains/apps/CLion/ch-0/191.6183.77/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /am/embassy/vol/x6/jetbrains/apps/CLion/ch-0/191.6183.77/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hoefsldyla/Documents/CLionCopy/work

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug

# Include any dependencies generated for this target.
include ext/stb/CMakeFiles/stb.dir/depend.make

# Include the progress variables for this target.
include ext/stb/CMakeFiles/stb.dir/progress.make

# Include the compile flags for this target's objects.
include ext/stb/CMakeFiles/stb.dir/flags.make

ext/stb/CMakeFiles/stb.dir/stb.c.o: ext/stb/CMakeFiles/stb.dir/flags.make
ext/stb/CMakeFiles/stb.dir/stb.c.o: ../ext/stb/stb.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object ext/stb/CMakeFiles/stb.dir/stb.c.o"
	cd /home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug/ext/stb && /bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/stb.dir/stb.c.o   -c /home/hoefsldyla/Documents/CLionCopy/work/ext/stb/stb.c

ext/stb/CMakeFiles/stb.dir/stb.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/stb.dir/stb.c.i"
	cd /home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug/ext/stb && /bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hoefsldyla/Documents/CLionCopy/work/ext/stb/stb.c > CMakeFiles/stb.dir/stb.c.i

ext/stb/CMakeFiles/stb.dir/stb.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/stb.dir/stb.c.s"
	cd /home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug/ext/stb && /bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hoefsldyla/Documents/CLionCopy/work/ext/stb/stb.c -o CMakeFiles/stb.dir/stb.c.s

# Object files for target stb
stb_OBJECTS = \
"CMakeFiles/stb.dir/stb.c.o"

# External object files for target stb
stb_EXTERNAL_OBJECTS =

bin/libstb.a: ext/stb/CMakeFiles/stb.dir/stb.c.o
bin/libstb.a: ext/stb/CMakeFiles/stb.dir/build.make
bin/libstb.a: ext/stb/CMakeFiles/stb.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library ../../bin/libstb.a"
	cd /home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug/ext/stb && $(CMAKE_COMMAND) -P CMakeFiles/stb.dir/cmake_clean_target.cmake
	cd /home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug/ext/stb && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/stb.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ext/stb/CMakeFiles/stb.dir/build: bin/libstb.a

.PHONY : ext/stb/CMakeFiles/stb.dir/build

ext/stb/CMakeFiles/stb.dir/clean:
	cd /home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug/ext/stb && $(CMAKE_COMMAND) -P CMakeFiles/stb.dir/cmake_clean.cmake
.PHONY : ext/stb/CMakeFiles/stb.dir/clean

ext/stb/CMakeFiles/stb.dir/depend:
	cd /home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hoefsldyla/Documents/CLionCopy/work /home/hoefsldyla/Documents/CLionCopy/work/ext/stb /home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug /home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug/ext/stb /home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug/ext/stb/CMakeFiles/stb.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ext/stb/CMakeFiles/stb.dir/depend

