@echo
::git clone https://github.com/microsoft/vcpkg
cd c:\vcpkg
::call bootstrap-vcpkg
::call vcpkg integrate install
call vcpkg install assimp:x64-windows
call vcpkg install glad:x64-windows
call vcpkg install glfw3:x64-windows
call vcpkg install glm:x64-windows
call vcpkg install ms-gltf:x64-windows
call vcpkg install stb:x64-windows
pause