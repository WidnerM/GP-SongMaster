cmake_minimum_required(VERSION 3.21)

# language standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_C_EXTENSIONS NO)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Let's choose a name and version for the extension. Change the name to your
# liking. The version should be a string like "1.0".
set(PROJECT_NAME "SongMasterExtension") 
set (PROJECT_VERSION "0.1.0")
set (PROJECT_TITLE "Song Master Extension")
set (PROJECT_DESCR "Does some Song Master stuff over OSC")
string(TIMESTAMP TODAY "%d/%m/%Y")
set (PROJECT_BUILD_DATE ${TODAY})
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

include("${CMAKE_CURRENT_LIST_DIR}/cmake/dependencies.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/juce-shared-library.cmake")

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/Version.h @ONLY)

# Define our library including sources, include directories and dependencies
juce_add_shared_library(${PROJECT_NAME})
target_sources(
  ${PROJECT_NAME}
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/LibMain.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/LibMain.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/LogWindow.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/LogWindow.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/juceOSC.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/CallbackSendmidi.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/CallbackSendmidi.h")

target_link_libraries(
  ${PROJECT_NAME} PRIVATE gigperformer::sdk::cpp juce::juce_gui_basics
                          juce::juce_core juce::juce_events juce::juce_graphics
                          juce::juce_osc)
target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(
  ${PROJECT_NAME} PRIVATE "JUCER_ENABLE_GPL_MODE=1"
                          "JUCE_DISPLAY_SPLASH_SCREEN=0")

# Install the extension on the development machine
install(
  TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION "${GIG_PERFORMER_EXTENSIONS_DIRECTORY}"
  RUNTIME DESTINATION "${GIG_PERFORMER_EXTENSIONS_DIRECTORY}")
