@echo off

set VSVARS=C:\Program Files\Microsoft Visual Studio 9.0\Common7\Tools\vsvars32.bat
set CYGWINBASE=C:\cygwin
set PATH=%CYGWINBASE%\bin;%PATH%
call "%VSVARS%"

C:
chdir C:\cygwin\bin

bash --login -i