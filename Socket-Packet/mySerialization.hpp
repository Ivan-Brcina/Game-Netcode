#pragma once
#include <glm/glm.hpp>

template<typename Stream> void serializeFloat(Stream& stream, float& value) {

	union FloatInt {
		float float_value;
		uint32_t int_value;
	};

	FloatInt temp;
	if(Stream::isWriting) {
		temp.float_value = value;
		// stream.serializeWord(temp.int_value);
		stream.serializeBytes(&temp.int_value, sizeof(uint32_t));
	}
	if(Stream::isReading) {
		// stream.serializeWord(temp.int_value);
		stream.serializeBytes(&temp.int_value, sizeof(uint32_t));
		value = temp.float_value;
	}
}

template<typename Stream> void serializeVec3(Stream& stream, glm::vec3& value) {
	serializeFloat(stream, value.x);
	serializeFloat(stream, value.y);
	serializeFloat(stream, value.z);
}

template<typename Stream> void serializeVec2(Stream& stream, glm::vec2& value) {
	serializeFloat(stream, value.x);
	serializeFloat(stream, value.y);
}

template<typename Stream> void serializeInt32(Stream& stream, uint32_t& value) {
	// stream.serializeWord(value);
	stream.serializeBytes(&value, sizeof(uint32_t));
}

template<typename Stream> void serializeBool(Stream& stream, bool& value) {
	stream.serializeBytes(&value, sizeof(uint8_t));
}
