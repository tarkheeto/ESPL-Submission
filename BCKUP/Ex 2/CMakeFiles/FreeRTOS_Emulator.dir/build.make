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
CMAKE_SOURCE_DIR = /home/tarkheeto/FreeRTOS-Emulator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tarkheeto/FreeRTOS-Emulator

# Include any dependencies generated for this target.
include CMakeFiles/FreeRTOS_Emulator.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FreeRTOS_Emulator.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FreeRTOS_Emulator.dir/flags.make

CMakeFiles/FreeRTOS_Emulator.dir/src/main.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/src/main.c.o: src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/src/main.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/src/main.c

CMakeFiles/FreeRTOS_Emulator.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/src/main.c > CMakeFiles/FreeRTOS_Emulator.dir/src/main.c.i

CMakeFiles/FreeRTOS_Emulator.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/src/main.c -o CMakeFiles/FreeRTOS_Emulator.dir/src/main.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/croutine.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/croutine.c.o: lib/FreeRTOS_Kernel/croutine.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/croutine.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/croutine.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/croutine.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/croutine.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/croutine.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/croutine.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/croutine.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/croutine.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/croutine.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/croutine.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/croutine.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/list.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/list.c.o: lib/FreeRTOS_Kernel/list.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/list.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/list.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/list.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/list.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/list.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/list.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/list.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/list.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/list.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/list.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/list.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c.o: lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c.o: lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/queue.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/queue.c.o: lib/FreeRTOS_Kernel/queue.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/queue.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/queue.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/queue.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/queue.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/queue.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/queue.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/queue.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/queue.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/queue.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/queue.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/queue.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/tasks.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/tasks.c.o: lib/FreeRTOS_Kernel/tasks.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/tasks.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/tasks.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/tasks.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/tasks.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/tasks.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/tasks.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/tasks.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/tasks.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/tasks.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/tasks.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/tasks.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/timers.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/timers.c.o: lib/FreeRTOS_Kernel/timers.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/timers.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/timers.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/timers.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/timers.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/timers.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/timers.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/timers.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/timers.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/timers.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/FreeRTOS_Kernel/timers.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/timers.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Ball.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Ball.c.o: lib/Gfx/TUM_Ball.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Ball.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Ball.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Ball.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Ball.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Ball.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Ball.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Ball.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Ball.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Ball.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Ball.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Ball.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Draw.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Draw.c.o: lib/Gfx/TUM_Draw.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Draw.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Draw.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Draw.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Draw.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Draw.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Draw.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Draw.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Draw.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Draw.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Draw.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Draw.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Event.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Event.c.o: lib/Gfx/TUM_Event.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Event.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Event.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Event.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Event.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Event.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Event.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Event.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Event.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Event.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Event.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Event.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Font.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Font.c.o: lib/Gfx/TUM_Font.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Font.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Font.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Font.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Font.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Font.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Font.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Font.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Font.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Font.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Font.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Font.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_FreeRTOS_Utils.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_FreeRTOS_Utils.c.o: lib/Gfx/TUM_FreeRTOS_Utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_FreeRTOS_Utils.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_FreeRTOS_Utils.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_FreeRTOS_Utils.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_FreeRTOS_Utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_FreeRTOS_Utils.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_FreeRTOS_Utils.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_FreeRTOS_Utils.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_FreeRTOS_Utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_FreeRTOS_Utils.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_FreeRTOS_Utils.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_FreeRTOS_Utils.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Print.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Print.c.o: lib/Gfx/TUM_Print.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Print.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Print.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Print.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Print.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Print.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Print.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Print.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Print.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Print.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Print.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Print.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Sound.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Sound.c.o: lib/Gfx/TUM_Sound.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Sound.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Sound.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Sound.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Sound.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Sound.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Sound.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Sound.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Sound.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Sound.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Sound.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Sound.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Utils.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Utils.c.o: lib/Gfx/TUM_Utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Utils.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Utils.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Utils.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Utils.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Utils.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Utils.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Utils.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/Gfx/TUM_Utils.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Utils.c.s

CMakeFiles/FreeRTOS_Emulator.dir/lib/AsyncIO/AsyncIO.c.o: CMakeFiles/FreeRTOS_Emulator.dir/flags.make
CMakeFiles/FreeRTOS_Emulator.dir/lib/AsyncIO/AsyncIO.c.o: lib/AsyncIO/AsyncIO.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building C object CMakeFiles/FreeRTOS_Emulator.dir/lib/AsyncIO/AsyncIO.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FreeRTOS_Emulator.dir/lib/AsyncIO/AsyncIO.c.o   -c /home/tarkheeto/FreeRTOS-Emulator/lib/AsyncIO/AsyncIO.c

