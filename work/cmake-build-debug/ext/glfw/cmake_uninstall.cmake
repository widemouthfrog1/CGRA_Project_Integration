
if (NOT EXISTS "/home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug/ext/glfw/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: \"/home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug/ext/glfw/install_manifest.txt\"")
endif()

file(READ "/home/hoefsldyla/Documents/CLionCopy/work/cmake-build-debug/ext/glfw/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")

foreach (file ${files})
  message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  if (EXISTS "$ENV{DESTDIR}${file}")
    exec_program("/am/embassy/vol/x6/jetbrains/apps/CLion/ch-0/191.6183.77/bin/cmake/linux/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif()
  elseif (IS_SYMLINK "$ENV{DESTDIR}${file}")
    EXEC_PROGRAM("/am/embassy/vol/x6/jetbrains/apps/CLion/ch-0/191.6183.77/bin/cmake/linux/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing symlink \"$ENV{DESTDIR}${file}\"")
    endif()
  else()
    message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
  endif()
endforeach()

