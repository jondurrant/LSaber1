cmake_minimum_required(VERSION 3.12)

# Change your executable name to something creative!
set(NAME LSaber1) # <-- Name your project/executable here!

include(pico_sdk_import.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_CXX_ENABLE_RTTI 1)

# Initialize the SDK
pico_sdk_init()

include("lib/json-maker.cmake")
include("lib/tiny-json.cmake")

add_subdirectory(src)



set(TWIN_THING_PICO_CONFIG_PATH ${CMAKE_CURRENT_LIST_DIR}/src/config)

# Include required libraries
include("lib/twinThingPicoESP/twinThingPicoESP.cmake")
include("lib/FreeRTOS-Kernel.cmake")
#include("lib/lwesp.cmake")
include("lib/lwespFork.cmake")
include("lib/RtosComponents/RTOSComponents.cmake")
include("lib/PicoLED/PicoLed.cmake")






#Set up files for the release packages
install(CODE "execute_process(COMMAND $ENV{HOME}/bin/picoDeploy.sh ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.elf)")

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.uf2
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
