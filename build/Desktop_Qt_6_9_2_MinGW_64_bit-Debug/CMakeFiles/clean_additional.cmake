# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\rahma_mechri_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\rahma_mechri_autogen.dir\\ParseCache.txt"
  "rahma_mechri_autogen"
  )
endif()
