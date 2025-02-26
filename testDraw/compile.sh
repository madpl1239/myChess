# compile.sh for testSL
# 12-02-2025 by madpl

g++ -o testSL saveLoad.cpp ../source/chessBoard.cpp ../source/moveLogger.cpp \
-I../source -lsfml-system -lsfml-window -lsfml-graphics \
-std=c++23 -march=native
