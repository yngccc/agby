@echo off
echo. 

call "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat" x64

mkdir "%~dp0/build" 2>nul
pushd "%~dp0/build"

printf "\ncompiling glsl..."
rmdir /s /q shaders 2>nul
mkdir shaders
pushd "../src/glsl"
for %%G in (.vert, .frag) do forfiles /m *%%G /c "cmd /c %VULKAN_SDK%/Bin/glslc.exe -o ../../build/shaders/@file.spv @file & if errorlevel 1 echo file name: @file"
popd

where /q ispc
if not errorlevel 0 (
  printf "Error: cannot find ispc.exe\n"
  exit 0
)
printf "\ncompiling ispc...\n"
del *.ispc.* 2>nul
ispc ../src/ispc/simple.ispc -o simple.ispc.obj -h simple.ispc.h --target=sse4-i32x4

rmdir /s /q compiler_output 2>nul
mkdir compiler_output

printf "\ncompiling c++...\n"
set flags=/nologo /Od /MD /EHsc /GS /sdl /FC /W3
if "%~2"=="clang" (
  set flags=%flags% -Wno-missing-braces -Wno-microsoft-include -mssse3
)
set dirs=/I %VULKAN_SDK%/include /link /LIBPATH:../vendor/lib/windows
set libs=user32.lib gdi32.lib Shcore.lib Wtsapi32.lib Comdlg32.lib
set bullet_libs=BulletCollision.lib BulletDynamics.lib BulletInverseDynamics.lib BulletSoftBody.lib LinearMath.lib
set no_console=/SUBSYSTEM:windows /ENTRY:mainCRTStartup

set compile_editor_cmd=start /b cmd /c "cl ../src/editor.cpp %flags% %dirs% %libs% %bullet_libs% > compiler_output/editor.txt"
set compile_game_cmd=start /b cmd /c "cl ../src/game.cpp %flags% %dirs% %libs% %bullet_libs% > compiler_output/game.txt"
set compile_import_cmd=start /b cmd /c "cl ../src/import.cpp %flags% %dirs% %libs% nvtt.lib ispc_texcomp.lib > compiler_output/import.txt"
set compile_test_cmd=start /b cmd /c "cl ../src/test.cpp %flags% %dirs% %libs% simple.ispc.obj> compiler_output/test.txt"

if "%~2"=="clang" (
  set compile_editor_cmd=start /b cmd /c "clang-cl ../src/editor.cpp -o editor.clang.exe %flags% %dirs% %libs% %bullet_libs% > compiler_output/editor.txt"
  set compile_game_cmd=start /b cmd /c "clang-cl ../src/game.cpp -o game.clang.exe %flags% %dirs% %libs% %bullet_libs% > compiler_output/game.txt"
  set compile_import_cmd=start /b cmd /c "clang-cl ../src/import.cpp -o import.clang.exe %flags% %dirs% %libs% nvtt.lib ispc_texcomp.lib > compiler_output/import.txt"
  set compile_test_cmd=start /b cmd /c "clang-cl ../src/test.cpp -o test.clang.exe %flags% %dirs% %libs% simple.ispc.obj> compiler_output/test.txt"
)

if "%~1"=="" set compile_all=true
if "%~1"=="all" set compile_all=true
if "%compile_all%"=="true" (
  set compile_editor=true
  set compile_game=true
  set compile_import=true
  set compile_test=true

	%compile_editor_cmd%
	%compile_game_cmd%
	%compile_import_cmd%
	%compile_test_cmd%
)
if "%~1"=="editor" (
  set compile_editor=true
	%compile_editor_cmd%
)
if "%~1"=="game" (
  set compile_game=true
	%compile_game_cmd%
)
if "%~1"=="import" (
  set compile_import=true
	%compile_import_cmd%
)
if "%~1"=="test" (
  set compile_test=true
	%compile_test_cmd%
)

if "%compile_editor%"=="true" (
  :wait_editor_txt
	if not exist compiler_output/editor.txt goto :wait_editor_txt
	2>nul (>>compiler_output/editor.txt echo off) || goto :wait_editor_txt
	type compiler_output\editor.txt
)
if "%compile_game%"=="true" (
  :wait_game_txt
	if not exist compiler_output/game.txt goto :wait_game_txt
	2>nul (>>compiler_output/game.txt echo off) || goto :wait_game_txt
	type compiler_output\game.txt
)
if "%compile_import%"=="true" (
  :wait_import_txt
	if not exist compiler_output/import.txt goto :wait_import_txt
	2>nul (>>compiler_output/import.txt echo off) || goto :wait_import_txt
	type compiler_output\import.txt
)
if "%compile_test%"=="true" (
  :wait_test_txt
	if not exist compiler_output/test.txt goto :wait_test_txt
	2>nul (>>compiler_output/test.txt echo off) || goto :wait_test_txt
	type compiler_output\test.txt
)

copy /y ..\vendor\lib\windows\nvtt.dll nvtt.dll >nul
copy /y ..\vendor\lib\windows\ispc_texcomp.dll ispc_texcomp.dll >nul

if not exist "assets" mkdir "assets"
pushd "assets"
if not exist "models" mkdir "models"
if not exist "skyboxes" mkdir "skyboxes"
if not exist "terrains" mkdir "terrains"
popd

popd
