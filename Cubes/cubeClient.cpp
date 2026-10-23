#include "cubeClient.hpp"
#include <stdio.h>
#include <chrono>

using namespace std::chrono;

Client::Client(uint32_t id) : clientAddr(127,0,0,1,9000), playerId_(id) {
	try {
		sock.open();
		sock.setNonBlocking(true);
		sock.bind(clientAddr);
	} catch (std::system_error e) {
		std::cerr<<e.what()<<std::endl;
	}
}

void Client::sendInputs() {
	GLFWwindow* window = renderer.getWindow();
	int keys[] = {GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D};
	bool inputs[4];

	for(int i = 0; i < 4; i++) {
		inputs[i] = glfwGetKey(window, keys[i]) == GLFW_PRESS;
	}

	PacketInputs inputPacket{playerId_, inputs};
	inputPacket.serializeData(wStream);

	try {
		sock.send(clientAddr, wStream.getBuffer());
	} catch(std::system_error e) {
		std::cerr << e.what() << std::endl;
	} catch(std::runtime_error e) {
		std::cerr << e.what() << std::endl;
	}
}

bool Client::receiveSnapshot() {
	bool filled = false;

	while(true) {
		try {
			sock.receive(serverAddr, rStream.getBuffer());
		} catch(std::system_error e) {
			std::cerr<<e.what()<<std::endl;
		} catch (bool wouldBlock) { //If there is nothing else to read, break
			break;
		}
		filled = true;
		// PacketSnapshot snapshot{};
		// snapshot.serializeData(rStream); //Will these get destroyed and when
		// this->snapshot = std::move(snapshot); //????
		this->snapshot.serializeData(rStream);
	}
	return filled;
}

//Here I am unsure as to how to actually do the timing right for client work and 
//rendering work
void Client::run() {
    time_point<steady_clock> previous = steady_clock::now();
		auto dt = microseconds(16667); // 60fps
		microseconds accumulator(0);
		bool loop = true;

		while(loop) {
			auto current = steady_clock::now();
			accumulator += duration_cast<microseconds>(current - previous);
			previous = current;

			renderer.pollEvents();
			if(glfwGetKey(renderer.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) loop = false;

			while(accumulator >= dt) {

				if(receiveSnapshot()) {
					this->game.applySnapshot(this->snapshot);
				}
																		 
				sendInputs();
				accumulator -= dt;
			}

			renderer.render(this->game);
		}
}

int main(int argc, char** argv) {

	if(argc != 2) {
		printf("Usage: %s [number] where number = 1 or 2.", argv[0]);
		return 0;
	}

	if(*argv[1] != '1' && *argv[1] != '2') {
		printf("Invalid argument: Should be 1 or 2");
		return 0;
	}

	uint32_t num = *argv[1] - '0';

	Client client{num};
	client.run();

}
