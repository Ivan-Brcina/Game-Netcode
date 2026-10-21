#pragma once
#include "../Socket-Packet/myPacket.hpp"
#include "cubePlayer.hpp"
#include <memory>

//I had the additional idea of making a Game superclass that any project can inherit from 
//so that it always works with Server and Client in tandem.

class CubeGame {
	private:
		Player p1, p2;
	public:

		CubeGame() = default;
		~CubeGame() = default;

		void simulate(std::vector<std::unique_ptr<PacketInputs>>& inputPackets); //To be used by the server
		void update(PacketSnapshot& snapshot); //To be later used for client-side prediction

		//Should this be copied or can it be any other way?
		std::vector<glm::vec2> getPositions();
};
