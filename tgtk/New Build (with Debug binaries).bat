@echo off
set /p buildname=What build name? 
mkdir builds\debug\build-%date%-%buildname%
xcopy /e /i massshoot\massshoot builds\debug\build-%date%-%buildname%\massshoot
copy x64\Debug\massshoot.exe builds\debug\build-%date%-%buildname%\
copy x64\Debug\edwyplanner.exe builds\debug\build-%date%-%buildname%\
echo Debug build done. Press any key to exit.
pause > nul