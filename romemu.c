#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// AD[0:7]=GPIO[0:7], A[8:15]A[16:23]=GPIO[8:15]
// GPIO23:(output) onboard SPS mode select 0(default),
// GPIO24:(input) USB power voltage detect(inpput),
// GPIO25:LED

#define RAMCAP 131072
#define GPIOOUTPUTS 0x6ffffff

#define ASTB 16
#define HOS_N_TGT 17
#define CS_N 18
#define RD_N 19
#define WR_N 20
#define RESET_N 21
#define BUFEN_N 22

#define LED 25
void dumpallasc(int mode);
void fillramasc(void);
void dumpallhex(void);
void fillramhex(void);
void fillentireramhex(int filldata);
void writeonebytetosram(uint32_t address, uint32_t data);
int32_t readonebytefromsram(uint32_t address);
void outputaddress(uint32_t address);

int main()
{
    int c;
    stdio_init_all();
    gpio_init_mask(GPIOOUTPUTS);
    gpio_set_dir_out_masked(GPIOOUTPUTS); // set gpio[0:26] as output;
    gpio_put(BUFEN_N, 1);
    gpio_put(RESET_N, 0);   // reset target at startup
    gpio_put(HOS_N_TGT, 0); // Host handles RAM signal lines
    gpio_put(CS_N, 1);
    gpio_put(RD_N, 1);
    gpio_put(WR_N, 1);
    gpio_put(ASTB, 0);
    sleep_ms(1000);
    printf("Start\n");
    printf("Target reset active:\n");

    while (1)
    {

        // commands (temp. case sensitive)
        // W	Wait ascii hexadecimal data from console and write to the emulator RAM. Starts from address 0
        // R	Read and output entire emurator RAM contents in ascii hexadecimal to console.
        // w	Intel HEX version of command W
        // R	Intel HEX version of command R
        // F    Fill entire ram with 0xff
        // E	Read and output entire emurator RAM contents in ascii hexadecimal to console with ram address.
        // X	Run target
        // S	Stop (keep RESET# low) target
        // s	Reset target
        gpio_put(LED, 1);
        printf(">");
        c = getc(stdin);
        gpio_put(LED, 0);
        printf("%c", c);
        // putc('\n', stdout);
        if (c == 'R')
        {
            gpio_put(RESET_N, 0); // reset target at startup
            gpio_put(HOS_N_TGT, 0);
            busy_wait_us(30);
            dumpallasc(0);
        }
        else if (c == 'W')
        {
            gpio_put(RESET_N, 0); // reset target at startup
            gpio_put(HOS_N_TGT, 0);
            busy_wait_us(30);
            fillramasc();
        }
        else if (c == 'r')
        {
            gpio_put(RESET_N, 0); // reset target at startup
            gpio_put(HOS_N_TGT, 0);
            busy_wait_us(30);
            dumpallhex();
        }
        else if (c == 'w')
        {
            gpio_put(RESET_N, 0); // reset target at startup
            gpio_put(HOS_N_TGT, 0);
            busy_wait_us(30);
            fillramhex();
        }
        else if (c == 'F')
        {
            gpio_put(RESET_N, 0); // reset target at startup
            gpio_put(HOS_N_TGT, 0);
            busy_wait_us(30);
            fillentireramhex(0xff);
        }
        else if (c == 'E')
        {
            gpio_put(RESET_N, 0); // reset target at startup
            gpio_put(HOS_N_TGT, 0);
            busy_wait_us(30);
            dumpallasc(1);
        }
        else if (c == 'X')
        {
            gpio_put(HOS_N_TGT, 1);
            gpio_put(RESET_N, 1);

            printf("Target reset inactive\n");
        }
        else if (c == 'S')
        {
            gpio_put(RESET_N, 0);

            printf("Target reset active\n");
            gpio_put(HOS_N_TGT, 0);
        }
        else if (c == 's')
        {
            gpio_put(RESET_N, 0);
            busy_wait_us(30000);
            gpio_put(HOS_N_TGT, 1);
            gpio_put(RESET_N, 1);

            printf("Target restart\n");
        }
        else
        {
            putc('\n', stdout);
        }
    }
}

