/*
 * defines.hpp
 *
 * 02-12-2024 by madpl
 */ 
#pragma once

#include <SFML/Graphics.hpp>
#include "piece.hpp"


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
#define DEJAVUSANS "./resources/DejaVuSans.ttf"

using chessBoard_t = std::vector<std::vector<Piece>>;

const int SIZE = 504;		// size of chessBoard (504x504 pikseli)
const int SIZE2 = 450;
const int TILE_SIZE = 56;	// chess square size
const int OFFSET = 28;		// offset for piece

const int TEXTLOGO_HEIGHT = 16;
const int TEXTMOVE_HEIGHT = 26;
const int TEXT_HEIGHT1 = 32;
const int TEXT_HEIGHT2 = 26;
const int TEXTON_RIGHT = 24;

const sf::Color DeepPink{0xFF, 0x14, 0x93};
const sf::Color LimeGreen{0x32, 0xCD, 0x32};
const sf::Color MediumSpringGreen{0x00, 0xFA, 0x9A};
const sf::Color IndianRed{0xDD, 0x4C, 0x4C};
