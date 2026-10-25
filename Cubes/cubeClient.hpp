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

		bool receiveSnapshot();
		void sendInputs();

	public:
		Client();

		void run();
		void connect();
		~Client() = default;
};
