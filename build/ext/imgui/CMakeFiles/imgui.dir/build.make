# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dylan/Documents/CGRA_Project_Integration/work

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dylan/Documents/CGRA_Project_Integration/build

# Include any dependencies generated for this target.
include ext/imgui/CMakeFiles/imgui.dir/depend.make

# Include the progress variables for this target.
include ext/imgui/CMakeFiles/imgui.dir/progress.make

# Include the compile flags for this target's objects.
include ext/imgui/CMakeFiles/imgui.dir/flags.make

ext/imgui/CMakeFiles/imgui.dir/imgui.cpp.o: ext/imgui/CMakeFiles/imgui.dir/flags.make
ext/imgui/CMakeFiles/imgui.dir/imgui.cpp.o: /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dylan/Documents/CGRA_Project_Integration/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object ext/imgui/CMakeFiles/imgui.dir/imgui.cpp.o"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/imgui.dir/imgui.cpp.o -c /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui.cpp

ext/imgui/CMakeFiles/imgui.dir/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/imgui.cpp.i"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui.cpp > CMakeFiles/imgui.dir/imgui.cpp.i

ext/imgui/CMakeFiles/imgui.dir/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/imgui.cpp.s"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui.cpp -o CMakeFiles/imgui.dir/imgui.cpp.s

ext/imgui/CMakeFiles/imgui.dir/imgui_draw.cpp.o: ext/imgui/CMakeFiles/imgui.dir/flags.make
ext/imgui/CMakeFiles/imgui.dir/imgui_draw.cpp.o: /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_draw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dylan/Documents/CGRA_Project_Integration/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object ext/imgui/CMakeFiles/imgui.dir/imgui_draw.cpp.o"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/imgui.dir/imgui_draw.cpp.o -c /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_draw.cpp

ext/imgui/CMakeFiles/imgui.dir/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/imgui_draw.cpp.i"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_draw.cpp > CMakeFiles/imgui.dir/imgui_draw.cpp.i

ext/imgui/CMakeFiles/imgui.dir/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/imgui_draw.cpp.s"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_draw.cpp -o CMakeFiles/imgui.dir/imgui_draw.cpp.s

ext/imgui/CMakeFiles/imgui.dir/imgui_widgets.cpp.o: ext/imgui/CMakeFiles/imgui.dir/flags.make
ext/imgui/CMakeFiles/imgui.dir/imgui_widgets.cpp.o: /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_widgets.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dylan/Documents/CGRA_Project_Integration/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object ext/imgui/CMakeFiles/imgui.dir/imgui_widgets.cpp.o"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/imgui.dir/imgui_widgets.cpp.o -c /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_widgets.cpp

ext/imgui/CMakeFiles/imgui.dir/imgui_widgets.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/imgui_widgets.cpp.i"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_widgets.cpp > CMakeFiles/imgui.dir/imgui_widgets.cpp.i

ext/imgui/CMakeFiles/imgui.dir/imgui_widgets.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/imgui_widgets.cpp.s"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_widgets.cpp -o CMakeFiles/imgui.dir/imgui_widgets.cpp.s

ext/imgui/CMakeFiles/imgui.dir/imgui_impl_glfw.cpp.o: ext/imgui/CMakeFiles/imgui.dir/flags.make
ext/imgui/CMakeFiles/imgui.dir/imgui_impl_glfw.cpp.o: /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_impl_glfw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dylan/Documents/CGRA_Project_Integration/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object ext/imgui/CMakeFiles/imgui.dir/imgui_impl_glfw.cpp.o"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/imgui.dir/imgui_impl_glfw.cpp.o -c /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_impl_glfw.cpp

ext/imgui/CMakeFiles/imgui.dir/imgui_impl_glfw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/imgui_impl_glfw.cpp.i"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_impl_glfw.cpp > CMakeFiles/imgui.dir/imgui_impl_glfw.cpp.i

ext/imgui/CMakeFiles/imgui.dir/imgui_impl_glfw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/imgui_impl_glfw.cpp.s"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_impl_glfw.cpp -o CMakeFiles/imgui.dir/imgui_impl_glfw.cpp.s

ext/imgui/CMakeFiles/imgui.dir/imgui_impl_opengl3.cpp.o: ext/imgui/CMakeFiles/imgui.dir/flags.make
ext/imgui/CMakeFiles/imgui.dir/imgui_impl_opengl3.cpp.o: /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_impl_opengl3.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dylan/Documents/CGRA_Project_Integration/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object ext/imgui/CMakeFiles/imgui.dir/imgui_impl_opengl3.cpp.o"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/imgui.dir/imgui_impl_opengl3.cpp.o -c /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_impl_opengl3.cpp

ext/imgui/CMakeFiles/imgui.dir/imgui_impl_opengl3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/imgui_impl_opengl3.cpp.i"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_impl_opengl3.cpp > CMakeFiles/imgui.dir/imgui_impl_opengl3.cpp.i

ext/imgui/CMakeFiles/imgui.dir/imgui_impl_opengl3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/imgui_impl_opengl3.cpp.s"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui/imgui_impl_opengl3.cpp -o CMakeFiles/imgui.dir/imgui_impl_opengl3.cpp.s

# Object files for target imgui
imgui_OBJECTS = \
"CMakeFiles/imgui.dir/imgui.cpp.o" \
"CMakeFiles/imgui.dir/imgui_draw.cpp.o" \
"CMakeFiles/imgui.dir/imgui_widgets.cpp.o" \
"CMakeFiles/imgui.dir/imgui_impl_glfw.cpp.o" \
"CMakeFiles/imgui.dir/imgui_impl_opengl3.cpp.o"

# External object files for target imgui
imgui_EXTERNAL_OBJECTS =

bin/libimgui.a: ext/imgui/CMakeFiles/imgui.dir/imgui.cpp.o
bin/libimgui.a: ext/imgui/CMakeFiles/imgui.dir/imgui_draw.cpp.o
bin/libimgui.a: ext/imgui/CMakeFiles/imgui.dir/imgui_widgets.cpp.o
bin/libimgui.a: ext/imgui/CMakeFiles/imgui.dir/imgui_impl_glfw.cpp.o
bin/libimgui.a: ext/imgui/CMakeFiles/imgui.dir/imgui_impl_opengl3.cpp.o
bin/libimgui.a: ext/imgui/CMakeFiles/imgui.dir/build.make
bin/libimgui.a: ext/imgui/CMakeFiles/imgui.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dylan/Documents/CGRA_Project_Integration/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library ../../bin/libimgui.a"
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && $(CMAKE_COMMAND) -P CMakeFiles/imgui.dir/cmake_clean_target.cmake
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/imgui.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ext/imgui/CMakeFiles/imgui.dir/build: bin/libimgui.a

.PHONY : ext/imgui/CMakeFiles/imgui.dir/build

ext/imgui/CMakeFiles/imgui.dir/clean:
	cd /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui && $(CMAKE_COMMAND) -P CMakeFiles/imgui.dir/cmake_clean.cmake
.PHONY : ext/imgui/CMakeFiles/imgui.dir/clean

ext/imgui/CMakeFiles/imgui.dir/depend:
	cd /home/dylan/Documents/CGRA_Project_Integration/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dylan/Documents/CGRA_Project_Integration/work /home/dylan/Documents/CGRA_Project_Integration/work/ext/imgui /home/dylan/Documents/CGRA_Project_Integration/build /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui /home/dylan/Documents/CGRA_Project_Integration/build/ext/imgui/CMakeFiles/imgui.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ext/imgui/CMakeFiles/imgui.dir/depend

