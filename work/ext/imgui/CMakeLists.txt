add_library(imgui
	
	imconfig.h
	imgui.h
	imgui.cpp
	imgui_draw.cpp
	imgui_widgets.cpp
	imgui_internal.h

	imgui_impl_glfw.cpp
	imgui_impl_glfw.h
	imgui_impl_opengl3.cpp
	imgui_impl_opengl3.h
)

target_include_directories(imgui PUBLIC "${PROJECT_SOURCE_DIR}/ext/imgui")