CMakeFiles/FreeRTOS_Emulator.dir/lib/AsyncIO/AsyncIO.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FreeRTOS_Emulator.dir/lib/AsyncIO/AsyncIO.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tarkheeto/FreeRTOS-Emulator/lib/AsyncIO/AsyncIO.c > CMakeFiles/FreeRTOS_Emulator.dir/lib/AsyncIO/AsyncIO.c.i

CMakeFiles/FreeRTOS_Emulator.dir/lib/AsyncIO/AsyncIO.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FreeRTOS_Emulator.dir/lib/AsyncIO/AsyncIO.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tarkheeto/FreeRTOS-Emulator/lib/AsyncIO/AsyncIO.c -o CMakeFiles/FreeRTOS_Emulator.dir/lib/AsyncIO/AsyncIO.c.s

# Object files for target FreeRTOS_Emulator
FreeRTOS_Emulator_OBJECTS = \
"CMakeFiles/FreeRTOS_Emulator.dir/src/main.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/croutine.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/list.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/queue.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/tasks.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/timers.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Ball.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Draw.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Event.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Font.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_FreeRTOS_Utils.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Print.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Sound.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Utils.c.o" \
"CMakeFiles/FreeRTOS_Emulator.dir/lib/AsyncIO/AsyncIO.c.o"

# External object files for target FreeRTOS_Emulator
FreeRTOS_Emulator_EXTERNAL_OBJECTS =

bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/src/main.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/croutine.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/list.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/GCC/Posix/port.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/portable/MemMang/heap_3.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/queue.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/tasks.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/FreeRTOS_Kernel/timers.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Ball.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Draw.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Event.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Font.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_FreeRTOS_Utils.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Print.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Sound.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/Gfx/TUM_Utils.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/lib/AsyncIO/AsyncIO.c.o
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/build.make
bin/FreeRTOS_Emulator: /usr/lib/x86_64-linux-gnu/libSDL2.so
bin/FreeRTOS_Emulator: /usr/lib/x86_64-linux-gnu/libSDL2main.a
bin/FreeRTOS_Emulator: /usr/lib/x86_64-linux-gnu/libSDL2_gfx.so
bin/FreeRTOS_Emulator: /usr/lib/x86_64-linux-gnu/libSDL2_image.so
bin/FreeRTOS_Emulator: /usr/lib/x86_64-linux-gnu/libSDL2_mixer.so
bin/FreeRTOS_Emulator: /usr/lib/x86_64-linux-gnu/libSDL2_ttf.so
bin/FreeRTOS_Emulator: /usr/lib/x86_64-linux-gnu/libSDL2main.a
bin/FreeRTOS_Emulator: /usr/lib/x86_64-linux-gnu/libSDL2_gfx.so
bin/FreeRTOS_Emulator: /usr/lib/x86_64-linux-gnu/libSDL2_image.so
bin/FreeRTOS_Emulator: /usr/lib/x86_64-linux-gnu/libSDL2_mixer.so
bin/FreeRTOS_Emulator: /usr/lib/x86_64-linux-gnu/libSDL2_ttf.so
bin/FreeRTOS_Emulator: CMakeFiles/FreeRTOS_Emulator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tarkheeto/FreeRTOS-Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Linking C executable bin/FreeRTOS_Emulator"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FreeRTOS_Emulator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FreeRTOS_Emulator.dir/build: bin/FreeRTOS_Emulator

.PHONY : CMakeFiles/FreeRTOS_Emulator.dir/build

CMakeFiles/FreeRTOS_Emulator.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FreeRTOS_Emulator.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FreeRTOS_Emulator.dir/clean

CMakeFiles/FreeRTOS_Emulator.dir/depend:
	cd /home/tarkheeto/FreeRTOS-Emulator && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tarkheeto/FreeRTOS-Emulator /home/tarkheeto/FreeRTOS-Emulator /home/tarkheeto/FreeRTOS-Emulator /home/tarkheeto/FreeRTOS-Emulator /home/tarkheeto/FreeRTOS-Emulator/CMakeFiles/FreeRTOS_Emulator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FreeRTOS_Emulator.dir/depend

