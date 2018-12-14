call "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat" x64

mkdir "%~dp0/build" 2>nul
pushd "%~dp0/build"

echo compiling hlsl...
mkdir hlsl 2>nul
start /b forfiles /p ..\src\hlsl /m *.vps /c "cmd /c fxc.exe /nologo /Od /Zi /T vs_5_0 /E vertex_shader /Fo ..\\..\\build\\hlsl\\@FNAME.vs.fxc @PATH >nul" >nul
start /b forfiles /p ..\src\hlsl /m *.vps /c "cmd /c fxc.exe /nologo /Od /Zi /T ps_5_0 /E pixel_shader /Fo ..\\..\\build\\hlsl\\@FNAME.ps.fxc @PATH >nul" >nul
start /b forfiles /p ..\src\hlsl /m *.cs /c "cmd /c fxc.exe /nologo /Od /Zi /T cs_5_0 /E compute_shader /Fo ..\\..\\build\\hlsl\\@FNAME.cs.fxc @PATH >nul" >nul

echo compiling ispc...
..\vendor\bin\ispc.exe ..\src\ispc\simple.ispc -o simple.ispc.obj -h ..\src\ispc\simple.ispc.h --target=sse4-i32x4 >nul

echo compiling flatbuffers...
..\vendor\bin\flatc.exe -c -o ..\src\flatbuffers ..\src\flatbuffers\world.fbs

copy /y ..\vendor\lib\vc15_x64\*.dll >nul

if "%~1"=="prebuild_only" goto :end

echo compiling cpp...
rem set cc=cl
set cc=clang-cl

set cc_options=/Od
rem set cc_options=%cc_options% /O2

set cc_options=%cc_options% /nologo /W3 /WX /MD /EHsc /GS /sdl /FC
rem set cc_options=%cc_options% /SUBSYSTEM:windows /ENTRY:mainCRTStartup

if "%cc%"=="clang-cl" set cc_options=%cc_options% -fdiagnostics-absolute-paths -mssse3 -Wno-missing-braces -Wno-microsoft-include -Wno-unused-function -Wno-unknown-pragmas -Wno-return-type-c-linkage

set cc_options=%cc_options% /I ..\vendor\include /I ..\vendor\include\bullet /I ..\vendor\include\physx

set cc_options=%cc_options% user32.lib gdi32.lib Shcore.lib Wtsapi32.lib Comdlg32.lib d3d11.lib
set cc_options=%cc_options% PhysX3_x64.lib PhysX3Common_x64.lib PhysX3Extensions_x64.lib PhysX3CharacterKinematic_x64.lib PhysX3Vehicle_x64.lib PxFoundation_x64.lib
set cc_options=%cc_options% BulletCollision.lib BulletDynamics.lib LinearMath.lib
set cc_options=%cc_options% nvtt.lib ispc_texcomp.lib simple.ispc.obj
set cc_options=%cc_options% /link /LIBPATH:..\vendor\lib\vc15_x64

set compile_editor_cmd=start /b cmd /c "%cc% ..\src\editor.cpp /Feeditor.exe %cc_options% > compiler_output\editor.txt"
set compile_game_cmd=start /b cmd /c "%cc% ..\src\game.cpp /Fegame.exe %cc_options% > compiler_output\game.txt"
set compile_import_cmd=start /b cmd /c "%cc% ..\src\import.cpp /Feimport.exe %cc_options% > compiler_output\import.txt"
set compile_ray_tracer_cmd=start /b cmd /c "%cc% ..\src\ray_tracer.cpp /Feray_tracer.exe %cc_options% > compiler_output\ray_tracer.txt"
set compile_test_cmd=start /b cmd /c "%cc% ..\src\test.cpp /Fetest.exe %cc_options% > compiler_output\test.txt"

mkdir compiler_output 2>nul
del /Q compiler_output\*

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
