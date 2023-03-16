@echo off

set THRD_DIR="Thread"
set SVC_DIR="Services"
set ENT_DIR="EntryPoint"
set MECH_DIR="Mechanism"
set BIN_DIR="Bin"
set ICLD_DIR="Include"
set APP_DIR="App"
set LIB_DIR="Lib"
set CCFLAGS="-g"
echo -----------Building in progress-----------

::building phase
echo Building WinThread
g++ -c %THRD_DIR%/WinThread.cxx -I %ICLD_DIR% -o %BIN_DIR%/WinThread.o %CCFLAGS%

echo Building InputService
g++ -c %SVC_DIR%/InputService.cxx -I %ICLD_DIR% -o %BIN_DIR%/InputService.o %CCFLAGS%

echo Building DisplayService
g++ -c %SVC_DIR%/DisplayService.cxx -I %ICLD_DIR% -o %BIN_DIR%/DisplayService.o %CCFLAGS%
g++ -c %SVC_DIR%/DisplayComponents/GraphicObject.cxx -I %ICLD_DIR% -o %BIN_DIR%/GraphicObject.o %CCFLAGS%
g++ -c %SVC_DIR%/DisplayComponents/Bitmap.cxx -I %ICLD_DIR% -o %BIN_DIR%/Bitmap.o %CCFLAGS%
g++ -c %SVC_DIR%/DisplayComponents/Textbox.cxx -I %ICLD_DIR% -o %BIN_DIR%/Textbox.o %CCFLAGS%

echo Building TweenService
g++ -c %SVC_DIR%/TweenService.cxx -I %ICLD_DIR% -o %BIN_DIR%/TweenService.o %CCFLAGS%

echo Building GameFlow
g++ -c %MECH_DIR%/GameFlow.cxx -I %ICLD_DIR% -o %BIN_DIR%/GameFlow.o %CCFLAGS%

echo Building main.cxx
g++ -c %ENT_DIR%/main.cxx -I %ICLD_DIR% -o %BIN_DIR%/main.o %CCFLAGS%

::linking phase
echo Linking bin files
g++ %BIN_DIR%/* -o %APP_DIR%/game.exe -L %LIB_DIR% -lgdi32

echo -----------Building finished-----------