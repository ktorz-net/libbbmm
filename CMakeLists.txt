cmake_minimum_required(VERSION 3.00)
project(BbMm LANGUAGES C)

# Options:
cmake_policy(SET CMP0064 NEW)
option(TEST "Generate tests" OFF)
set(CMAKE_BUILD_TYPE Debug)

# Activate C99 :
SET(CMAKE_C_FLAGS "-std=c99" )
#SET(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -g -fsanitize=address")
SET(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -g")
SET(CMAKE_C_FLAGS_RELEASE "-Wall")

if(TEST)
  message("Test")
else()
  message("no Test")
endif()
message( ${CMAKE_BUILD_TYPE} )

# Dependancy :
# Check for unit-test:
# find_package(check REQUIRED)
# RayLib for viewer:
# find_package(raylib 2.0 REQUIRED)

# project configuration :
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src)

#------------#
## C O R E  ##
#------------#
set(SRC_FILES
    src/stt-basictools.c
    src/stt-BmCode.c
    src/stt-BmBench.c
    src/stt-BmTree.c
    src/stt-BmVector.c
#    src/val-BmGauge.c
#    src/val-BmEval.c
#    src/pbb-BmDistribution.c
#    src/pbb-BmCondition.c
#    src/pbb-BmTransition.c
#    src/mdl-BmDomain.c
#    src/mdl-BmSpace.c
#    src/mdl-BmSystem.c
)

# Build
add_library(bbmm SHARED ${SRC_FILES})
target_link_libraries(bbmm)

# Install
install(TARGETS bbmm LIBRARY DESTINATION lib)
install(FILES bbmm-structures.h DESTINATION include)
install(FILES bbmm-values.h DESTINATION include)
install(FILES bbmm-probabilities.h DESTINATION include)
install(FILES bbmm-models.h DESTINATION include)
install(FILES bbmm-solvers.h DESTINATION include)

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
#    test/tc-val-BmGauge.c
#    test/tc-val-BmEval.c
#    test/tc-pbb-BmDistribution.c
#    test/tc-pbb-BmCondition.c
#    test/tc-pbb-BmTransition.c
#    test/tc-mld-BmDomain.c
#    test/tc-mld-BmSpace.c
#    test/tc-mld-BmSystem.c
#    test/tc-cofeeRobot.c
    test/main-testrunner.c
)

add_executable(run-test-BbMm ${TEST_SRC_FILES})
target_link_libraries(run-test-BbMm bbmm ${CHECK_LIBS})

endif()
