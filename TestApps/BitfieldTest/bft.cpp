#include <stdio.h>
#include <stdint.h>

/**
 * This is an example of a union that has an 8-bit form you use to move all 8 flag bits at once
 * or bitfields to manipulate single bits 1 at a time
 */
union SecondByteUnion
{
		uint8_t secondByteWhole;
		struct
		{
			uint8_t secondByteA:1; // Bit 0 aka 0x01
			uint8_t secondByteB:3;
			uint8_t secondByteC:2;
			uint8_t secondByteD:1;
			uint8_t secondByteE:1; // Bit 7 aka 0x80
		};
} __attribute__((packed));

struct testStruct
{
	uint8_t firstByte;   // Due to endian-ness, when you print this as a 32-bit, this is the ......XX bytes

	union SecondByteUnion secondByte;

	uint16_t threeFourByteA:7; // This does not get 16-bit endian swapped
	uint16_t threeFourByteB:4;
	uint16_t threeFourByteC:5;
} __attribute__((packed));

int main(int argc, char** argv)
{
	struct testStruct x;

	x.firstByte = 0x16;
	
	x.secondByte.secondByteA = 1;
	x.secondByte.secondByteB = 7;
	x.secondByte.secondByteC = 1;
	x.secondByte.secondByteD = 1;
	x.secondByte.secondByteE = 0;

	x.threeFourByteA = 0x41;
	x.threeFourByteB = 0x09;
	x.threeFourByteC = 0x15;

	printf("x.firstByte = 0x%x\n", x.firstByte);

	printf("x.secondByteA = 0x%x\n", x.secondByte.secondByteA);
	printf("x.secondByteB = 0x%x\n", x.secondByte.secondByteB);
	printf("x.secondByteC = 0x%x\n", x.secondByte.secondByteC);
	printf("x.secondByteD = 0x%x\n", x.secondByte.secondByteD);
	printf("x.secondByteE = 0x%x\n", x.secondByte.secondByteE);
	printf("x.secondByteWhole = 0x%x\n", x.secondByte.secondByteWhole);

	printf("x.threeFourByteA = 0x%x\n", x.threeFourByteA);
	printf("x.threeFourByteB = 0x%x\n", x.threeFourByteB);
	printf("x.threeFourByteC = 0x%x\n", x.threeFourByteC);

	uint32_t* compareMe = (uint32_t*) &x;

	printf("Compare Me = 0x%x\n", *compareMe);
	return 0;
}


