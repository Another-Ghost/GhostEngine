@echo
call bootstrap-vcpkg
call vcpkg integrate install
call vcpkg install assimp
call vcpkg install glad
call vcpkg install glfw3
call vcpkg install glm
call vcpkg install ms-gltf
call vcpkg install stb
pause