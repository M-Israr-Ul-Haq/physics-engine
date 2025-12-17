cmake_minimum_required(VERSION 3.16)
project(physics_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML 3.0 with correct component names
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# Collect all source files from physics_engine directory
file(GLOB_RECURSE SOURCES 
    "physics_engine/*.cpp"
    "physics_engine/Atomic_Chaos/*.cpp"
    "physics_engine/Orbital_Chaos/*.cpp"
    "physics_engine/Pendulum_Chaos/*.cpp"
)

# Add the main executable
add_executable(physics_engine ${SOURCES})

# Link SFML libraries (note: lowercase in target names)
target_link_libraries(physics_engine PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

# Include directories
target_include_directories(physics_engine PRIVATE 
    "${CMAKE_SOURCE_DIR}/physics_engine"
)
