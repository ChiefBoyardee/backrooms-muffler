cmake_minimum_required(VERSION 3.22)

project(BackroomsMuffler VERSION 0.3.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(JUCE)

if(APPLE)
    set(BACKROOMS_FORMATS VST3 AU Standalone)
else()
    set(BACKROOMS_FORMATS VST3 Standalone)
endif()

juce_add_plugin(BackroomsMuffler
    COMPANY_NAME "Backrooms Audio"
    BUNDLE_ID "com.backroomsaudio.backroomsmuffler"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Bkrm
    PLUGIN_CODE Bmuf
    FORMATS ${BACKROOMS_FORMATS}
    PRODUCT_NAME "Backrooms Muffler")

juce_add_binary_data(BackroomsAssets
    SOURCES
        assets/ui/wallpaper_tile.png
        assets/ui/carpet_tile.png
        assets/ui/ceiling_tile.png)

target_sources(BackroomsMuffler
    PRIVATE
        source/PluginProcessor.cpp
        source/PluginEditor.cpp
        source/dsp/BackroomsProcessor.cpp)

target_compile_definitions(BackroomsMuffler
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0)

target_link_libraries(BackroomsMuffler
    PRIVATE
        BackroomsAssets
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

target_include_directories(BackroomsMuffler PRIVATE source)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
