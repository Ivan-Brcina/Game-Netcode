#include "cubePlayer.hpp"

//Horrible but I want to speed this up
//0 1 2 3 -> up left down right
void Player::updatePosition(std::vector<bool>& inputs) {
	if(inputs[0]) 
		position_ += glm::vec2(0.0f,1.0f);
	if(inputs[1])
		position_ += glm::vec2(-1.0f,0.0f);
	if(inputs[2])
		position_ += glm::vec2(0.0f,-1.0f);
	if(inputs[3])
		position_ += glm::vec2(1.0f,0.0f);
}
