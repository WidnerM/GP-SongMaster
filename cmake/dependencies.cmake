include(FetchContent)

function(fetch_dependency name)
  FetchContent_GetProperties(${name})
  if(NOT ${name}_POPULATED)
    message(STATUS "Fetching and configuring ${name}...")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    FetchContent_Populate(${name})
    add_subdirectory(
      ${${name}_SOURCE_DIR}
      ${${name}_BINARY_DIR}
      # we have to set EXCLUDE_FROM_ALL so that dependent targets are not
      # included into the install trees.
      EXCLUDE_FROM_ALL)

    list(POP_BACK CMAKE_MESSAGE_INDENT)
  endif()
endfunction(fetch_dependency)

FetchContent_Declare(
  gp-sdk
  GIT_REPOSITORY https://github.com/gigperformer/gp-sdk.git
  # GIT_TAG 756ecd249f1ff00f3d3a5955769408f74c9fa453 # sdk v60
  GIT_TAG edc4313339dbb1526ccc5602b2ca6b0144ea199d # sdk v 63
)
fetch_dependency(gp-sdk)

FetchContent_Declare(
  juce
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  # GIT_TAG 2f980209cc4091a4490bb1bafc5d530f16834e58 # v6.1.6
  GIT_TAG 29396c22c93392d6738e021b83196283d6e4d850 # v8.0.12
)
fetch_dependency(juce)
