# CMakeList.txt : CMake project for Project1, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Project1 "Project1.cpp" "PatientQueue.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Project1 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
