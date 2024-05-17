cmake_minimum_required(VERSION 3.5)
project(car_road_line_detect)

add_executable(${PROJECT_NAME}
    src/videoShow_sub.cpp
    # src/videoShow_pub.cpp
)

find_package(OpenCV 4 REQUIRED)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        "include"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${OpenCV_LIBS}
)