@echo off

set THRD_FODR="Thread"
set SVC_FODR="Services"
set ENT_FODR="EntryPoint"
set BIN_FODR="Bin"
set ICLD_FODR="Include"
set APP_FODR="App"

echo -----------Building in progress-----------

::building phase
echo Building WinThread.cxx
g++ -c %THRD_FODR%/WinThread.cxx -I %ICLD_FODR% -o %BIN_FODR%/WinThread.o
echo Building InputService.cxx
g++ -c %SVC_FODR%/InputService.cxx -I %ICLD_FODR% -o %BIN_FODR%/InputService.o
echo Building main.cxx
g++ -c %ENT_FODR%/main.cxx -I %ICLD_FODR% -o %BIN_FODR%/main.o

::linking phase
echo Linking bin files
g++ %BIN_FODR%/* -o %APP_FODR%/game.exe

echo -----------Building finished-----------