void dumpallasc(int mode)
{
    uint32_t readaddress = 0;
    int32_t r, c, t;
    do
    {
        if (((readaddress & 0x0f) == 0) && (mode == 1))
        {
            for (int i = 5; i >= 0; i--)
            {
                c = (readaddress >> (i*4) ) & 0x0f;
                if (c < 10)
                {
                    c = c + 0x30;
                }
                else if (c >= 10)
                {
                    c = c + 0x37;
                }
                putc(c, stdout);
                
            }

            putc(':', stdout);
            putc(' ', stdout);
        }
        r = readonebytefromsram(readaddress);
        c = (r >> 4) & 0x0f;
        if (c < 10)
        {
            c = c + 0x30;
        }
        else if (c >= 10)
        {
            c = c + 0x37;
        }
        putc(c, stdout);
        c = r & 0x0f;
        if (c < 10)
        {
            c = c + 0x30;
        }
        else if (c >= 10)
        {
            c = c + 0x37;
        }
        putc(c, stdout);
        putc(0x20, stdout);
        readaddress++;
        if ((readaddress & 0x0f) == 0)
            putc('\n', stdout);
    } while (readaddress < RAMCAP);
}

void fillramasc(void)
{
    uint32_t writeaddress = 0;
    int32_t w, c, t, endcond, endfile, nibble;
    nibble = 1;
    endfile = 0;
    endcond = 0;
    printf("Write data from address 0.\n");
    printf("Pour ascii file, or manually input hexadecimal numbers:\n");
    printf("CR, LF, and tab are ignored\n");
    printf("Ctrl-C, Ctrl-Z, or more than five blank characters terminates\n");
    printf("process and fill remaining space with 0FFH\n");
    do
    {

        c = getc(stdin);
        if (c == ' ')
        {

            endfile++;
        }
        if ((c == '\x03') || (c == '\x1a'))
        {
            endcond = -1;
        }
        if ((c >= '0') && (c <= '9'))
        {
            c = c - '0';
            endfile = 0;
        }
        else if ((c >= 'A') && (c <= 'F'))
        {
            c = c - 0x37;
            endfile = 0;
        }
        else if ((c >= 'a') && (c <= 'f'))
        {
            c = c - 0x57;
            endfile = 0;
        }
        if (nibble == 1)
        {
            w = c << 4;
        }
        else if (nibble == 0)
        {
            w = w + c;
            ;
        }
        if (nibble == 0)
        {
            writeonebytetosram(writeaddress, w);
            nibble = 1;
            writeaddress++;
        }
        if (endfile > 3)
        {
            while (writeaddress < RAMCAP)
            {
                writeonebytetosram(writeaddress, 0xff);
                writeaddress++;
            }
        } // fill remaining ram space with 0xff(ROM BLANK)
        if (endcond < 0)
        {
            writeaddress = RAMCAP;
        }
    } while (writeaddress < RAMCAP);
}

void dumpallhex(void)
{
    printf("Not available\n");
    return;
}

