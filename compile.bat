@ECHO off
mkdir .\compile
del /q .\compile
for /R %%f in (*.h) do copy %%f .\compile
for /R %%f in (*.cpp) do copy %%f .\compile
for /R %%f in (*.ino) do copy %%f .\compile
call particle compile photon .\compile
call del /q .\compile
call rmdir /q .\compile


