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
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/core-src)

#------------#
## C O R E  ##
#------------#
set(SRC_FILES
    core-src/stt-basictools.c
    core-src/stt-BmCode.c
    core-src/stt-BmBench.c
    core-src/stt-BmNet.c
    core-src/stt-BmTree.c
    core-src/pbb-BmDistribution.c
    core-src/pbb-BmCondition.c
    core-src/pbb-BmTransition.c
    core-src/mdl-BmDomain.c
    core-src/mdl-BmSpace.c
    core-src/mdl-BmSystem.c
)

# Build
add_library(bbmm SHARED ${SRC_FILES})
target_link_libraries(bbmm)

# Install
install(TARGETS bbmm LIBRARY DESTINATION lib)
install(FILES bbmm-stt.h DESTINATION include)
install(FILES bbmm-pbb.h DESTINATION include)
install(FILES bbmm-mdl.h DESTINATION include)
install(FILES bbmm-slv.h DESTINATION include)

#-----------#
## T E S T ##
#-----------#
if(TEST)

set(CHECK_LIBS check subunit m rt pthread)
set(TEST_SRC_FILES 
    core-test/tc-stt-BmCode.c
    core-test/tc-stt-BmBench.c
    core-test/tc-stt-BmNet.c
    core-test/tc-stt-BmTree.c
    core-test/tc-pbb-BmDistribution.c
    core-test/tc-pbb-BmCondition.c
    core-test/tc-pbb-BmTransition.c
    core-test/tc-mld-BmDomain.c
    core-test/tc-mld-BmSpace.c
    core-test/tc-mld-BmSystem.c
#    core-test/tc-cofeeRobot.c
    core-test/main-testrunner.c
)

add_executable(test-BbMm ${TEST_SRC_FILES})
target_link_libraries(test-BbMm bbmm ${CHECK_LIBS})

endif()
