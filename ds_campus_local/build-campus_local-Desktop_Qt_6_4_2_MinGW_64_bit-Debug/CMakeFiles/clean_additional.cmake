# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\campus_local_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\campus_local_autogen.dir\\ParseCache.txt"
  "campus_local_autogen"
  )
endif()
