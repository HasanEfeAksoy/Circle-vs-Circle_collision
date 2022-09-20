:: for unicode chars (save as ansi)
chcp 1252

g++ *.cpp -o main.exe -ID:\SFML-2.5.1\include -LD:\SFML-2.5.1\lib -lsfml-graphics-d -lsfml-system-d -lsfml-window-d
main.exe
