cmake_minimum_required(VERSION 3.5)

#project(kipod LANGUAGES CXX)
project(quacry)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(kipod)
set(SPDLOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/kipod/external_libs/spdlog/include/)
include_directories(${SPDLOG_DIR})
include_directories(kipod/include)
include_directories(kipod/siegel2021/include)
add_executable(quacry
  src/main.cpp

  src/quasicrystals_pointset.h
  src/quasicrystals_pointset.cpp
  src/quasicrystal.h
  src/quasicrystal.cpp
  src/quasicrystals_scene.h
  src/quasicrystals_scene.cpp
  src/quasicrystals_sidebar.h
  src/quasicrystals_sidebar.cpp
  src/quasicrystals_console.h
  src/quasicrystals_console.cpp
  src/quasicrystals_module.h
  src/quasicrystals_module.cpp
  src/quasicrystals_examples.h
  src/quasicrystals_examples.cpp
  src/quasicrystals_physical_window.h
  src/quasicrystals_physical_window.cpp
  src/quasicrystals_guimathcontrol.h
  src/quasicrystals_guimathcontrol.cpp



  src/math/lattice.cpp
  src/math/minkowski_embedding.cpp
  src/math/sl.cpp
  src/math/lattice.h		
  src/math/minkowski_embedding.h	
  src/math/sl.h
)

target_link_libraries(quacry PUBLIC kipod)

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
