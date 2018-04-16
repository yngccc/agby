@echo off
echo. 

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

where /q ctags
if ERRORLEVEL 0 (
pushd "%~dp0\src"
	start /b ctags -f .tags ^
	gpk.cpp codegen.cpp common.cpp editor.cpp game.cpp import.cpp level.cpp lightmap.cpp math.cpp platform_windows.cpp test.cpp vulkan.cpp ^
	..\vendor\include\imgui\imgui.h %VULKAN_SDK%\include\vulkan\vulkan_core.h ..\vendor\include\tinygltf\tiny_gltf.h
popd	
)

mkdir "%~dp0\build" 2>nul
pushd "%~dp0\build"

set flags=/nologo /Od /MD /EHsc /W3 /GS /sdl /FC /D_CRT_SECURE_NO_WARNINGS /D "_ITERATOR_DEBUG_LEVEL=0"
set dirs=/I %VULKAN_SDK%\include /link /LIBPATH:..\vendor\lib\windows
set libs=user32.lib gdi32.lib Shcore.lib Wtsapi32.lib Comdlg32.lib
set bullet3_libs=BulletCollision.lib BulletDynamics.lib BulletInverseDynamics.lib BulletInverseDynamicsUtils.lib BulletSoftBody.lib LinearMath.lib Bullet3Common.lib Bullet3Collision.lib Bullet3Dynamics.lib Bullet3Geometry.lib
set no_console=/SUBSYSTEM:windows /ENTRY:mainCRTStartup

rem cl ..\src\codegen.cpp %flags% %libs%
rem if not %ERRORLEVEL% EQU 0 (
rem 	echo compiling error: "codegen.cpp"
rem 	exit /b
rem )
rem codegen ..\src\common.cpp ..\src\math.cpp ..\src\vulkan.cpp ..\src\assets.cpp ..\src\menu.cpp ..\src\level.cpp ..\src\editor.cpp

rmdir /s /q compiler_output 2>nul
mkdir compiler_output

set compile_editor=start /b 9>"compiler_output\editor.lock" cmd /c "cl ..\src\editor.cpp %flags% %dirs% %libs% %bullet3_libs% > compiler_output\editor.txt"
set compile_game=start /b 9>"compiler_output\game.lock" cmd /c "cl ..\src\game.cpp %flags% %dirs% %libs% %bullet3_libs% > compiler_output\game.txt"
set compile_import=start /b 9>"compiler_output\import.lock" cmd /c "cl ..\src\import.cpp %flags% %dirs% %libs% nvtt.lib ispc_texcomp.lib > compiler_output\import.txt"
set compile_lightmap=start /b 9>"compiler_output\lightmap.lock" cmd /c "cl ..\src\lightmap.cpp %flags% %dirs% %libs% UVAtlas.lib DirectXMesh.lib > compiler_output\lightmap.txt"
set compile_test=start /b 9>"compiler_output\test.lock" cmd /c "cl ..\src\test.cpp %flags% %dirs% %libs% > compiler_output\test.txt"

printf "\ncompiling cpp files...\n"
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

1>nul 2>nul ping /n 2 ::1
for %%F in (compiler_output\*.lock) do (
	(call ) 9>"%%F" || goto :Wait
) 2>nul

type compiler_output\editor.txt 2>nul
type compiler_output\game.txt 2>nul
type compiler_output\import.txt 2>nul
type compiler_output\lightmap.txt 2>nul
type compiler_output\test.txt 2>nul

printf "\ncompiling shaders...\n"
rmdir /s /q shaders
mkdir shaders
pushd "..\src\shaders"
for %%G in (.vert, .frag) do forfiles /m *%%G /c "cmd /c %VULKAN_SDK%\Bin\glslc.exe -o ..\..\build\shaders\\@file.spv @file & if ERRORLEVEL 1 echo file name: @file"
popd

copy /y ..\vendor\lib\windows\nvtt.dll nvtt.dll >NUL
copy /y ..\vendor\lib\windows\ispc_texcomp.dll ispc_texcomp.dll >NUL

if not exist "assets" mkdir "assets"
pushd "assets"
if not exist "models" mkdir "models"
if not exist "skyboxes" mkdir "skyboxes"
if not exist "terrains" mkdir "terrains"
popd

popd
