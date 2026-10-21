#pragma once
#include "../Socket-Packet/mySocket.hpp"
#include "../Socket-Packet/myPacket.hpp"
#include "cubeGame.hpp"

#include <memory>

//Currently both serverAddr and clientAddr are loopback addresses on the same port.
//
//
//Might later think about how to make Server to be a reusable library utility with 
//the options to overload functions like simulate(), while read() and write() and so on
//remain the same or something.
class Server {
	private:
		Address serverAddr;
		Address clientAddr;
		Socket sock;

		WriteStream wStream;
		ReadStream rStream;
		std::vector<std::unique_ptr<PacketInputs>> inputPackets;

		CubeGame game; //Change this to change the game logic handling

		void readInputs();
		void sendSnapshot();

	public:
		Server();
		~Server() = default;

		void run();
};
