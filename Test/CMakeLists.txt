cmake_minimum_required (VERSION 3.4)

find_package(Misc)

set(SOURCES
    Hud.cpp
    Hud.h
    main.cpp
)

add_definitions(
    -DNOMINMAX
    -DWIN32_LEAN_AND_MEAN
    -DVC_EXTRALEAN
    -D_CRT_SECURE_NO_WARNINGS
    -D_SECURE_SCL=0
    -D_SCL_SECURE_NO_WARNINGS
)

add_executable(${PROJECT_NAME}_unit-test ${SOURCES})
target_link_libraries(${PROJECT_NAME}_unit-test PRIVATE ${PROJECT_NAME} Misc::Misc Wx DxUtility Dxx Math Time)

add_subdirectory(Wx)
add_subdirectory(DxUtility)
add_subdirectory(Math)
add_subdirectory(Time)
