#pragma once
#include <unistd.h>
#include <cstdint>

#define BUFFERSIZE 512

//Here the return values of serializeWord as many other functions so far is set to void.
//The reason for this is because I don't yet know what, if anything, they should return.
//Will most likely be used for error checking in some way.

struct Buffer {
		char data_[BUFFERSIZE];
		size_t size_ = BUFFERSIZE;
		int index_ = 0;
	
		void reset() { index_ = 0;}
		char* getData() {return data_;}
		size_t getSize() const {return index_;}
		size_t getMaxSize() const {return size_;}
};

//Stream classes could potentially become some kind of interface implementations;

class WriteStream {

	private:
		Buffer buffer_;
		// uint64_t scratch;
		// uint8_t scratchBits; //To be used later for bitpacking

	public:
		enum {isWriting = 1};
		enum {isReading = 0};

		Buffer& getBuffer();
		void serializeWord(uint32_t& value);
		void serializeBytes(void* value, int bytes);
		
};

class ReadStream {

	private:
		Buffer buffer_;
	public:
		enum {isWriting = 0};
		enum {isReading = 1};

		Buffer& getBuffer();
		void serializeWord(uint32_t& value);
		void serializeBytes(void* value, int bytes);
};

