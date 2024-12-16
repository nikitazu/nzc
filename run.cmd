@echo off

chcp 65001
call .\build.cmd

if %errorlevel%==0 (
  pushd .\bin
  call .\test.exe
  popd
)
