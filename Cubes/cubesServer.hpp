#pragma once
#include "../Socket-Packet/mySocket.hpp"
#include "../Socket-Packet/myPacket.hpp"
#include "cubeGame.hpp"

#include <memory>
#include <unordered_map>

//Currently both serverAddr and clientAddr are loopback addresses on the same port.
//
//
//Might later think about how to make Server to be a reusable library utility with 
//the options to overload functions like simulate(), while read() and write() and so on
//remain the same or something.

class Server {
	private:
		Address serverAddr;
		std::unordered_map<uint32_t, Address> clientAddrs;
		Socket sock;

		WriteStream wStream;
		ReadStream rStream;
		std::vector<std::unique_ptr<PacketInputs>> inputPackets;

		CubeGame game; //Change this to change the game logic handling

		void connect();
		bool readInputs();
		void sendSnapshot();

	public:
		Server(const char* serverIPv4);
		~Server() = default;

		void run();
};
