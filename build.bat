call "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat" x64

mkdir "%~dp0/build" 2>nul
pushd "%~dp0/build"

echo compiling hlsl...
rmdir /s /q hlsl 2>nul
mkdir hlsl
start /b forfiles /p ..\src\hlsl /m *.vps /c "cmd /c fxc.exe /nologo /Od /Zi /T vs_5_0 /E vertex_shader /Fo ..\\..\\build\\hlsl\\@FNAME.vs.fxc @PATH >nul" >nul
start /b forfiles /p ..\src\hlsl /m *.vps /c "cmd /c fxc.exe /nologo /Od /Zi /T ps_5_0 /E pixel_shader /Fo ..\\..\\build\\hlsl\\@FNAME.ps.fxc @PATH >nul" >nul
start /b forfiles /p ..\src\hlsl /m *.cs /c "cmd /c fxc.exe /nologo /Od /Zi /T cs_5_0 /E compute_shader /Fo ..\\..\\build\\hlsl\\@FNAME.cs.fxc @PATH >nul" >nul

echo compiling ispc...
del *.ispc.*
..\vendor\bin\ispc.exe ..\src\ispc\simple.ispc -o simple.ispc.obj -h ..\src\ispc\simple.ispc.h --target=sse4-i32x4 >nul

echo compiling flatbuffers...
del ..\src\flatbuffers\world_generated.h
..\vendor\bin\flatc.exe -c -o ..\src\flatbuffers ..\src\flatbuffers\world.fbs

copy /y ..\vendor\lib\vc15_x64\PhysX3_x64.dll PhysX3_x64.dll >nul
copy /y ..\vendor\lib\vc15_x64\PhysX3Common_x64.dll PhysX3Common_x64.dll >nul
copy /y ..\vendor\lib\vc15_x64\PhysX3CharacterKinematic_x64.dll PhysX3CharacterKinematic_x64.dll >nul
copy /y ..\vendor\lib\vc15_x64\PxFoundation_x64.dll PxFoundation_x64.dll >nul
copy /y ..\vendor\lib\vc15_x64\nvtt.dll nvtt.dll >nul
copy /y ..\vendor\lib\vc15_x64\ispc_texcomp.dll ispc_texcomp.dll >nul

if "%~1"=="prebuild_only" goto :end

rmdir /s /q compiler_output 2>nul
mkdir compiler_output

echo compiling cpp...
set cflags=/nologo /Od /MD /EHsc /GS /sdl /FC /W3 /WX
rem set no_console=/SUBSYSTEM:windows /ENTRY:mainCRTStartup
set dirs=/I ..\vendor\include /I ..\vendor\include\bullet /I ..\vendor\include\physx /link /LIBPATH:..\vendor\lib\vc15_x64
set windows_libs=user32.lib gdi32.lib Shcore.lib Wtsapi32.lib Comdlg32.lib d3d11.lib
set bullet_libs=BulletCollision.lib BulletDynamics.lib LinearMath.lib
set physx_libs=PhysX3_x64.lib PhysX3Common_x64.lib PhysX3Extensions_x64.lib PhysX3CharacterKinematic_x64.lib PhysX3Vehicle_x64.lib PxFoundation_x64.lib

set compile_editor_cmd=start /b cmd /c "cl ..\src\editor.cpp %cflags% %dirs% %no_console% %windows_libs% %bullet_libs% %physx_libs% > compiler_output\editor.txt"
set compile_game_cmd=start /b cmd /c "cl ..\src\game.cpp %cflags% %dirs% %no_console% %windows_libs% %bullet_libs% %physx_libs% > compiler_output\game.txt"
set compile_import_cmd=start /b cmd /c "cl ..\src\import.cpp %cflags% %dirs% %windows_libs% nvtt.lib ispc_texcomp.lib > compiler_output\import.txt"
set compile_ray_tracer_cmd=start /b cmd /c "cl ..\src\ray_tracer.cpp %cflags% %dirs% %windows_libs% > compiler_output\ray_tracer.txt"
set compile_test_cmd=start /b cmd /c "cl ..\src\test.cpp %cflags% %dirs% %windows_libs% simple.ispc.obj > compiler_output\test.txt"

rem set cflags=%cflags% -Wno-missing-braces -Wno-microsoft-include -Wno-unused-function -mssse3
rem set compile_editor_cmd=start /b cmd /c "clang-cl ..\src\editor.cpp -o editor.exe %cflags% %dirs% %no_console% %windows_libs% %bullet_libs% %physx_libs% > compiler_output\editor.txt"
rem set compile_game_cmd=start /b cmd /c "clang-cl ..\src\game.cpp -o game.exe %cflags% %dirs% %no_console% %windows_libs% %bullet_libs% %physx_libs% > compiler_output\game.txt"
rem set compile_import_cmd=start /b cmd /c "clang-cl ..\src\import.cpp -o import.exe %cflags% %dirs% %windows_libs% nvtt.lib ispc_texcomp.lib > compiler_output\import.txt"
rem set compile_ray_tracer_cmd=start /b cmd /c "clang-cl ..\src\ray_tracer.cpp -o ray_tracer.exe %cflags% %dirs% %windows_libs% > compiler_output\ray_tracer.txt"
rem set compile_test_cmd=start /b cmd /c "clang-cl ..\src\test.cpp -o test.exe %cflags% %dirs% %windows_libs% simple.ispc.obj> compiler_output\test.txt"

if "%~1"=="" set compile_all=true
if "%~1"=="all" set compile_all=true
if "%compile_all%"=="true" (
  set compile_editor=true
  set compile_game=true
  set compile_import=true
  set compile_ray_tracer=true
  set compile_test=true

	%compile_editor_cmd%
	%compile_game_cmd%
	%compile_import_cmd%
	%compile_ray_tracer_cmd%
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
if "%~1"=="ray_tracer" (
  set compile_ray_tracer=true
	%compile_ray_tracer_cmd%
)
if "%~1"=="test" (
  set compile_test=true
	%compile_test_cmd%
)

if "%compile_editor%"=="true" (
  :wait_editor_txt
	if not exist compiler_output\editor.txt goto :wait_editor_txt
	2>nul (>>compiler_output\editor.txt echo off) || goto :wait_editor_txt
	type compiler_output\editor.txt
)
if "%compile_game%"=="true" (
  :wait_game_txt
	if not exist compiler_output\game.txt goto :wait_game_txt
	2>nul (>>compiler_output\game.txt echo off) || goto :wait_game_txt
	type compiler_output\game.txt
)
if "%compile_import%"=="true" (
  :wait_import_txt
	if not exist compiler_output\import.txt goto :wait_import_txt
	2>nul (>>compiler_output\import.txt echo off) || goto :wait_import_txt
	type compiler_output\import.txt
)
if "%compile_ray_tracer%"=="true" (
  :wait_ray_tracer_txt
	if not exist compiler_output\ray_tracer.txt goto :wait_ray_tracer_txt
	2>nul (>>compiler_output\ray_tracer.txt echo off) || goto :wait_ray_tracer_txt
	type compiler_output\ray_tracer.txt
)
if "%compile_test%"=="true" (
  :wait_test_txt
	if not exist compiler_output\test.txt goto :wait_test_txt
	2>nul (>>compiler_output\test.txt echo off) || goto :wait_test_txt
	type compiler_output\test.txt
)

:end
popd
