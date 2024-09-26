get_filename_component(Cytnx_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

list(APPEND CMAKE_MODULE_PATH ${Cytnx_CMAKE_DIR})

# NOTE Had to use find_package because find_dependency does not support COMPONENTS or MODULE until 3.8.0

#find_dependency(Boost REQUIRED )

find_package(Boost REQUIRED )

list(REMOVE_AT CMAKE_MODULE_PATH -1)

if(NOT TARGET Cytnx::Cytnx)
    include("${Cytnx_CMAKE_DIR}/CytnxTargets.cmake")
endif()

set(Cytnx_Libraries Cytnx::Cytnx)
