# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\chaima_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\chaima_autogen.dir\\ParseCache.txt"
  "chaima_autogen"
  )
endif()
