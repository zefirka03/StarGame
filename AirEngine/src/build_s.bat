em++ -O1 main.cpp render/Shader.cpp -L"libs/" -I"../../dep/box2d/include/" -I"../../dep/imgui/" -I"../../dep/glm/" -I"../../dep/SOIL/include/" -I"../../dep/entt/" -limgui -lSOIL -lbox2d -o main1.html -sUSE_GLFW=3 -sUSE_WEBGL2=1 -sFULL_ES3 -sWASM=3 -sALLOW_MEMORY_GROWTH=1 --emrun --preload-file "../img"