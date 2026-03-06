@echo off

chcp 65001
call .\build.cmd

if %errorlevel%==0 (
  pushd .\wrk
  call ..\bin\word-count.exe .\word-count-sample.txt
  popd
)
