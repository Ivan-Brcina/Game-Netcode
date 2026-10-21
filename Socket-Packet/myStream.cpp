#include "myStream.hpp"
#include <cstring>
#include <assert.h>

//Here I suspect that I shouldn't pass or use uint32_t by reference.
//Possible alternative is copying on write, and returning the value on read.
//Also this functionality will be where bitpacking happens in the future.

//Problem: Who manages the index value and when? Right now temporarily it's the socket send and read

void WriteStream::serializeWord(uint32_t& value) {
	assert((buffer_.index_ + 4) <= buffer_.size_);
	std::memcpy(buffer_.data_ + buffer_.index_, &value, sizeof(uint32_t));
	buffer_.index_ += 4;
}

void WriteStream::serializeBytes(void* value, int bytes) {
	assert((buffer_.index_ + bytes) <= buffer_.size_);
	std::memcpy(buffer_.data_ + buffer_.index_, value, bytes);
	buffer_.index_ += bytes;
}

void ReadStream::serializeWord(uint32_t& value) {
	assert((buffer_.index_ + 4) <= buffer_.size_);
	std::memcpy(&value, buffer_.data_ + buffer_.index_, sizeof(uint32_t));
	buffer_.index_ += 4;
}

void ReadStream::serializeBytes(void* value, int bytes) {
	assert((buffer_.index_ + bytes) <= buffer_.size_);
	std::memcpy(value, buffer_.data_ + buffer_.index_, bytes);
	buffer_.index_ += bytes;
}

Buffer& WriteStream::getBuffer() {return buffer_;}
Buffer& ReadStream::getBuffer() {return buffer_;}
