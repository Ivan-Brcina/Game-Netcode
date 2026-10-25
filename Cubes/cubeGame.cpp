#include "cubeGame.hpp"
#include <iostream>

void CubeGame::simulate(std::vector<std::unique_ptr<PacketInputs>>& inputPackets) {

	for(std::unique_ptr<PacketInputs>& packet : inputPackets) {
		std::vector<bool> inputs = packet->getInputs();
		switch(packet->getId()) {
			case 1:
				p1.updatePosition(inputs);
			case 2:
				p2.updatePosition(inputs);
			default:
				std::cout << "The server is reading an invalid player id during simulation\n";
		}
	}
}

void CubeGame::applySnapshot(const PacketSnapshot& snapshot) {
	p1.setPosition(snapshot.getPositions().at(0));
	p2.setPosition(snapshot.getPositions().at(1));
}

std::vector<glm::vec2> CubeGame::getPositions() {
	return std::vector<glm::vec2>{p1.getPosition(),p2.getPosition()};
}

