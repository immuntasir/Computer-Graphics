g++ normal.cpp -o t1 -lGL -lGLU -lglut
g++ grayscale.cpp -o t2 -lGL -lGLU -lglut
g++ halftone.cpp -o t3 -lGL -lGLU -lglut

./t1 &
./t2 &
./t3 &
