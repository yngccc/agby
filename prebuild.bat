@echo off

echo starting prebuild...

mkdir "%~dp0/build"
pushd "%~dp0/build"

if "%~1"=="debug" (
    set build_type=debug
	mkdir debug
	pushd debug
	copy /y ..\..\vendor\lib\2017_x64_debug\*.dll
	copy /y ..\..\vendor\lib\2017_x64_debug\physx\*.dll
)
if "%~1"=="release" (
    set build_type=release
	mkdir release
	pushd release
	copy /y ..\..\vendor\lib\2017_x64_release\*.dll
	copy /y ..\..\vendor\lib\2017_x64_release\physx\*.dll
)

if "%~2"=="editor" (
	echo compiling hlsl...
	mkdir hlsl
	del /Q hlsl\*
	forfiles /p ..\..\src\hlsl /m *.vps.hlsl /c "cmd /c fxc.exe /nologo /Od /Zi /T vs_5_1 /E vertex_shader /Fo ..\\..\\build\\"%build_type%"\\hlsl\\@FNAME.vs.bytecode @PATH"
	forfiles /p ..\..\src\hlsl /m *.vps.hlsl /c "cmd /c fxc.exe /nologo /Od /Zi /T ps_5_1 /E pixel_shader /Fo ..\\..\\build\\"%build_type%"\\hlsl\\@FNAME.ps.bytecode @PATH"
	forfiles /p ..\..\src\hlsl /m *.rt.hlsl /c "cmd /c dxc.exe /nologo /Od /Zi /T lib_6_3 /Fo ..\\..\\build\\"%build_type%"\\hlsl\\@FNAME.bytecode @PATH"
	rem forfiles /p ..\..\src\hlsl /m *.cs.hlsl /c "cmd /c fxc.exe /nologo /Od /Zi /T cs_5_1 /E compute_shader /Fo ..\\..\\build\\"%build_type%"\\hlsl\\@FNAME.bytecode @PATH"

	echo compiling ispc...
	..\..\vendor\bin\ispc.exe ..\..\src\ispc\simple.ispc -o simple.ispc.obj -h ..\..\src\ispc\simple.ispc.h --target=sse4-i32x4
)

popd
popd

echo finished prebuild...
