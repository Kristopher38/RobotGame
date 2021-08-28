cmake_minimum_required(VERSION 2.8)

project(robotgame)

link_directories("C:\\Libraries\\include\\lua")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (UNIX)
	set(CMAKE_CXX_FLAGS "-lX11 -lGL -lpthread -lpng -lstdc++fs -llua53")
endif (UNIX)

add_executable(${PROJECT_NAME} "main.cpp" "robotgame.cpp" "block.cpp" "programmableblock.cpp" "pge/imgui.cpp"
	"pge/imgui_demo.cpp"
	"pge/imgui_draw.cpp"
	"pge/imgui_widgets.cpp" "pge/imgui_tables.cpp" "pge/imgui_impl_opengl2.cpp")

if (WIN32)
	target_link_libraries(${PROJECT_NAME} user32.lib)
	target_link_libraries(${PROJECT_NAME} gdi32.lib)
	target_link_libraries(${PROJECT_NAME} opengl32.lib)
	target_link_libraries(${PROJECT_NAME} gdiplus.lib)
	target_link_libraries(${PROJECT_NAME} Shlwapi.lib)
	target_link_libraries(${PROJECT_NAME} dwmapi.lib)
	target_link_libraries(${PROJECT_NAME} liblua53.a)
endif (WIN32)


