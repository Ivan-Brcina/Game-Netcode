#include "cubeClient.hpp"
#include <stdio.h>
#include <chrono>
#include <iostream>

using namespace std::chrono;

//Setting the port to 0 will 
//choose a random free port while keeping
//the desired ip address.
Client::Client() : clientAddr(127,0,0,1,0), serverAddr(127,0,0,1,9000) { 
	try {
		sock.open();
		sock.bind(clientAddr);
	} catch (std::system_error e) {
		std::cerr<<e.what()<<std::endl;
	}

}

void Client::connect() {
	PacketHello helloPacket{};
	helloPacket.serializeData(wStream);

	try {
		sock.send(serverAddr, wStream.getBuffer());
	} catch(std::system_error e) {
		std::cerr << e.what() << std::endl;
	} catch(std::runtime_error e) {
		std::cerr << e.what() << std::endl;
	}

	std::cout << "Sent connection request\n";
	std::cout << "Waiting for the start message\n";

	Address senderAddr{};

	try {
		sock.receive(senderAddr, rStream.getBuffer());
		if(senderAddr != serverAddr)
			throw std::runtime_error("Client received a packet from the wrong address");
	} catch(std::system_error e) {
		std::cerr<<e.what()<<std::endl;
	} 

	helloPacket.serializeData(rStream);
	playerId_ = helloPacket.getId();
	sock.setNonBlocking(true);
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
		sock.send(serverAddr, wStream.getBuffer());
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

	Client client{};
	client.connect();
	client.run();

}
