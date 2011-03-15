set GCC_PATH=%1
set PROJECT_NAME=%2
set BIN_PATH=..\%3

pushd %~dp0\%PROJECT_NAME%

if not exist %BIN_PATH% (
    md %BIN_PATH%
)

%GCC_PATH% -fPIC -c *.cpp || exit /b 1
%GCC_PATH% -shared -o %BIN_PATH%\%PROJECT_NAME%.lib *.o || exit /b 2

del *.o

popd


