#include "cubePlayer.hpp"

//Horrible but I want to speed this up
//0 1 2 3 -> up left down right
//Currently fixed to move the unit amount per second since the updatePosition should happen at 60fps.
void Player::updatePosition(std::vector<bool>& inputs) {
	float deltaTime = 1.0f / 60.0f;
	float unit = 3.0f;
	float deltaS = unit * deltaTime;

	if(inputs[0]) 
		position_ += glm::vec2(0.0f,deltaS);
	if(inputs[1])
		position_ += glm::vec2(-deltaS,0.0f);
	if(inputs[2])
		position_ += glm::vec2(0.0f,-deltaS);
	if(inputs[3])
		position_ += glm::vec2(deltaS,0.0f);
}

void Player::setPosition(const glm::vec2& newPosition) {
	this->position_ = newPosition;
}
