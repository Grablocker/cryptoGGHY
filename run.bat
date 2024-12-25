g++ -c ./main.cpp -o ./build/main.o
g++ -c ./sm4.cpp -o ./build/sm4.o
g++ ./build/main.o ./build/sm4.o -o main
