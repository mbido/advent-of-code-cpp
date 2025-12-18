cmake_minimum_required(VERSION 3.10)
project(AdventOfCodeCPP CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Optimisations pour la performance
if(MSVC)
    add_compile_options(/O2 /arch:AVX2)
else()
    add_compile_options(-O3 -march=native -flto -Wall -Wextra)
    add_link_options(-flto)
endif()

include_directories(include)

# Support OpenMP
find_package(OpenMP)

# Scan récursif pour trouver tous les jours dans src/YEAR/dayDAY/main.cpp
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*/day*/main.cpp)

foreach(SOURCE_FILE ${SOURCES})
    # Extraction de l'année et du jour à partir du chemin src/YEAR/dayDAY/main.cpp
    if(SOURCE_FILE MATCHES "src/([0-9]+)/day([0-9]+)/main.cpp")
        set(YEAR ${CMAKE_MATCH_1})
        set(DAY ${CMAKE_MATCH_2})
        set(EXE_NAME "aoc_${YEAR}_${DAY}")
        
        add_executable(${EXE_NAME} ${SOURCE_FILE})
        
        if(OpenMP_CXX_FOUND)
            target_link_libraries(${EXE_NAME} PUBLIC OpenMP::OpenMP_CXX)
        endif()

        # On définit le répertoire de travail pour que les chemins relatifs vers data/ fonctionnent
        set_target_properties(${EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
    endif()
endforeach()