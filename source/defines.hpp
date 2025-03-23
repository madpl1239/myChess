/*
 * defines.hpp
 *
 * 02-12-2024 by madpl
 */ 
#pragma once

// for debugging
#define DEBUG 1
// #undef DEBUG

// for Linux, Windows Systems
#define LINUX
#undef WINDOWS

#define READ 0
#define WRITE 1

#define ARIAL "./resources/arial.ttf"
#define DIGITAL "./resources/digital-7mono.ttf"

const int SIZE = 504;		// size of chessBoard (504x504 pikseli)
const int TILE_SIZE = 56;	// chess square size
const int OFFSET = 28;		// offset for piece

const int TEXTLOGO_HEIGHT = 16;
const int TEXTMOVE_HEIGHT = 26;
const int TEXTCHECK_HEIGHT = 32;
const int TEXTON_RIGHT = 24;
