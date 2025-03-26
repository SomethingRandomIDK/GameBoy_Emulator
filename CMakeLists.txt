cmake_minimum_required(VERSION 3.10)

project(gbemu C)

include(GNUInstallDirs)

find_package(SDL2 REQUIRED)

add_executable(gbemu
    src/main.c
    src/timer.c
    src/io.c
    src/cpu.c
    src/cart.c
    src/bus.c
    src/gui.c
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
    add_custom_command(TARGET gbemu POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	    "${SDL2_LIBDIR}/SDL2.dll"
	    $<TARGET_FILE_DIR:gbemu>)
endif()

target_link_libraries(gbemu
    SDL2::SDL2
)

install(TARGETS gbemu
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

