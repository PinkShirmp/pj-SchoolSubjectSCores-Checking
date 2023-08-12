cmake_minimum_required(VERSION 3.22.1)
project(o)

add_subdirectory(external/matplotlib)
add_subdirectory(external/json)
add_executable(${PROJECT_NAME} src/main.cpp)

find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
target_include_directories(${PROJECT_NAME}
    PUBLIC external/matplotlib
 #   PUBLIC usr/include
    PUBLIC include/
    PUBLIC external/json/single_include
)

target_link_directories(${PROJECT_NAME}
    PUBLIC /usr/local/bin
    
)
target_link_libraries(${PROJECT_NAME}  python3.10 matplotlib_cpp CURL::libcurl nlohmann_json::nlohmann_json)

