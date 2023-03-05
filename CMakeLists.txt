cmake_minimum_required(VERSION 3.5)

project(midi-to-joy LANGUAGES C)

add_executable(midi-to-joy main.c)

target_sources(midi-to-joy PRIVATE uncrustify.cfg)

target_link_libraries(midi-to-joy /usr/lib/libbsd.so)

install(TARGETS midi-to-joy
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
