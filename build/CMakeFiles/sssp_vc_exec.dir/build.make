# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/MiniGraph

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/MiniGraph/build

# Include any dependencies generated for this target.
include CMakeFiles/sssp_vc_exec.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sssp_vc_exec.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sssp_vc_exec.dir/flags.make

CMakeFiles/sssp_vc_exec.dir/apps/cpp/sssp_vc.cpp.o: CMakeFiles/sssp_vc_exec.dir/flags.make
CMakeFiles/sssp_vc_exec.dir/apps/cpp/sssp_vc.cpp.o: ../apps/cpp/sssp_vc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/MiniGraph/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sssp_vc_exec.dir/apps/cpp/sssp_vc.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sssp_vc_exec.dir/apps/cpp/sssp_vc.cpp.o -c /home/MiniGraph/apps/cpp/sssp_vc.cpp

CMakeFiles/sssp_vc_exec.dir/apps/cpp/sssp_vc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sssp_vc_exec.dir/apps/cpp/sssp_vc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/MiniGraph/apps/cpp/sssp_vc.cpp > CMakeFiles/sssp_vc_exec.dir/apps/cpp/sssp_vc.cpp.i

CMakeFiles/sssp_vc_exec.dir/apps/cpp/sssp_vc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sssp_vc_exec.dir/apps/cpp/sssp_vc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/MiniGraph/apps/cpp/sssp_vc.cpp -o CMakeFiles/sssp_vc_exec.dir/apps/cpp/sssp_vc.cpp.s

# Object files for target sssp_vc_exec
sssp_vc_exec_OBJECTS = \
"CMakeFiles/sssp_vc_exec.dir/apps/cpp/sssp_vc.cpp.o"

# External object files for target sssp_vc_exec
sssp_vc_exec_EXTERNAL_OBJECTS =

../bin/sssp_vc_exec: CMakeFiles/sssp_vc_exec.dir/apps/cpp/sssp_vc.cpp.o
../bin/sssp_vc_exec: CMakeFiles/sssp_vc_exec.dir/build.make
../bin/sssp_vc_exec: ../bin/libminigraph.a
../bin/sssp_vc_exec: /tmp/fbcode_builder_getdeps-ZhomeZfollyZbuildZfbcode_builder-root/installed/fmt-eVyq_Xn_6_PY_FY4WqtE46oKigF6uY-TdSkD2lHRl7M/lib/libfmt.a
../bin/sssp_vc_exec: /usr/include/double-conversion/../../lib/x86_64-linux-gnu/libdouble-conversion.a
../bin/sssp_vc_exec: /usr/lib/x86_64-linux-gnu/libgflags.so.2.2.2
../bin/sssp_vc_exec: /usr/lib/x86_64-linux-gnu/libglog.so
../bin/sssp_vc_exec: /tmp/fbcode_builder_getdeps-ZhomeZfollyZbuildZfbcode_builder-root/installed/folly/lib/libfolly.a
../bin/sssp_vc_exec: /tmp/fbcode_builder_getdeps-ZhomeZfollyZbuildZfbcode_builder-root/installed/fmt-eVyq_Xn_6_PY_FY4WqtE46oKigF6uY-TdSkD2lHRl7M/lib/libfmt.a
../bin/sssp_vc_exec: /usr/include/double-conversion/../../lib/x86_64-linux-gnu/libdouble-conversion.a
../bin/sssp_vc_exec: /usr/lib/x86_64-linux-gnu/libgflags.so.2.2.2
../bin/sssp_vc_exec: /usr/lib/x86_64-linux-gnu/libglog.so
../bin/sssp_vc_exec: ../minigraph/../third_party/yaml-cpp/build/libyaml-cpp.a
../bin/sssp_vc_exec: /usr/local/lib/libjemalloc.so
../bin/sssp_vc_exec: CMakeFiles/sssp_vc_exec.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/MiniGraph/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/sssp_vc_exec"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sssp_vc_exec.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sssp_vc_exec.dir/build: ../bin/sssp_vc_exec

.PHONY : CMakeFiles/sssp_vc_exec.dir/build

CMakeFiles/sssp_vc_exec.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sssp_vc_exec.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sssp_vc_exec.dir/clean

CMakeFiles/sssp_vc_exec.dir/depend:
	cd /home/MiniGraph/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/MiniGraph /home/MiniGraph /home/MiniGraph/build /home/MiniGraph/build /home/MiniGraph/build/CMakeFiles/sssp_vc_exec.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sssp_vc_exec.dir/depend
