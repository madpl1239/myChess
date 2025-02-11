g++ -std=c++23 -march=native -o testFEN testFEN.cpp ../source/chessBoard.cpp ../source/moveLogger.cpp \
-lgtest -lgtest_main -lpthread -I../source -lsfml-system -lsfml-window \
-lsfml-graphics -lsfml-audio
