@echo off
set /p buildname=What build name? 
mkdir builds\release\build-%date%-%buildname%
xcopy /e /i massshoot\massshoot builds\release\build-%date%-%buildname%\massshoot
copy x64\Release\massshoot.exe builds\release\build-%date%-%buildname%\
copy x64\Release\edwyplanner.exe builds\release\build-%date%-%buildname%\
echo Release build done. Press any key to exit.
pause > nul