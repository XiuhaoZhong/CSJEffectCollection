set(third_party_folder "ThirdParty")

if(NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(glfw)
    set_target_properties(glfw PROPERTIES FOLDER ${third_party_folder}/glfw)
    set_target_properties(update_mappings PROPERTIES FOLDER ${third_party_folder}/glfw)
endif()

if(NOT TARGET glew)
    option(glew-cmake_BUILD_SHARED "" OFF)
    option(glew-cmake_BUILD_STATIC "" ON)
    option(ONLY_LIBS "" ON)
    option(USE_GLU "" ON)
    add_subdirectory(glew)
    set_target_properties(glew PROPERTIES FOLDER ${third_party_folder}/glew)
endif()

if(NOT TARGET SOIL2)
set(SOIL2_INCLUDE SOIL2)
add_library(SOIL2 ${SOIL2_INCLUDE}/etc1_utils.c
                  ${SOIL2_INCLUDE}/image_DXT.c
                  ${SOIL2_INCLUDE}/image_helper.c
                  ${SOIL2_INCLUDE}/SOIL2.c)
set_target_properties(SOIL2 PROPERTIES FOLDER "ThirdParty/SOIL2")
endif()