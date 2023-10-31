# Install script for directory: F:/FlappyAceWin/deps/ace

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "c:/Users/NZJeu/.vscode/extensions/bartmanabyss.amiga-debug-1.7.4/bin/win32/opt/usr")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Users/NZJeu/.vscode/extensions/bartmanabyss.amiga-debug-1.7.4/bin/win32/opt/bin/m68k-amiga-elf-objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/objects-Debug/ace" TYPE FILE FILES
    "src/ace/managers/audio.c.obj"
    "src/ace/managers/blit.c.obj"
    "src/ace/managers/bob.c.obj"
    "src/ace/managers/copper.c.obj"
    "src/ace/managers/game.c.obj"
    "src/ace/managers/joy.c.obj"
    "src/ace/managers/key.c.obj"
    "src/ace/managers/log.c.obj"
    "src/ace/managers/memory.c.obj"
    "src/ace/managers/mouse.c.obj"
    "src/ace/managers/ptplayer.c.obj"
    "src/ace/managers/rand.c.obj"
    "src/ace/managers/sprite.c.obj"
    "src/ace/managers/state.c.obj"
    "src/ace/managers/system.c.obj"
    "src/ace/managers/timer.c.obj"
    "src/ace/managers/viewport/camera.c.obj"
    "src/ace/managers/viewport/scrollbuffer.c.obj"
    "src/ace/managers/viewport/simplebuffer.c.obj"
    "src/ace/managers/viewport/tilebuffer.c.obj"
    "src/ace/utils/bitmap.c.obj"
    "src/ace/utils/bmframe.c.obj"
    "src/ace/utils/chunky.c.obj"
    "src/ace/utils/custom.c.obj"
    "src/ace/utils/dir.c.obj"
    "src/ace/utils/extview.c.obj"
    "src/ace/utils/file.c.obj"
    "src/ace/utils/font.c.obj"
    "src/ace/utils/palette.c.obj"
    "src/ace/utils/sprite.c.obj"
    "src/ace/utils/string.c.obj"
    "src/ace/utils/tag.c.obj"
    "src/fixmath/fix16.c.obj"
    "src/fixmath/fix16_exp.c.obj"
    "src/fixmath/fix16_sqrt.c.obj"
    "src/fixmath/fix16_str.c.obj"
    "src/fixmath/fix16_trig.c.obj"
    "src/fixmath/fract32.c.obj"
    "src/fixmath/uint32.c.obj"
    "src/bartman/gcc8_a_support.s.obj"
    "src/bartman/gcc8_c_support.c.obj"
    "src/mini_std/ctype.c.obj"
    "src/mini_std/errno.c.obj"
    "src/mini_std/intrin.c.obj"
    "src/mini_std/printf.c.obj"
    "src/mini_std/stdio_file.c.obj"
    "src/mini_std/stdio_putchar.c.obj"
    "src/mini_std/stdlib.c.obj"
    "src/mini_std/string.c.obj"
    "src/mini_std/strtoul.c.obj"
    FILES_FROM_DIR "F:/FlappyAceWin/build/ace/CMakeFiles/ace.dir/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("F:/FlappyAceWin/build/ace/CMakeFiles/ace.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace" TYPE FILE FILES
    "F:/FlappyAceWin/deps/ace/include/ace/macros.h"
    "F:/FlappyAceWin/deps/ace/include/ace/types.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace/generic" TYPE FILE FILES
    "F:/FlappyAceWin/deps/ace/include/ace/generic/main.h"
    "F:/FlappyAceWin/deps/ace/include/ace/generic/screen.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace/utils" TYPE FILE FILES
    "F:/FlappyAceWin/deps/ace/include/ace/utils/bitmap.h"
    "F:/FlappyAceWin/deps/ace/include/ace/utils/bmframe.h"
    "F:/FlappyAceWin/deps/ace/include/ace/utils/chunky.h"
    "F:/FlappyAceWin/deps/ace/include/ace/utils/custom.h"
    "F:/FlappyAceWin/deps/ace/include/ace/utils/dir.h"
    "F:/FlappyAceWin/deps/ace/include/ace/utils/endian.h"
    "F:/FlappyAceWin/deps/ace/include/ace/utils/extview.h"
    "F:/FlappyAceWin/deps/ace/include/ace/utils/file.h"
    "F:/FlappyAceWin/deps/ace/include/ace/utils/font.h"
    "F:/FlappyAceWin/deps/ace/include/ace/utils/mini_std.h"
    "F:/FlappyAceWin/deps/ace/include/ace/utils/palette.h"
    "F:/FlappyAceWin/deps/ace/include/ace/utils/sprite.h"
    "F:/FlappyAceWin/deps/ace/include/ace/utils/string.h"
    "F:/FlappyAceWin/deps/ace/include/ace/utils/tag.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace/managers" TYPE FILE FILES
    "F:/FlappyAceWin/deps/ace/include/ace/managers/audio.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/blit.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/bob.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/copper.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/game.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/joy.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/key.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/log.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/memory.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/mouse.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/ptplayer.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/rand.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/sprite.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/state.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/system.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/timer.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace/managers/viewport" TYPE FILE FILES
    "F:/FlappyAceWin/deps/ace/include/ace/managers/viewport/camera.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/viewport/scrollbuffer.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/viewport/simplebuffer.h"
    "F:/FlappyAceWin/deps/ace/include/ace/managers/viewport/tilebuffer.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/fixmath" TYPE FILE FILES
    "F:/FlappyAceWin/deps/ace/include/fixmath/fix16.h"
    "F:/FlappyAceWin/deps/ace/include/fixmath/fix16_trig_sin_lut.h"
    "F:/FlappyAceWin/deps/ace/include/fixmath/fixmath.h"
    "F:/FlappyAceWin/deps/ace/include/fixmath/fract32.h"
    "F:/FlappyAceWin/deps/ace/include/fixmath/int64.h"
    "F:/FlappyAceWin/deps/ace/include/fixmath/uint32.h"
    )
endif()

