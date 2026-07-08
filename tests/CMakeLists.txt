cmake_minimum_required(VERSION 3.22)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4
    GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(Catch2)

add_executable(BackroomsMufflerTests
    BackroomsProcessorTests.cpp
    ${CMAKE_SOURCE_DIR}/source/dsp/BackroomsProcessor.cpp)

target_include_directories(BackroomsMufflerTests PRIVATE ${CMAKE_SOURCE_DIR}/source)

target_link_libraries(BackroomsMufflerTests
    PRIVATE
        Catch2::Catch2WithMain
        juce::juce_dsp
        juce::juce_audio_basics
        juce::juce_audio_formats)

include(Catch)
catch_discover_tests(BackroomsMufflerTests)
