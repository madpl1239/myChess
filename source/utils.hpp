/*
 * utils.hpp
 * 
 * 20-01-2025 by madpl
 */
#pragma once

#include <iostream>
#include <sstream>
#include <regex>
#include "stockHandle.hpp"
#include "sndManager.hpp"


int initialCommand(Stockfish& engine);
bool initialSounds(SoundManager& sndManager);
std::string getNextMove(Stockfish& engine, std::string& position);
std::string getNextMoveAfterFEN(Stockfish& engine, std::string& fen, std::string& position);
float getEvaluation(std::string& resp, int& mateEvaluation);
