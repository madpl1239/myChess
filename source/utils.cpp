/*
 * utils.cpp
 * 
 * 20-01-2025 by madpl
 */
#include "utils.hpp"


int initialCommand(Stockfish& engine)
{
	int result = 0;
	
	engine.sendCommand("uci");

	if(engine.getResponse().find("uciok") == std::string::npos)
	{
		engine.sendCommand("quit");
		std::cout << "engine: not uciok!\n";
			
		result = -1;
	}

	engine.sendCommand("uci");
	std::cout << engine.getResponse();

	engine.sendCommand("isready");
	if(engine.getResponse().find("readyok") != std::string::npos)
		std::cout << "readyok\n";
	else
	{
		engine.sendCommand("quit");
		std::cout << "not readyok!\n";
		
		result = -1;
	}

	engine.sendCommand("ucinewgame");
	engine.sendCommand("isready");
	
	std::string resp = engine.getResponse();
	if(resp.find("readyok") == std::string::npos)
	{
		std::cout << "response error\n";
		
		result = -1;
	}
	
	return result;
}


std::string getNextMove(Stockfish& engine, std::string& position)
{
	std::string command = "position startpos moves" + position;
	
	engine.sendCommand(command);
	engine.sendCommand("go depth 2");
	
	std::string response = engine.getResponse();
		
	size_t bestmoveIdx = response.find("bestmove");
	if(bestmoveIdx != std::string::npos)
	{
		size_t endIdx = response.find(' ', bestmoveIdx + 9);
		
		return (endIdx != std::string::npos) ? response.substr(bestmoveIdx + 9, endIdx - (bestmoveIdx + 9))
												: response.substr(bestmoveIdx + 9);
	}
	
	return "no response";
}
