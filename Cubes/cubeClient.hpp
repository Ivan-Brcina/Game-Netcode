#pragma once
#include "../Socket-Packet/mySocket.hpp"
#include "cubeGame.hpp"
#include "cubeRenderer.hpp"

class Client {
	private:
		Address serverAddr;
		Address clientAddr;
		Socket sock;

		WriteStream wStream;
		ReadStream rStream;

		CubeGame game;
		CubeRenderer renderer;

		PacketSnapshot snapshot;

		bool receiveSnapshot();
		void sendInputs();

	public:
		Client();

		void run();
		~Client() = default;
};
