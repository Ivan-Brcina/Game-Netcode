#include "myPacket.hpp"
#include "mySocket.hpp"
#include <system_error>
#include <errno.h>
#include <fcntl.h>

Address::Address(unsigned char a, 
				unsigned char b, 
				unsigned char c, 
				unsigned char d, 
				unsigned short port) : port_(port)
{
	int ipAddr = (a << 24) | (b  << 16) | (c << 8) | d;
	address_.sin_family = AF_INET;
	address_.sin_addr.s_addr = htonl(ipAddr);
	address_.sin_port = htons(port);
}

struct sockaddr_in* Address::getAddress() {
	return &address_;
}

void Address::setAddress(sockaddr_in& sockAddress) {
	address_ = sockAddress;
	port_ = sockAddress.sin_port;
}

void Socket::open() {
	fd_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd_ < 0)
		throw std::system_error();
}

void Socket::bind(Address& address) {
	int status; 
	status = ::bind(fd_,
							  (struct sockaddr*)address.getAddress(),
								 sizeof (sockaddr_in)
							 );

	if (status < 0) {
		throw std::system_error();
	}
}

bool Socket::isOpen() const {return fd_ > 0;}

void Socket::setNonBlocking(bool nonBlock) {
	int setValue = (nonBlock) ? 1 : 0;
	if (fcntl(fd_,F_SETFL, O_NONBLOCK, setValue) == -1) {
		throw std::system_error();
	}

}

void Socket::close() {::close(fd_);}

void Socket::send(Address& destination_addr,
									Buffer& buffer) 
{

	int sentBytes = sendto(this->fd_,
											 buffer.getData(),
											 buffer.getSize(),
											 0,
											 (struct sockaddr*)destination_addr.getAddress(),
											 sizeof(sockaddr_in)
											 );

	if(sentBytes < 0) {
		throw std::system_error();
	}

	if(sentBytes != buffer.getSize()) {
		throw std::runtime_error("sentBytes != buffer.getSize()");
	}

	buffer.reset();

}
//This is missing a receivedBytes == 0 for a graceful close.
void Socket::receive(Address& source_addr,
								 Buffer& buffer) {

	if(buffer.index_ > 0) {
		std::runtime_error("ReadBuffer should be reset but isn't");
	}

	sockaddr_in sourceAddr;
	socklen_t sourceSize = sizeof (sourceAddr);

	int receivedBytes = recvfrom(this->fd_,
															 buffer.getData(),
															 buffer.getMaxSize(),
															 0,
															 (struct sockaddr*)&sourceAddr,
															 &sourceSize
															 );

	if(receivedBytes < 0) {
		if(errno == EWOULDBLOCK) {
			buffer.reset();
			throw true;
		}
		else
			throw std::system_error();
	}

	buffer.reset();

}
