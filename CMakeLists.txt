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
    src/stt-BmCode.c
    src/stt-BmBench.c
    src/stt-BmTree.c
    src/stt-BmVector.c
    src/fct-BmCondition.c
    src/fct-BmInferer.c
    src/fct-BmCriterion.c
    src/fct-BmEvaluator.c
    src/mdl-BmChain.c
    src/mdl-BmProcess.c
    src/slv-BmFunction.c
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
    test/tc-stt-BmCode.c
    test/tc-stt-BmBench.c
    test/tc-stt-BmTree.c
    test/tc-stt-BmVector.c
    test/tc-fct-BmCondition.c
    test/tc-fct-BmInferer.c
    test/tc-fct-BmCriterion.c
    test/tc-fct-BmEvaluator.c
    test/tc-mdl-BmChain.c
    test/tc-mdl-BmProcess.c
    test/tc-slv-BmFunction.c
    test/tc-slv-BmBasicPolicy.c
#    test/tc-cofeeRobot.c
    test/main-testrunner.c
)

add_executable(run-test-BbMm ${TEST_SRC_FILES})
target_link_libraries(run-test-BbMm bbmm ${CHECK_LIBS})

endif()