void fillramhex(void)
{

    uint32_t addresslow, addresshigh, writeaddress, exitcode, errors;
    int32_t bytedata, chksum, charactercount, datatype, nibbles, recordlength;
    int32_t c, d, d8;

    // :llaaaattdddd...ddcc

    exitcode = 1;
    printf("Send Hex File:\n");
    while (exitcode != 0)
    {
        c = getc(stdin);

        if (c == ':')
        {
            chksum = chksum & 0xff;
            if (chksum)
            {
                errors++;
            }
            bytedata = 0;
            recordlength = 0;
            datatype = 0;
            chksum = 0;
            addresslow = 0;
            charactercount = 0;
            nibbles = 2;
            printf("\n");
        }
        d8 = 0;
        if ((c >= '0') && (c <= '9'))
        {
            d = c - 0x30;
            nibbles--;
            charactercount++;
            d8 = 1;
        }
        if ((c >= 'A') && (c <= 'F'))
        {
            d = c - 0x37;
            nibbles--;
            charactercount++;
            d8 = 1;
        }
        if ((c >= 'a') && (c <= 'f'))
        {
            d = c - 0x57;
            nibbles--;
            charactercount++;
            d8 = 1;
        }
        if (d8 == 1)
        {
            chksum = chksum + (d << (nibbles * 4));
            bytedata = bytedata + (d << (nibbles * 4));
            bytedata = bytedata & 0xff;
            if (nibbles == 0)
            {
                nibbles = 2;
            }
        }

        if ((charactercount >= 1) && (charactercount <= 2))
        {
            recordlength = (recordlength << 4) + d;
        }
        if ((charactercount >= 3) && (charactercount <= 6))
        {
            addresslow = (addresslow << 4) + d;
        }
        if ((charactercount >= 7) && (charactercount <= 8))
        {
            datatype = (datatype << 4) + d;
            if (datatype == 1)
            {
                exitcode = -2;
            }
            bytedata = 0;
        }
        if (charactercount >= 9)
        {
            if ((datatype == 4) && (charactercount <= 12))
            {
                addresshigh = (addresshigh << 4) + d;
            }
            if (datatype == 0)
            {
                if (nibbles == 2)
                {
                    recordlength = recordlength - 1;
                    if (recordlength >= 0)
                    {
                        writeaddress = addresslow + addresshigh * 0x10000;
                        if (charactercount == 10)
                        {
                        //    printf("%6x:", writeaddress);
                        }
                        writeonebytetosram(writeaddress, bytedata);
                        //printf("%2x ", bytedata);
                    }
                    bytedata = 0;
                    addresslow++;
                }
            }
            if (datatype == 1)
            {
                exitcode++;
            }
        }
    }
}
void fillentireramhex(int filldata)
{
    int writeaddress = 0, f = 0;
    f = filldata & 0xff;
    while (writeaddress < RAMCAP)
    {
        
        writeonebytetosram(writeaddress, f);
        writeaddress++;
    }
}
void writeonebytetosram(uint32_t address, uint32_t data)
{
    uint32_t addressupper, outputdata;
    gpio_set_dir_out_masked(GPIOOUTPUTS); // set gpio[0:26] as output;
    gpio_put(BUFEN_N, 0);
    outputaddress(address);
    //_nop;
    gpio_put(CS_N, 0);
    //_nop;
    addressupper = ((address >> 8) & 0xff00);
    outputdata = addressupper + (data & 0xff);
    gpio_put_masked(0xffff, outputdata);
    //_nop
    gpio_put(WR_N, 0);
    busy_wait_us(1);
    gpio_put(WR_N, 1);
    //_nop;
    gpio_put(CS_N, 1);
    gpio_put(BUFEN_N, 1);
    return;
}

int32_t readonebytefromsram(uint32_t address)
{
    uint32_t data;
    gpio_put(BUFEN_N, 0);
    outputaddress(address);
    gpio_set_dir_in_masked(0xff); // set gpio[0:7] as INPUT;
    //    _nop;
    gpio_put(CS_N, 0);
    //_nop;
    gpio_put(RD_N, 0);
    busy_wait_us(1);
    data = gpio_get_all() & 0xff;
    //_nop;
    gpio_put(RD_N, 1);
    //_nop;
    gpio_put(CS_N, 1);
    gpio_put(BUFEN_N, 1);
    gpio_set_dir_out_masked(GPIOOUTPUTS); // set gpio[0:26] as output;

    return data;
}

void outputaddress(uint32_t address)
{
    uint32_t addressupper, addresslower16;
    gpio_set_dir_out_masked(GPIOOUTPUTS); // set gpio[0:26] as output;
    addressupper = ((address >> 8) & 0xff00);
    // addresshigh=(address>>8 &0xff);
    // addresslow=(address &0xff);

    addresslower16 = address & 0xffff;
    gpio_put_masked(0xffff, addresslower16);
    gpio_put(ASTB, 1); // release latch
    busy_wait_us(1);
    gpio_put(ASTB, 0);                     // hold low and high address
                                           //_nop
    gpio_put_masked(0xff00, addressupper); // A16-A23
}
