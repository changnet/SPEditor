@echo off

pyinstaller --onefile export.py
copy dist\export.exe .\
rd /s /q dist
rd /s /q build
del /f /s /q export.spec

pause