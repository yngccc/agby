@echo off
echo. 

mkdir "%~dp0/build" 2>nul
pushd "%~dp0/build"

set flags=/nologo /Od /MD /EHsc /W3 -Wno-missing-braces -Wno-microsoft-include -mssse3
set dirs=/I %VULKAN_SDK%/include /link /LIBPATH:../vendor/lib/windows
set libs=user32.lib gdi32.lib Shcore.lib Wtsapi32.lib Comdlg32.lib
set bullet_libs=BulletCollision.lib BulletDynamics.lib BulletInverseDynamics.lib BulletSoftBody.lib LinearMath.lib

clang-cl ../src/editor.cpp -o editor.clang.exe %flags% %dirs% %libs% %bullet_libs%
clang-cl ../src/game.cpp -o game.clang.exe %flags% %dirs% %libs% %bullet_libs%
clang-cl ../src/import.cpp -o import.clang.exe %flags% %dirs% %libs% %bullet_libs% nvtt.lib ispc_texcomp.lib
clang-cl ../src/test.cpp -o test.clang.exe %flags% %dirs% %libs%

popd