@echo off

set THRD_FODR="Thread"
set SVC_FODR="Services"
set ENT_FODR="EntryPoint"
set BIN_FODR="Bin"
set ICLD_FODR="Include"
set APP_FODR="App"
set LIB_FODR="Lib"
set CCFLAGS="-g"
echo -----------Building in progress-----------

::building phase
echo Building WinThread.cxx
g++ -c %THRD_FODR%/WinThread.cxx -I %ICLD_FODR% -o %BIN_FODR%/WinThread.o %CCFLAGS%
echo Building InputService.cxx
g++ -c %SVC_FODR%/InputService.cxx -I %ICLD_FODR% -o %BIN_FODR%/InputService.o %CCFLAGS%
echo Building DisplayService.cxx
g++ -c %SVC_FODR%/DisplayService.cxx -I %ICLD_FODR% -o %BIN_FODR%/DisplayService.o %CCFLAGS%
g++ -c %SVC_FODR%/DisplayComponents/GraphicObject.cxx -I %ICLD_FODR% -o %BIN_FODR%/GraphicObject.o %CCFLAGS%
g++ -c %SVC_FODR%/DisplayComponents/Bitmap.cxx -I %ICLD_FODR% -o %BIN_FODR%/Bitmap.o %CCFLAGS%
g++ -c %SVC_FODR%/DisplayComponents/Textbox.cxx -I %ICLD_FODR% -o %BIN_FODR%/Textbox.o %CCFLAGS%
echo Building TweenService.cxx
g++ -c %SVC_FODR%/TweenService.cxx -I %ICLD_FODR% -o %BIN_FODR%/TweenService.o %CCFLAGS%
echo Building main.cxx
g++ -c %ENT_FODR%/main.cxx -I %ICLD_FODR% -o %BIN_FODR%/main.o %CCFLAGS%

::linking phase
echo Linking bin files
g++ %BIN_FODR%/* -o %APP_FODR%/game.exe -L %LIB_FODR% -lgdi32

echo -----------Building finished-----------