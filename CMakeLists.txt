cmake_minimum_required(VERSION 3.10)

project(gbemu C)

include(GNUInstallDirs)

find_package(SDL2 REQUIRED)

if(WIN32)
    add_definitions(-DSDL_MAIN_HANDLED)
endif()

add_executable(gbemu
    src/main.c
    src/cpu.c
    src/cart.c
    src/bus.c
    src/interpreter.c
    src/logging/log.c
)

target_link_libraries(gbemu
    SDL2::SDL2
)

install(TARGETS gbemu
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

