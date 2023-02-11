#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t addresslow, addresshigh, writeaddress, exitcode, errors;
int32_t bytedata, chksum, charactercount, datatype, nibbles, recordlength;

// :llaaaattdddd...ddcc
int main() {

	FILE *fp;
	fp = fopen("testdata.hex", "r")


		while (exitcode != 0)
		{
			//c = getc(stdin);
			c = fgetc(fp);
			if (c == ':')
			{
				if (chksum)
				{
					errors++;
					chksum = 0;
				};
				addresslow = 0;
				charactercount = 0;
				nibbles = 2;
			}

			if ((c >= '0') && (c <= '9'))
			{
				d = c - 0x30;
			}
			if ((c >= 'A') && (c <= 'F'))
			{
				d = c - 0x37;
			}
			if ((c >= 'a') && (c <= 'f'))
			{
				d = c - 0x57;
			}
			chksum = chksum + (d << ((nibbles >> 1) * 4))
				if ((charactercount >= 1) && (charactercount <= 2) {
					recordlength = (recordlength << 4) + d;
				}
			if ((charactercount >= 3) && (charactercount <= 6)) {
				addresslow = (addresslow << 4) + d;
			}
			if ((charactercount >= 7) && (charactercount <= 8))
			{
				datatype = (datatype << 4) + d;
					if (datatype == 1)
					{
						exitcode = -2;
					}
			}
			if (charactercount >= 9)
			{
				if ((datatype == 4) && (charactercount <= 12))
				{
					addresshigh = (addresshigh << 4) + d;
				}
				if ((datatype == 0) && (nibbles >= 1))
				{
					bytedata = (bytedata << 4) + d;
					nibbles--
				}
				if (datatype == 1)
				{
					exitcode++;
				}
			}
			if (nibbles == 0)
			{
				nibbles = 2;
				recordlength = recordlength - 2;
				if (recordlength >= 0)
				{
					writeaddress = addresslow + addresshigh;
					//writeonebytetosram(writeaddress, bytedata);
					printf("%0x ", bytedata);
				}
				addresslow++;

			}
		}
	fclose(fp);
}