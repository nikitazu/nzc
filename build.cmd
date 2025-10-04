@echo off

rem Опции компилятора MSVC
rem предупреждения https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170

rem Опции компилятора CLANG
rem https://clang.llvm.org/docs/UsersManual.html
rem https://clang.llvm.org/docs/ClangCommandLineReference.html

rem TODO https://www.youtube.com/watch?v=9UIIMBqq1D4
rem      clang -fsanitize=address

chcp 65001
if not exist .\bin\ (
   mkdir .\bin
)

pushd .\bin
  call clang -std=c17 -Wall --debug ^
     -fdiagnostics-absolute-paths ^
     -fmacro-prefix-map=..\= ^
     -DUNICODE -D_UNICODE -DWIN32 -D_WINDOWS ^
     ..\test.c ^
     -o test.exe
popd
