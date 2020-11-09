
include_directories(${PROJECT_SOURCE_DIR}/src)

addtest(server_test test.cpp)
target_link_libraries(server_test parallel_stream Boost::serialization Boost::log )