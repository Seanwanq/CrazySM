cmake_minimum_required(VERSION 3.15.0)
project(monte_carlo_simulation_of_hard_spheres VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

include_directories(include)

# add_executable(monte_carlo_simulation_of_hard_spheres main.c)
add_executable(monte_carlo_simulation_of_hard_spheres hs-nvt.c)

if (UNIX)
    target_link_libraries(monte_carlo_simulation_of_hard_spheres m)
endif (UNIX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
