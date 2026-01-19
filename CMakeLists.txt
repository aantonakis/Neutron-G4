cmake_minimum_required(VERSION 3.16)
project(neutron_argon_g4)

find_package(Geant4 REQUIRED ui_all vis_all)
find_package(ROOT REQUIRED)

include(${Geant4_USE_FILE})

include_directories(include)
include_directories(${ROOT_INCLUDE_DIRS})

file(GLOB sources src/*.cc)
file(GLOB headers include/*.hh)

add_executable(neutron_argon ${sources} ${headers})
#target_link_libraries(neutron_argon ${Geant4_LIBRARIES})
target_link_libraries(neutron_argon
    ${Geant4_LIBRARIES}
    ${ROOT_LIBRARIES}
)
