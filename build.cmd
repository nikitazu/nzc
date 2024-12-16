@echo off

rem Опции компилятора MSVC
rem предупреждения https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170

rem Опции компилятора CLANG
rem https://clang.llvm.org/docs/UsersManual.html
rem https://clang.llvm.org/docs/ClangCommandLineReference.html

chcp 65001
if not exist .\bin\ (
   mkdir .\bin
)

pushd .\bin
  call clang -std=c17 -Wall --debug ^
     -fdiagnostics-absolute-paths ^
     -fmacro-prefix-map=..\= ^
     -DUNICODE -D_UNICODE -DWIN32 -D_WINDOWS ^
     ..\test_win32.c ^
     -o test.exe
popd
