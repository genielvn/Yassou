@echo off
gcc -o main.exe main.c error.c
timeout 2
cls
main.exe -d test.yass