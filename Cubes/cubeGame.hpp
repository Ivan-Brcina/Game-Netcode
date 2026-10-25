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

		CubeGame() : p1(glm::vec2(-5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
								 p2(glm::vec2( 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)) {}
		~CubeGame() = default;

		void simulate(std::vector<std::unique_ptr<PacketInputs>>& inputPackets); //To be used by the server
		void applySnapshot(const PacketSnapshot& snapshot);

		//Should this be copied or can it be any other way?
		std::vector<glm::vec2> getPositions() const;
		std::vector<glm::vec3> getColors() const;
};
