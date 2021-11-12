cmake_minimum_required(VERSION 3.0.0)
project(asio_exploring VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(asio_exploring asio_exploring.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(DIR_SRC 
    ${PROJECT_SOURCE_DIR}/src
)
include(CPack)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(timer_synchronously ${DIR_SRC}/timer_synchronously.cpp)
add_executable(timer_asynchronously ${DIR_SRC}/timer_asynchronously.cpp)
add_executable(timer3_asynchronously ${DIR_SRC}/timer3_asynchronously.cpp)
add_executable(timer4_asynchronously ${DIR_SRC}/timer4_asynchronously.cpp)
add_executable(timer5_asynchronously ${DIR_SRC}/timer5_asynchronously.cpp)

target_link_libraries(timer_synchronously pthread)
target_link_libraries(timer_asynchronously pthread)
target_link_libraries(timer3_asynchronously pthread)
target_link_libraries(timer4_asynchronously pthread)
target_link_libraries(timer5_asynchronously pthread boost_thread)

