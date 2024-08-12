cmake_minimum_required(VERSION 3.10)

project(GBEmu)

include(GNUInstallDirs)

add_executable(gbemu
    src/main.c
)

target_link_libraries(gbemu
    -lSDL2
)

install(TARGETS gbemu
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

