# projet-labo

1. First compile
 ```
gcc -c ex_opencv.cpp -lstdc++ -lm -Wall `pkg-config opencv --cflags`
gcc ex_opencv.o -lstdc++ -lm -o ex_opencv `pkg-config opencv --libs`
 ```

2. Then run it
 ```
./ex_opencv (image)
 ```