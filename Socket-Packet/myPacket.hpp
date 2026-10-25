#pragma once
#include <glm/glm.hpp>
#include <stdexcept>
#include "mySerialization.hpp"

#include <iostream> //debugging

enum packetTypeEnum {
	POSITION_PACKET,
	STATUS_PACKET,
	INPUT_PACKET,
	SNAP_PACKET,
	HELLO_PACKET
};

// serialize_data in these cases means that these methods will either fill out the Packet class
// from the ReadStream or write from it to the WriteStream depending on what's passed down as the type Stream

//Each packet is using an isValid function which doesn't really make much sense. They should only check
//if the packet type is correct per the actual Packet class, not check all of them. The general check should
//be done by the user of Packets. Which means this should be changed greatly.

//Wondering if there is a need/way to abstract this and compact it.
//
//Not sure if the packets should be the ones to reset the stream buffer indices? 

class Packet {
	protected:
		uint32_t packetType_;
	public:
		Packet() = default;
		Packet(packetTypeEnum packetType) : packetType_(packetType) {}
		uint32_t getType() const {return packetType_;}
		virtual ~Packet() = default;

};

bool isValidPacket(uint32_t packetType);

//These were just tests

// class PacketPosition : public Packet {
// 	private:
// 		glm::vec3 position_;
// 		glm::vec3 front_;
// 	public:
// 		PacketPosition() : Packet(packetTypeEnum::POSITION_PACKET) {}
// 		PacketPosition(glm::vec3& position, glm::vec3& front);
//
// 		//Temporary for testing outputs
// 		friend std::ostream& operator<<(std::ostream& out, const PacketPosition& packet);
//
// 		//Is responsible for for either writing or reading from the Streams 
// 		template<typename Stream> void serializeData(Stream& stream) {
//
// 			serializeInt32(stream, this->packetType_);
//
// 			if(Stream::isReading && !isValidPacket(this->packetType_)) {
// 				throw std::invalid_argument("Not a correct type of packet");
// 			}
//
// 			serializeVec3(stream, this->position_);
// 			serializeVec3(stream, this->front_);
// 			stream.reset();
// 		}
// };
//
// class PacketStatus : public Packet{
// 	private:
// 		uint32_t health_;
// 		uint32_t mana_;
// 		uint32_t stamina_;
// 	public:
// 		PacketStatus() : Packet(packetTypeEnum::STATUS_PACKET) {}
// 		PacketStatus(uint32_t health, uint32_t mana, uint32_t bullets);
//
// 		template<typename Stream> void serializeData(Stream& stream) {
// 			serializeInt32(stream, this->packetType_);
//
// 			if(Stream::isReading && !isValidPacket(this->packetType_)) {
// 				throw std::invalid_argument("Not a correct type of packet");
// 			}
//
// 			serializeInt32(stream, this->health_);
// 			serializeInt32(stream, this->mana_);
// 			serializeInt32(stream, this->stamina_);
// 			stream.reset();
// 		}
// };


//This is going to be specialized to just handle four directional movement for the 
//sake of simplicity. Will definitely be changed later as more inputs get handled
//Used to send the input state of the given network frame from the client to the server.

class PacketInputs : public Packet {
	private:
		uint32_t id_;
		bool inputs_[4];
	public:
		PacketInputs() : Packet(packetTypeEnum::INPUT_PACKET) {}
		PacketInputs(PacketInputs&& other) = default;

		PacketInputs(uint32_t id, bool inputs[4]) : Packet(packetTypeEnum::INPUT_PACKET), id_(id) {
			for(int i = 0; i < 4; ++i) 
				inputs_[i] = inputs[i];
		}

		uint32_t getId() const {return id_;}
		std::vector<bool> getInputs() const {
			return std::vector<bool>{inputs_[0],inputs_[1],inputs_[2],inputs_[3]};
		}
		
		template<typename Stream> void serializeData(Stream& stream) {

			serializeInt32(stream, this->packetType_);

			if(Stream::isReading && (this->packetType_ != packetTypeEnum::INPUT_PACKET)) {
				std::cout << "Server read this type : " << this->packetType_ << ", expected 2\n";
				throw std::invalid_argument("Not a correct type of packet (Input)");
			}
			serializeInt32(stream, id_);
			for(int i = 0; i < 4; ++i) {
				serializeBool(stream, inputs_[i]); 
			}

			if(Stream::isReading)
				stream.getBuffer().reset();
		}
};

//Used to package the game state and broadcast it to all the clients.
class PacketSnapshot : public Packet {
	private:
		std::vector<glm::vec2> positions_;

	public:
		PacketSnapshot() : Packet(packetTypeEnum::SNAP_PACKET) {}
		PacketSnapshot(std::vector<glm::vec2> positions) : Packet(packetTypeEnum::SNAP_PACKET), positions_(positions) {}

		const std::vector<glm::vec2>& getPositions() const {return positions_;}
		
		template<typename Stream> void serializeData(Stream& stream) {

			serializeInt32(stream, this->packetType_);

			if(Stream::isReading && (this->packetType_ != packetTypeEnum::SNAP_PACKET)) {
				throw std::invalid_argument("Not a correct type of packet (Snapshot)");
			}

			// if(Stream::isWriting) { //Debugging //This part is correct
			// 	std::cout << "The size of the snapshot vector during send is: " << positions_.size() << ", expected 2\n";
			// } 
			
			if(Stream::isReading) {
				positions_.clear();
				glm::vec2 position;
				for(int i = 0; i < 2; i++) {
					serializeVec2(stream, position);
					positions_.push_back(position);
				}
				stream.getBuffer().reset();
			} else {
				for(auto& position : positions_) {
					serializeVec2(stream, position);
				}
			}

		}
};


//Unlike all the other packets. This one is designed to be sent empty by the client.
//Meant to be used at the moment the connection is established. The client 
//introduces itself to the server through this packet.
//Once the client received this packet from the server with its playerId assigned to it,
//it may start run()-ning. 
class PacketHello : public Packet {
	private:
		uint32_t id_;
	public: 

		PacketHello() : Packet(packetTypeEnum::HELLO_PACKET), id_(0) {}
		PacketHello(uint32_t id) : Packet(packetTypeEnum::HELLO_PACKET), id_(id) {}

		uint32_t getId() const {return id_;}

		template<typename Stream> void serializeData(Stream& stream) {

			serializeInt32(stream, this->packetType_);

			if(Stream::isReading && (this->packetType_ != packetTypeEnum::HELLO_PACKET)) {
				throw std::invalid_argument("Not a correct type of packet (Hello)");
			}
			
			serializeInt32(stream, id_);

			if(Stream::isReading)
				stream.getBuffer().reset();
		}
};


