echo off

set GCC_PATH="C:\ProgramFiles\MinGW\bin\g++"
set STARTUP_PROJECT_NAME=model
set OUT_BINARY_DIR=Bin_gcc
set SOLUTION_NAME=g4m

call %~dp0\_build_shared.cmd %GCC_PATH% commonClasses %OUT_BINARY_DIR% || exit /b 1
call %~dp0\_build_shared.cmd %GCC_PATH% jDataViewClasses %OUT_BINARY_DIR% || exit /b 1

rem %GCC_PATH% %STARTUP_PROJECT_NAME%\*.cpp %OUT_BINARY_DIR%\*.lib -I .\commonClasses -I .\jDataViewClasses -O %OUT_BINARY_DIR%\%SOLUTION_NAME% -O2

echo on

exit /b 0

