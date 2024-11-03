@echo off
title File Structure Compare
powershell "%~dp0\fileStructCmp.exe %1 %2 v | tee outl.txt"
pause
notepad outl.txt
del outl.txt