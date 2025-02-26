# compile.sh for testSL
# 12-02-2025 by madpl

g++ -o testDraw draw.cpp ../source/chessBoard.cpp ../source/moveLogger.cpp \
../source/sndManager.hpp -I../source -lsfml-system -lsfml-window -lsfml-graphics \
-lsfml-audio -std=c++23 -march=native
