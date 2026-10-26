#pragma once
#include "../Socket-Packet/mySocket.hpp"
#include "cubeGame.hpp"
#include "../Rendering/cubeRenderer.hpp"

class Client {
	private:
		Address serverAddr;
		Address clientAddr;
		Socket sock;

		WriteStream wStream;
		ReadStream rStream;

		uint32_t playerId_;
		CubeGame game;
		CubeRenderer renderer;

		PacketSnapshot snapshot;

		void connect();
		bool receiveSnapshot();
		void sendInputs();
	public:
		Client(const char* clientIPv4, const char* serverIPv4);
		~Client() = default;

		void run();
};
