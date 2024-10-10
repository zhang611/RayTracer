
message(STATUS "Building Srouce")
set(SOURCE_DIR ${PROJECT_ROOT_DIR}/source)


# add header files
include_directories(${SOURCE_DIR}/core)
include_directories(${SOURCE_DIR}/core/math)
include_directories(${SOURCE_DIR}/render)
include_directories(${SOURCE_DIR}/geometry)
include_directories(${SOURCE_DIR}/material)
include_directories(${SOURCE_DIR}/framework)

file(GLOB_RECURSE HEADER_FILES ${SOURCE_DIR}/*.h ${SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp )

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# add static library
target_link_libraries(${PROJECT_NAME} PRIVATE stb_image glad)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

message(STATUS "Building Srouce - done")

# copy engine/asset folder to build folder
set(DEBUG_ASSET_DIR ${CMAKE_CURRENT_BINARY_DIR}/Debug/asset)
file(REMOVE_RECURSE ${DEBUG_ASSET_DIR}) # clear target dir
file(COPY ${RESOURCE_DIR}/ DESTINATION ${DEBUG_ASSET_DIR})