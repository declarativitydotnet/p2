ADD_LIBRARY(p2oni oni.C oniStageProcessor.C)

SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)  # use "conventional" if syntax
IF(APPLE)
  SET_SOURCE_FILES_PROPERTIES(oniStageProcessor.C PROPERTIES COMPILE_FLAGS "-DLIB_SUFFIX=dylib")
ELSEIF(WIN32)
# Pretend this will help. ;)
  SET_SOURCE_FILES_PROPERTIES(oniStageProcessor.C PROPERTIES COMPILE_FLAGS "-DLIB_SUFFIX=dll")
ELSE()
  SET_SOURCE_FILES_PROPERTIES(oniStageProcessor.C PROPERTIES COMPILE_FLAGS "-DLIB_SUFFIX=so")
ENDIF()
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS false)

ADD_LIBRARY(oniecho oni-sample.c)

# look for stasis

FIND_LIBRARY(STASIS_LIBRARY NAMES stasis)
IF(STASIS_LIBRARY)
  ADD_LIBRARY(onistasis stasis.c stasisTypes.C )
  TARGET_LINK_LIBRARIES(onistasis stasis rw)
ENDIF(STASIS_LIBRARY)

TARGET_LINK_LIBRARIES(p2oni p2stages p2core)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

INSTALL(TARGETS p2oni LIBRARY DESTINATION lib)
