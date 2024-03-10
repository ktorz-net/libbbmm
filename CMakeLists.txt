cmake_minimum_required(VERSION 3.00)
project(BbMm LANGUAGES C)

# Options:
cmake_policy(SET CMP0064 NEW)
option(TEST "Generate tests" OFF)
set(CMAKE_BUILD_TYPE RELEASE)

if(TEST)
  set(CMAKE_BUILD_TYPE DEBUG)
  message("Build_type: Debug with Test - ON ")
else()
  message("Build_type: Release")
endif()

# Activate C99 :
SET(CMAKE_C_FLAGS "-std=c99" )
#SET(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -g -fsanitize=address")
SET(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -g")
SET(CMAKE_C_FLAGS_RELEASE "-Wall")

# project configuration :
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src)

#------------#
## C O R E  ##
#------------#
set(SRC_FILES
#STRUCTURE MODULE:
  src/stt-BmCode.c
  src/stt-BmVector.c
  src/stt-BmBench.c
  src/stt-BmTree.c
# FUNCTION MODULE:
  src/fct-BmValueFct.c
  src/fct-BmFunction.c
# COMPONENT MODULE:
  src/cpn-BmCondition.c
  src/cpn-BmInferer.c
  src/cpn-BmEvaluator.c
# SOLVER MODULE:
  src/slv-BmBasicPolicy.c
)

# Build
add_library(bbmm SHARED ${SRC_FILES})
target_link_libraries(bbmm)

# Install
install(TARGETS bbmm LIBRARY DESTINATION lib)
install(FILES src/bbmm.h DESTINATION include)

#-----------#
## T E S T ##
#-----------#
if(TEST)

set(CHECK_LIBS check subunit m rt pthread)
set(TEST_SRC_FILES 
#STRUCTURE MODULE:
  test/tc-stt-BmCode.c
  test/tc-stt-BmVector.c
  test/tc-stt-BmBench.c
  test/tc-stt-BmTree.c
# FUNCTION MODULE:
  test/tc-fct-BmValueFct.c
  test/tc-fct-BmFunction.c
# COMPONENT MODULE:
  test/tc-cpn-BmCondition.c
  test/tc-cpn-BmInferer.c
  test/tc-cpn-BmEvaluator.c
# SOLVER MODULE:
  test/tc-slv-BmBasicPolicy.c
# TEST-CASES:
#    test/tc-cofeeRobot.c
# MAIN:
  test/main-testrunner.c
)

add_executable(run-test-BbMm ${TEST_SRC_FILES})
target_link_libraries(run-test-BbMm bbmm ${CHECK_LIBS})

endif()
