#include "myPacket.hpp"
#include "mySerialization.hpp"
#include <iostream>

// PacketPosition::PacketPosition(glm::vec3& pos, glm::vec3& front) : Packet(packetTypeEnum::POSITION_PACKET),
// 	position_(pos), front_(front) {}
//
// bool isValidPacket(uint32_t packetType) {
// 	switch(packetType) {
// 		case packetTypeEnum::POSITION_PACKET:
// 		case packetTypeEnum::STATUS_PACKET:
// 		case packetTypeEnum::INPUT_PACKET:
// 		case packetTypeEnum::SNAP_PACKET:
// 			return true;
// 		default:
// 			return false;
//
// 	}
// }
//
// std::ostream& operator<<(std::ostream& out, const PacketPosition& packet) {
// 	out << "Packet type -> " << packet.packetType_ << "\n" <<
// 				 "Position: \n" << 
// 				 " x: " << packet.position_.x						<< "\n" << 
// 				 " y: " << packet.position_.y						<< "\n" <<		 
// 				 " z: " << packet.position_.z						<< "\n" << 
// 				 "Front: \n" << 
// 				 " x: " << packet.front_.x							<< "\n" << 
// 				 " y: " << packet.front_.y							<< "\n" <<		 
// 				 " z: " << packet.front_.z							<< std::endl;
//
// 	return out;
// }
