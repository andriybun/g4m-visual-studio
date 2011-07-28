echo off

set GCC_PATH="C:\ProgramFiles\MinGW\bin\g++"
set STARTUP_PROJECT_NAME=g4m
set OUT_BINARY_DIR=Bin_gcc
set SOLUTION_NAME=g4m

call %~dp0\_build_shared.cmd %GCC_PATH% commonClasses %OUT_BINARY_DIR% || exit /b 1
call %~dp0\_build_shared.cmd %GCC_PATH% jDataViewClasses %OUT_BINARY_DIR% || exit /b 1

call %~dp0\_build_executable.cmd %GCC_PATH% metadata_generator %OUT_BINARY_DIR% || exit /b 1 

pushd %~dp0\%STARTUP_PROJECT_NAME%

call %GCC_PATH% -c *.cpp -I ..\commonClasses -I ..\jDataViewClasses -O2 || popd exit /b 1
call %GCC_PATH% *.o ..\%OUT_BINARY_DIR%\*.lib -o "..\%OUT_BINARY_DIR%\%SOLUTION_NAME%" || exit /b 1

del *.o

popd

echo on

exit /b 0