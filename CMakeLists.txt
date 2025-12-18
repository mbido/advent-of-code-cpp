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

# Support fmt
find_package(fmt CONFIG)
if(NOT fmt_FOUND)
    find_package(fmt)
endif()
if(fmt_FOUND)
    message(STATUS "Found fmt: ${fmt_DIR}")
else()
    message(WARNING "fmt not found!")
endif()

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
        
        if(fmt_FOUND)
            target_link_libraries(${EXE_NAME} PUBLIC fmt::fmt)
            # S'assurer que les en-têtes sont trouvés même si fmt::fmt ne propage pas correctement
            get_target_property(fmt_include_dir fmt::fmt INTERFACE_INCLUDE_DIRECTORIES)
            if(fmt_include_dir)
                target_include_directories(${EXE_NAME} PUBLIC ${fmt_include_dir})
            endif()
        endif()

        # On définit le répertoire de travail pour que les chemins relatifs vers data/ fonctionnent
        set_target_properties(${EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
    endif()
endforeach()