#pragma once
#include <glm/glm.hpp>
#include <vector>

//Currently there is no plan to use the color_ field. It's there to simply fill up space and simulate
//a player being something more complex than a position. Might change later

class Player {
	private:
		glm::vec2 position_;
		glm::vec3 color_;
	public:
		Player() = default;
		Player(const glm::vec2& pos, const glm::vec3& color) : position_(pos), color_(color) {}
		~Player() = default;

		glm::vec2 getPosition() const {return position_;}
		void setPosition(const glm::vec2& newPosition);
		glm::vec3 getColor() const {return color_;}
		void updatePosition(std::vector<bool>& inputs);
};
