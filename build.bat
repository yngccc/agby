@echo off
echo. 

call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64

where /q ctags
if ERRORLEVEL 0 (
pushd "%~dp0\src"
	start /b ctags -f .tags ^
	gpk.cpp codegen.cpp common.cpp editor.cpp game.cpp import.cpp level.cpp lightmap.cpp math.cpp platform_windows.cpp test.cpp vulkan.cpp ^
	..\\vendor\\include\\imgui\\imgui.h %VULKAN_SDK%\\include\\vulkan\\vulkan_core.h ..\\vendor\\include\\tinygltf\\tiny_gltf.h
popd	
)

if not exist "%~dp0\build" mkdir "%~dp0\build"
pushd "%~dp0\build"

set flags=/nologo /Od /Z7 /MD /EHsc /W3 /GS /sdl /FC /D_CRT_SECURE_NO_WARNINGS /D "_ITERATOR_DEBUG_LEVEL=0"
set dirs=/I %VULKAN_SDK%\include /link /LIBPATH:..\vendor\lib\windows
set libs=user32.lib gdi32.lib Shcore.lib Wtsapi32.lib
set bullet3_libs=BulletCollision.lib BulletDynamics.lib BulletInverseDynamics.lib BulletInverseDynamicsUtils.lib BulletSoftBody.lib LinearMath.lib Bullet3Common.lib Bullet3Collision.lib Bullet3Dynamics.lib Bullet3Geometry.lib
set no_console=/SUBSYSTEM:windows /ENTRY:mainCRTStartup

rem cl ..\src\codegen.cpp %flags% %libs%
rem if not %ERRORLEVEL% EQU 0 (
rem 	echo compiling error: "codegen.cpp"
rem 	exit /b
rem )
rem codegen ..\src\common.cpp ..\src\math.cpp ..\src\vulkan.cpp ..\src\assets.cpp ..\src\menu.cpp ..\src\level.cpp ..\src\editor.cpp

set compile_editor=start /b cl ..\src\editor.cpp %flags% %dirs% %libs% %bullet3_libs%
set compile_game=start /b cl ..\src\game.cpp %flags% %dirs% %libs% %bullet3_libs%
set compile_import=start /b cl ..\src\import.cpp %flags% %dirs% %libs% nvtt.lib
set compile_lightmap=start /b cl ..\src\lightmap.cpp %flags% %dirs% %libs% UVAtlas.lib DirectXMesh.lib
set compile_test=start /b cl ..\src\test.cpp %flags% %dirs% %libs%

if "%~1"=="" (
	 %compile_editor%
	 %compile_game%
	 %compile_import%
	 %compile_lightmap%
	 %compile_test%
)
if "%~1"=="editor" %compile_editor%
if "%~1"=="game" %compile_game%
if "%~1"=="import" %compile_import%
if "%~1"=="lightmap" %compile_lightmap%
if "%~1"=="test" %compile_test%

rmdir /s /q shaders
mkdir shaders
pushd "..\src\shaders"
for %%G in (.vert, .frag) do forfiles /m *%%G /c "cmd /c %VULKAN_SDK%\Bin\glslc.exe -o ..\..\build\shaders\\@file.spv @file & if ERRORLEVEL 1 echo file name: @file"
popd

copy /y ..\vendor\lib\windows\nvtt.dll nvtt.dll >NUL

if not exist "assets" mkdir "assets"
pushd "assets"
xcopy /e /i /y ..\agby_assets\fonts fonts >NUL
popd

popd
