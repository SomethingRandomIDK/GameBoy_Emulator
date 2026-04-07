cmake_minimum_required(VERSION 3.10)

project(gbemu C)

include(GNUInstallDirs)

if (MSVC)
    find_package(SDL2 CONFIG REQUIRED)
else()
    find_package(SDL2 REQUIRED)
    set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
endif()

add_executable(gbemu
    src/main.c
    src/timer.c
    src/io.c
    src/apu.c
    src/cpu.c
    src/cart.c
    src/bus.c
    src/gui.c
    src/ppu.c
    src/lcd.c
    src/serial.c
    src/interrupt.c
    src/interpreter.c
    src/logging/log.c
)

if(WIN32)
    add_definitions(-DSDL_MAIN_HANDLED)
    include_directories(${SDL2_INCLUDE_DIR})
    link_directories(${SDL2_LIBDIR})
    target_link_libraries(gbemu
        SDL2::SDL2
    )
else()
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(gbemu ${SDL2_LIBRARIES})
endif()

install(TARGETS gbemu
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

