cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME CSJEffectCollection)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EFFECTCOLLECTION_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(THIRD_PARTY_DIR "${EFFECTCOLLECTION_ROOT_DIR}/3rdParty")

#[[
set(SOIL2_INCLUDE ${THIRD_PARTY_DIR}/SOIL2)
add_library(SOIL2 ${SOIL2_INCLUDE}/etc1_utils.c
                  ${SOIL2_INCLUDE}/image_DXT.c
                  ${SOIL2_INCLUDE}/image_helper.c
                  ${SOIL2_INCLUDE}/SOIL2.c)
set_target_properties(SOIL2 PROPERTIES FOLDER "ThirdParty/SOIL2")
#]]

project(${TARGET_NAME} VERSION 0.1 LANGUAGES CXX)

add_definitions("-DUNICODE" "-D_UNICODE")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES "src/*")

if(CMAKE_HOST_WIN32)
set(Native_LIBS shlwapi opengl32 glu32)
else()
set(Native_LIBS)
endif()

set(glew_include ${THIRD_PARTY_DIR}/glew/include)
set(glfw_include ${THIRD_PARTY_DIR}/glfw/include)
set(SOIL2_include ${THIRD_PARTY_DIR}/SOIL2)

add_executable(${TARGET_NAME}
    ${SOURCE_FILES}
)

include_directories(${EFFECTCOLLECTION_ROOT_DIR}/src ${THIRD_PARTY_DIR})

#target_link_libraries(${TARGET_NAME} PUBLIC ${glew_lib})
target_link_libraries(${TARGET_NAME} PUBLIC glfw)
target_link_libraries(${TARGET_NAME} PRIVATE ${Native_LIBS})
target_link_libraries(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:SOIL2>)
target_link_libraries(${TARGET_NAME} PUBLIC glew)

target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${glew_include}>)
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${glfw_include}>)

set_target_properties(${TARGET_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

add_subdirectory(${THIRD_PARTY_DIR})

include(GNUInstallDirs)
install(TARGETS ${TARGET_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
