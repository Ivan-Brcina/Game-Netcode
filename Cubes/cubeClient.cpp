#include "cubeClient.hpp"
#include <chrono>
#include <iostream>

using namespace std::chrono;

//Setting the port to 0 will 
//choose a random free port while keeping
//the desired ip address.
Client::Client(const char* clientIPv4, const char* serverIPv4) : clientAddr(clientIPv4, 0), serverAddr(serverIPv4,9000) { 
	try {
		sock.open();
		sock.bind(clientAddr);
	} catch (std::system_error e) {
		std::cerr<<e.what()<<std::endl;
	}

	connect();
}

void Client::connect() {
	PacketHello helloPacket;
	helloPacket.serializeData(wStream);

	try {
		sock.send(&serverAddr, wStream.getBuffer());
	} catch(std::system_error e) {
		std::cerr << e.what() << std::endl;
	} catch(std::runtime_error e) {
		std::cerr << e.what() << std::endl;
	}

	std::cout << "Sent connection request\n";
	std::cout << "Waiting for the start message\n";

	Address senderAddr{};

	try {
		sock.receive(&senderAddr, rStream.getBuffer());
		if(senderAddr != serverAddr)
			throw std::runtime_error("Client received a packet from the wrong address");
	} catch(std::system_error e) {
		std::cerr<<e.what()<<std::endl;
	} 

	helloPacket.serializeData(rStream);
	playerId_ = helloPacket.getId();
	sock.setNonBlocking(true);
	std::cout << "Connection succesful, beginning run()" << std::endl;
}

void Client::sendInputs() {
	GLFWwindow* window = renderer.getWindow();
	int keys[] = {GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D};
	bool inputs[4];

	for(int i = 0; i < 4; i++) {
		inputs[i] = glfwGetKey(window, keys[i]) == GLFW_PRESS;
	}

	PacketInputs inputPacket{this->playerId_, inputs};
	inputPacket.serializeData(wStream);

	try {
		sock.send(&serverAddr, wStream.getBuffer());
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
			sock.receive(nullptr, rStream.getBuffer());
		} catch(std::system_error e) {
			std::cerr<<e.what()<<std::endl;
		} catch (bool wouldBlock) { //If there is nothing else to read, break
			break;
		}
		filled = true;
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

		std::cout << "Reached the beginning of the main loop\n";
		while(loop) {
			auto current = steady_clock::now();
			accumulator += duration_cast<microseconds>(current - previous);
			previous = current;

			renderer.pollEvents();
			if(glfwGetKey(renderer.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) loop = false;

			while(accumulator >= dt) {

				// std::cout << "Reached the inner network while loop\n";
				if(receiveSnapshot()) {
					// std::cout << "Received a snapshot\n";
					this->game.applySnapshot(this->snapshot); //Bug is here
					// std::cout << "Applied said snapshot\n";
				}
																		 
				// std::cout << "before sendInputs()\n";
				sendInputs();
				// std::cout << "Sent a set of inputs\n";
				accumulator -= dt;
			}

			renderer.render(this->game);
		}
}

int main(int argc, char** argv) {

	if(argc != 3) {
		std::cout << "Usage: cubeClient (clientIp serverIp): 192.168.1.10 192.168.1.20\n";
	}
	std::string clientIp{argv[1]};
	std::string serverIp{argv[2]};
	Client client{clientIp.c_str(), serverIp.c_str()};
	client.run();

}
