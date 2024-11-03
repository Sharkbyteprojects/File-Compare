add_executable (fileStructCmp "fileStructCmp.cpp" "fileStructCmp.h" "file.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET fileStructCmp PROPERTY CXX_STANDARD 20)
endif()
