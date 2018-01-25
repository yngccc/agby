@echo off
echo. 

call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64

if not exist "%~dp0\build" mkdir "%~dp0\build"
pushd "%~dp0\build"

set flags=/nologo /Od /W3 /Zo /Z7 /EHa /FC /D_CRT_SECURE_NO_WARNINGS /D "_ITERATOR_DEBUG_LEVEL=0"
set dirs=/I ..\vendor\include /I ..\vendor\include\fbx /I %VULKAN_SDK%\include /link /LIBPATH:..\vendor\lib\windows
set libs=user32.lib gdi32.lib Shcore.lib Wtsapi32.lib

rem cl ..\src\codegen.cpp %flags% %libs%
rem if not %ERRORLEVEL% EQU 0 (
rem 	echo compiling error: "codegen.cpp"
rem 	exit /b
rem )
rem codegen ..\src\common.cpp ..\src\math.cpp ..\src\vulkan.cpp ..\src\assets.cpp ..\src\menu.cpp ..\src\level.cpp ..\src\editor.cpp

start /b cl ..\src\editor.cpp %flags% /MD %dirs% %libs%
start /b cl ..\src\game.cpp %flags% /MD %dirs% %libs%
start /b cl ..\src\import.cpp %flags% /MD %dirs% %libs% libfbxsdk.lib nvtt.lib
start /b cl ..\src\lightmap.cpp %flags% /MT %dirs% %libs% uvatlas.lib directxmesh.lib
start /b cl ..\src\test.cpp %flags% /MD %dirs% %libs%

rmdir /s /q shaders
mkdir shaders
pushd "..\src\shaders"
for %%G in (.vert, .frag) do forfiles /m *%%G /c "cmd /c %VULKAN_SDK%\Bin\glslc.exe -o ..\..\build\shaders\\@file.spv @file & if ERRORLEVEL 1 echo file name: @file"
popd

copy /y ..\vendor\lib\windows\libfbxsdk.dll libfbxsdk.dll >NUL
copy /y ..\vendor\lib\windows\nvtt.dll nvtt.dll >NUL

if not exist "assets" mkdir "assets"
pushd "assets"
xcopy /e /i /y ..\agby_assets\fonts fonts >NUL
popd

popd
