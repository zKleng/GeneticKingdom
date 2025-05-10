cmake_minimum_required(VERSION 3.10)
project(GeneticKingdom)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ruta base de SFML (instalado vía Homebrew)
set(SFML_PATH "/usr/local")

# Incluir encabezados del proyecto
include_directories(include)

# Incluir encabezados de SFML
include_directories(${SFML_PATH}/include)

# Buscar todos los .cpp en src/ y subcarpetas
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Crear el ejecutable
add_executable(GeneticKingdom ${SOURCES})

# Linkear bibliotecas de SFML manualmente
target_link_libraries(GeneticKingdom
    ${SFML_PATH}/lib/libsfml-graphics.dylib
    ${SFML_PATH}/lib/libsfml-window.dylib
    ${SFML_PATH}/lib/libsfml-system.dylib
)

# Guardar ejecutable en la carpeta bin/
set_target_properties(GeneticKingdom PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# Poder ejercutar el programa desde consola con "make run"
add_custom_target(run
    COMMAND ${CMAKE_SOURCE_DIR}/bin/GeneticKingdom
    DEPENDS GeneticKingdom
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
