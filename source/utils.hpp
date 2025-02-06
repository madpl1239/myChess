/*
 * utils.hpp
 * 
 * 20-01-2025 by madpl
 */
#pragma once

#include <iostream>
#include "stockHandle.hpp"
#include "sndManager.hpp"


int initialCommand(Stockfish& engine);
bool initialSounds(SoundManager& sndManager);
std::string getNextMove(Stockfish& engine, std::string& position);
