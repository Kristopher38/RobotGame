cmake_minimum_required(VERSION 2.8)

project(robotgame)

if (WIN32)
	link_directories("C:\\Libraries\\include\\lua")
endif (WIN32)

macro(print_all_variables)
	message(STATUS "print_all_variables------------------------------------------{")
	get_cmake_property(_variableNames VARIABLES)
	foreach (_variableName ${_variableNames})
		message(STATUS "${_variableName}=${${_variableName}}")
	endforeach()
	message(STATUS "print_all_variables------------------------------------------}")
endmacro()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (UNIX)
	#if (EMSCRIPTEN)
	#	set (CMAKE_CXX_FLAGS "-std=c++17 -O2 -L/home/kris/Downloads/Lua/lua-5.4.3 -llua -s ALLOW_MEMORY_GROWTH=1 -s FULL_ES3=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 --preload-file ./assets --preload-file ./examples")
	#	set (CMAKE_EXECUTABLE_SUFFIX ".html")
	#endif (EMSCRIPTEN)
	set(CMAKE_CXX_FLAGS "-llua -lX11 -lGL -lpthread -lpng -lstdc++fs")
endif (UNIX)

if (WIN32)
	set(IMGUI_IMPL "pge/imgui_impl_opengl2.cpp")
endif (WIN32)

if (UNIX)
	set(IMGUI_IMPL "pge/imgui_impl_opengl2.cpp")
endif (UNIX)

add_executable(${PROJECT_NAME}
	"main.cpp"
	"robotgame.cpp"
	"uistate.cpp"
	"spritemanager.cpp"
	"map.cpp"
	"robot.cpp"
	"block.cpp"
	"programmableblock.cpp"
	"buttonblock.cpp"
	"diodeblock.cpp"
	"motorblock.cpp"
	"mapblock.cpp"
	"radarblock.cpp"
	"pge/imgui.cpp"
	"pge/imgui_demo.cpp"
	"pge/imgui_draw.cpp"
	"pge/imgui_widgets.cpp"
	"pge/imgui_tables.cpp"
	"pge/imgui_stdlib.cpp"
	${IMGUI_IMPL})

if (WIN32)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-static-libstdc++")
	target_link_libraries(${PROJECT_NAME} user32.lib)
	target_link_libraries(${PROJECT_NAME} gdi32.lib)
	target_link_libraries(${PROJECT_NAME} opengl32.lib)
	target_link_libraries(${PROJECT_NAME} gdiplus.lib)
	target_link_libraries(${PROJECT_NAME} Shlwapi.lib)
	target_link_libraries(${PROJECT_NAME} dwmapi.lib)
	target_link_libraries(${PROJECT_NAME} liblua.a)
endif (WIN32)

#print_all_variables()

