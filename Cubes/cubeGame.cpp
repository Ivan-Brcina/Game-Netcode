#include "cubeGame.hpp"
#include <iostream>

void CubeGame::simulate(std::vector<std::unique_ptr<PacketInputs>>& inputPackets) {

	for(std::unique_ptr<PacketInputs>& packet : inputPackets) {
		std::vector<bool> inputs = packet->getInputs();
		switch((int)packet->getId()) {
			case 1:
				p1.updatePosition(inputs);
				break;
			case 2:
				p2.updatePosition(inputs);
				break;
			default:
				std::cout << "The server is reading an invalid player id during simulation\nIt reads"
									<< packet->getId() << ", expected either 1 or 2";
				break;
		}
	}
}

void CubeGame::applySnapshot(const PacketSnapshot& snapshot) {
	// std::cout << "The snapshot vector size is " << snapshot.getPositions().size() << ", expected 2\n"; //debug
	p1.setPosition(snapshot.getPositions().at(0));
	p2.setPosition(snapshot.getPositions().at(1));
}

std::vector<glm::vec2> CubeGame::getPositions() const {
	return std::vector<glm::vec2>{p1.getPosition(),p2.getPosition()};
}

std::vector<glm::vec3> CubeGame::getColors() const {
	return std::vector<glm::vec3>{p1.getColor(),p2.getColor()};
}

