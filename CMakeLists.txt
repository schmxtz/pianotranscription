cmake_minimum_required(VERSION 3.10)
project(pianotranscription VERSION 0.1 LANGUAGES C)

add_library(header_only_lib INTERFACE)
target_include_directories(header_only_lib INTERFACE include)

add_library(audio_lib STATIC lib/audio/audio.c)
target_include_directories(audio_lib PUBLIC lib/audio/audio.h)

add_executable(pianotranscription main.c)
target_link_libraries(pianotranscription PRIVATE audio_lib header_only_lib)