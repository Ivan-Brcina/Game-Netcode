#include "cubeGame.hpp"

void CubeGame::simulate(std::vector<std::unique_ptr<PacketInputs>>& inputPackets) {
	for(std::unique_ptr<PacketInputs>& packet : inputPackets) {
		std::vector<bool> inputs = packet->getInputs();
		switch(packet->getId()) {
			case 0:
				p1.updatePosition(inputs);
			case 1:
				p2.updatePosition(inputs);
			default:
				std::cout << "The server is reading an invalid player id during simulation\n";
		}
	}
}

std::vector<glm::vec2> CubeGame::getPositions() {
	return std::vector<glm::vec2>{p1.getPosition(),p2.getPosition()};
}

