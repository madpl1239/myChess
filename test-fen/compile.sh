# compile.sh for testFEN
# 12-02-2025 by madpl

g++ -o testFEN testFEN.cpp ../source/chessBoard.cpp ../source/moveLogger.cpp \
	-lgtest -lgtest_main -lpthread -I../source \
	-lsfml-system -lsfml-window -lsfml-graphics -std=c++23 -march=native
