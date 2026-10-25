#pragma once

#include <netdb.h>
#include <unistd.h>

#include "myStream.hpp"

#define MAXBUFFERSIZE 512

//The entire Address class needs to be thought out better and reworked. 
class Address {
	private:
		sockaddr_in address_;
		unsigned short port_;

	public:
		Address() = default; //Do this when I want to make an empty address to later be filled in by recvfrom
		Address(unsigned char a, 
						unsigned char b, 
						unsigned char c, 
						unsigned char d, 
						unsigned short port);

		Address(unsigned int address, unsigned short port);

		bool operator==(const Address& other) {
			if((address_.sin_addr.s_addr == other.address_.sin_addr.s_addr) &&
				(address_.sin_port == other.address_.sin_port))
				return true;
			return false;
		}

		bool operator!=(const Address& other) {return !(*this == other);}

		~Address() = default;

		int getA() const; 
		int getB() const;
		int getC() const; 
		int getD() const; 
		int getPort() const {return port_;}
		void setPort(unsigned short port) {port_ = port;}
		struct sockaddr_in* getAddress();
		void setAddress(sockaddr_in& sockAddress);
};

class Socket {
	private:
		int fd_ = 0;

	public:
		Socket() = default;

		//Sockets and other file descriptors are never copied
		Socket(const Socket&) = delete;
		Socket& operator=(const Socket&) = delete;

		Socket(Socket&& other) = default;
		Socket& operator=(Socket&&) = default;

		~Socket() {if (fd_ > 0) this->close();}

		bool isOpen() const; 

		void setNonBlocking(bool nonBlock);

		void open();
		void bind(Address& address);
		void close();

		//The idea is to use this as socket.send(address, stream.getBuffer(), size);
		//where stream is of type WriteStream
		
		void send(Address& destination_addr,
							Buffer& buffer);

		//The idea is to use this as socket.send(address, stream.getBuffer(), size);
		//where stream is of type ReadStream
		
		void receive(Address& source_addr,
								 Buffer& buffer);

};
