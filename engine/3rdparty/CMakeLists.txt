set(third_party_folder "ThirdParty")

if(NOT TARGET glad)
    file(GLOB_RECURSE GLAD_SOURCES CONFIGURE_DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}/glad/*.c)
    add_library(glad STATIC ${GLAD_SOURCES})
    set_target_properties(glad PROPERTIES FOLDER ${third_party_folder}/glad)
    target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
endif()

if(NOT TARGET assimp)
    # add assimp headers
    file(GLOB_RECURSE ASSIMP_HEADERS CONFIGURE_DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}/assimp/include/*.h)
    add_library(assimp INTERFACE ${ASSIMP_HEADERS})
    target_include_directories(assimp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/assimp/include)
    set_target_properties(assimp PROPERTIES FOLDER ${third_party_folder}/assimp)
    
endif()

if(NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(glfw)
    set_target_properties(glfw PROPERTIES FOLDER ${third_party_folder}/glfw)
    # set_target_properties(update_mappings PROPERTIES FOLDER ${third_party_folder}/glfw)
endif()

#add stb_image as a library
if(NOT TARGET stb_image)
    file(GLOB STB_SOURCES CONFIGURE_DEPENDS  "${CMAKE_CURRENT_SOURCE_DIR}/stb/*.h")
    add_library(stb_image INTERFACE ${STB_SOURCES})
    target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb/include)
    set_target_properties(glfw PROPERTIES FOLDER ${third_party_folder}/stb)
endif()