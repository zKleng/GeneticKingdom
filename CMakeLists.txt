cmake_minimum_required(VERSION 3.10)
project(GeneticKingdom)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir encabezados del proyecto
include_directories(include)

# Buscar todos los .cpp en src/ y subcarpetas
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Crear el ejecutable
add_executable(GeneticKingdom ${SOURCES})

# Configuracion del SFML
find_package(SFML 2.5 COMPONENTS graphics window system QUIET)

if(SFML_FOUND)
    message(STATUS "SFML found via find_package")
    include_directories(${SFML_INCLUDE_DIRS})
    target_link_libraries(GeneticKingdom ${SFML_LIBRARIES})
else()
    message(WARNING "SFML not found with find_package, using manual paths")

    # Rutas manuales (hay que adaptarlo a cada SO, sino no funciona, manualmente hay que cambiar las rutas)
    set(SFML_PATH "/usr") #cambiar esta ruta
    include_directories(${SFML_PATH}/include)

    target_link_libraries(GeneticKingdom # y estas 3 otras
        sfml-graphics
        sfml-window
        sfml-system
    )
endif()

# Guardar ejecutable en la carpeta bin/
set_target_properties(GeneticKingdom PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# Ejecutar el juego con make run
add_custom_target(run
    COMMAND ${CMAKE_SOURCE_DIR}/bin/GeneticKingdom
    DEPENDS GeneticKingdom
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Limpiar archivos intermedios con make clean-all
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/build/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/build/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/build/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/build/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/build/*.o
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
    COMMENT "Cleaning contents of build/ and bin/, keeping folders"
)
