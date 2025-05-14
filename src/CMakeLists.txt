file(GLOB SOURCES "*.cpp")

add_executable(reactor ${SOURCES})

target_link_libraries(reactor pthread)