#include <iostream>
#include <stdint.h>

struct testStruct
{
	uint8_t firstByte;   // Due to endian-ness, when you print this as a 32-bit, this is the ......XX bytes
	
	uint8_t secondByteA:1; // Bit 0 aka 0x01
	uint8_t secondByteB:3;
	uint8_t secondByteC:2;
	uint8_t secondByteD:1;
	uint8_t secondByteE:1; // Bit 7 aka 0x80
	
	uint16_t threeFourByteA:7; // This does not get 16-bit endian swapped
	uint16_t threeFourByteB:4;
	uint16_t threeFourByteC:5;
} __attribute__((packed));

int main(int argc, char** argv)
{
	struct testStruct x;

	x.firstByte = 0x16;
	
	x.secondByteA = 1;
	x.secondByteB = 7;
	x.secondByteC = 1;
	x.secondByteD = 1;
	x.secondByteE = 0;

	x.threeFourByteA = 0x41;
	x.threeFourByteB = 0x09;
	x.threeFourByteC = 0x15;

	std::cout << "x.firstByte = " <<   (int) x.firstByte << std::endl;
	std::cout << "x.secondByteA = " << (int) x.secondByteA << std::endl;
	std::cout << "x.secondByteB = " << (int) x.secondByteB << std::endl;
	std::cout << "x.secondByteC = " << (int) x.secondByteC << std::endl;
	std::cout << "x.secondByteD = " << (int) x.secondByteD << std::endl;
	std::cout << "x.secondByteE = " << (int) x.secondByteE << std::endl;

	std::cout << "x.threeFourByteA = " << x.threeFourByteA << std::endl;
	std::cout << "x.threeFourByteB = " << x.threeFourByteB << std::endl;
	std::cout << "x.threeFourByteC = " << x.threeFourByteC << std::endl;

	uint32_t* compareMe = (uint32_t*) &x;

	std::cout << "Compare Me = " << std::hex << *compareMe << std::endl;
	return 0;
}


