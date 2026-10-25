#include "cubesServer.hpp"
// #include "../Socket-Packet/mySerialization.hpp"
#include "../Socket-Packet/myPacket.hpp"
#include <chrono>
#include <system_error>
#include <iostream>

using namespace std::chrono;

Server::Server() : serverAddr(127,0,0,1,9000) {
	try {
		sock.open();
		sock.bind(serverAddr);
	} catch (std::system_error e) {
		std::cerr<<e.what()<<std::endl;
	}

	connect();
}

//Waits for both players, expects two PacketHellos, writes down who they came from, stores them in a std::unordered_map,
//sends back the helloPacket with each players id attached, telling them to start running. Then starts run{)-ning()
//The unordered_map starts indexing from 1.
void Server::connect() {
	Address clientAddr;

	//Handles the two players who MUST connect
	for(int i = 0; i < 2;) {
		PacketHello helloPacket;
		std::cout << "Waiting for player " << i + 1 << std::endl;
		try {
			sock.receive(&clientAddr, rStream.getBuffer());
			helloPacket.serializeData(rStream);
			if(helloPacket.getId() == uint32_t{0})
				clientAddrs.emplace(++i,clientAddr);
			else throw std::runtime_error("Server received a non 0 id on connect()\n");

		} catch(std::system_error e) {
			std::cerr<<e.what()<<std::endl;
		} catch(std::runtime_error e) {
			std::cerr<<e.what()<<std::endl;
		}

		std::cout << "Server received the " << i << ". hello packet with id = " << helloPacket.getId() <<
								 " and packet type = " << helloPacket.getType() << ", expected 4\n";
	}

	//Broadcasts the IDs back to all the connected players
	for(auto& client : clientAddrs) {

		PacketHello startRunningPacket(client.first);
		startRunningPacket.serializeData(wStream);
		std::cout << "Server sent out a start packet\n";
		std::cout << startRunningPacket.getType() << "\n";
		try {
			sock.send(&client.second, wStream.getBuffer());
		} catch(std::system_error e) {
			std::cerr << e.what() << std::endl;
		} catch(std::runtime_error e) {
			std::cerr << e.what() << std::endl;
		}

	}

	sock.setNonBlocking(true);
	std::cout << "Server running...\n";
}

//I need to have a loop here to utilize the entire tick time window for everything there is
//to read inside the buffer. Which means I will have to change the socket to return 
//the WOULD_BLOCK signal in case it's empty (Already done)
//
//I should actually separate this into read, simulate and send methods.
//I am thinking of a vector filled with Packet classes (If even possible)
//which the read function will simply fill out. 
//Simulate will read from there, update the world and clear that vector.
//Send will then use the p1 and p2 to construct a single snapshot packet and send it away
//
//The other way is to simply simulate the changes as packets are read. The first method
//feels slower, but I have no idea if it's even close to a significant slowdown.
//

//Returns false if there was nothing to read for the entire time the function is running. True otherwise
bool Server::readInputs() {
	bool filled = false;
	inputPackets.clear();

	while(true) { 

		try {
			sock.receive(nullptr, rStream.getBuffer());
		} catch(std::system_error e) {
			std::cerr<<e.what()<<std::endl;
		} catch (bool wouldBlock) { //If there is nothing else to read, break
			break;
		}

		filled = true;
		PacketInputs packet{};
		packet.serializeData(rStream);
		this->inputPackets.push_back(std::make_unique<PacketInputs>(std::move(packet))); //No idea if this works
	}
	return filled;
}

//Takes the player states, packs them into a snapshot packet and sends that to loopback again.
//Here I don't like how the data is serialized again for each player, but currently I have no idea
//how to neatly avoid that case.

void Server::sendSnapshot() {

	PacketSnapshot snapshot{game.getPositions()};

	for(auto& client : clientAddrs) {
		snapshot.serializeData(wStream);
		try {
			sock.send(&client.second, wStream.getBuffer());
		} catch(std::system_error e) {
			std::cerr << e.what() << std::endl;
		} catch(std::runtime_error e) {
			std::cerr << e.what() << std::endl;
		}
	}
}

//This code makes sure the actual working loop happen 60 times a second.
//This is implemtented by a simple time_point boundary that's offset each 
//16667 microseconds. The client side is more standard with a proper accumulator system

//There is also the technique to send the server to sleep until it's time 
//to do a "tick". That way I can save CPU resources.

//A potential idea for a potential problem is to make dt dynamic based on whether
//the server finished the whole work during one tick time interval. Of course thought 
//out in more detail

//Should I send snapshots only when the game state changes or literally always?
//For now it'll be always
void Server::run() {
    time_point<steady_clock> start = steady_clock::now();
		auto dt = microseconds(16667); // 60fps
		time_point<steady_clock> accumulator = start + dt;
		bool loop = true;

		while(loop) {
			time_point<steady_clock> currentTime = steady_clock::now();
			while(currentTime >= accumulator) {

				if (readInputs())
					game.simulate(this->inputPackets);

				sendSnapshot();
				accumulator += dt;
			}
		}
}

int main() {
	
	Server server{};
	server.run();
}
