g++ -c ./src/main.cpp -o ./build/main.o
g++ -c ./src/sm4.cpp -o ./build/sm4.o
g++ -c ./src/work_pattern.cpp -o ./build/work_pattern.o
g++ -c ./src/PKCSpadding.cpp -o ./build/PKCSpadding.o
g++ ./build/main.o ./build/sm4.o ./build/work_pattern.o ./build/PKCSpadding.o -o main
