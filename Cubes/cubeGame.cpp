#include "cubeGame.hpp"
#include <iostream>
#include <queue>

//Should possibly be able to apply the previously applied input in case the queue is empty at least once.
//Not sure why that's useful 
void CubeGame::step(std::vector<std::queue<PacketInputs>>& queues) {

	for(int i = 0; i < queues.size(); i++) {

		if(!queues.at(i).empty()) {

			PacketInputs& packet = queues.at(i).front();
			std::vector<bool> inputs = packet.getInputs();

			switch((int)packet.getId()) {
				case 1:
					p1.updatePosition(inputs);
					break;
				case 2:
					p2.updatePosition(inputs);
					break;
				default:
					std::cout << "Somehow an invalid playerId reached the step() function\n";
					break;
				}

				queues.at(i).pop();
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

