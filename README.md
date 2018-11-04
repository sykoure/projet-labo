# projet-labo
gcc -c ex_opencv.cpp -lm -Wall `pkg-config opencv --cflags`
gcc ex_opencv.o -lm -o ex_opencv `pkg-config opencv --libs`

./ex_opencv <image>