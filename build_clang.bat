@echo off
echo. 

mkdir "%~dp0/build" 2>nul
pushd "%~dp0/build"

set flags=-mssse3 -Wall -Wno-missing-braces -Wno-microsoft-include
set dirs=-I %VULKAN_SDK%/include -L ../vendor/lib/windows
set libs=-luser32.lib -lgdi32.lib -lShcore.lib -lWtsapi32.lib -lComdlg32.lib
set bullet_libs=-lBulletCollision.lib -lBulletDynamics.lib -lBulletInverseDynamics.lib -lBulletSoftBody.lib -lLinearMath.lib

clang ../src/editor.cpp -o editor.clang.exe %flags% %dirs% %libs% %bullet_libs%
clang ../src/test.cpp -o test.clang.exe %flags% %dirs% %libs%

popd