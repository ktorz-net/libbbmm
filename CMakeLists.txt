cmake_minimum_required(VERSION 3.00)
project(BeMAge LANGUAGES C)

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
#    core-src/stt-wdCollection.c
#    core-src/stt-wdNet.c
#    core-src/stt-wdTree.c
#    core-src/pbb-wdDistribution.c
#    core-src/pbb-wdCondition.c
#    core-src/pbb-wdTransition.c
#    core-src/mdl-wdDomain.c
#    core-src/mdl-wdSpace.c
#    core-src/mdl-wdModel.c
)

# Build
add_library(bemage SHARED ${SRC_FILES})
target_link_libraries(bemage)

# Install
install(TARGETS bemage LIBRARY DESTINATION lib)
install(FILES bemage-stt.h DESTINATION include)
install(FILES bemage-pbb.h DESTINATION include)
install(FILES bemage-mdl.h DESTINATION include)
install(FILES bemage-slv.h DESTINATION include)

#-----------#
## T E S T ##
#-----------#
if(TEST)

set(CHECK_LIBS check subunit m rt pthread)
set(TEST_SRC_FILES 
    core-test/tc-stt-BmCode.c
#    core-test/tc-stt-wdCollection.c
#    core-test/tc-stt-wdNet.c
#    core-test/tc-stt-wdTree.c
#    core-test/tc-pbb-wdDistribution.c
#    core-test/tc-pbb-wdCondition.c
#    core-test/tc-pbb-wdTransition.c
#    core-test/tc-mld-wdDomain.c
#    core-test/tc-mld-wdSpace.c
#    core-test/tc-mld-wdModel.c
#    core-test/tc-cofeeRobot.c
    core-test/main-testrunner.c
)

add_executable(test-BeMAge ${TEST_SRC_FILES})
target_link_libraries(test-BeMAge bemage ${CHECK_LIBS})

endif()
