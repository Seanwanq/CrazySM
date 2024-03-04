cmake_minimum_required(VERSION 3.20.0)
project(MC_simulation_NPT VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

# add_executable(MC_simulation_NPT main.c absolute-path.c)
add_executable(MC_simulation_NPT npt.c absolute-path.c)

if (UNIX)
    target_link_libraries(MC_simulation_NPT m)
endif (UNIX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
