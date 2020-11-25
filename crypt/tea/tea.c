#include <stdio.h>
#include <string.h>

int tea_encrypt(const unsigned char *Key,unsigned char *Data,int BlockCount)
{
    unsigned int y,z,sum,i,j;
    unsigned int delta = 0x9e3779b9;  /* a key schedule constant */
    unsigned int a, b, c, d;   /* cache key */

    a = (unsigned int)((Key[0] << 24) | (Key[1] << 16) | (Key[2] << 8) | Key[3]);
    b = (unsigned int)((Key[4] << 24) | (Key[5] << 16) | (Key[6] << 8) | Key[7]);
    c = (unsigned int)((Key[8] << 24) | (Key[9] << 16) | (Key[10] << 8) | Key[11]);
    d = (unsigned int)((Key[12] << 24) | (Key[13] << 16) | (Key[14] << 8) | Key[15]);
    
    for(i = 0; i < BlockCount;i++)
    {
        sum = 0;
        y = (unsigned int)((Data[i*8 + 0] << 24) | (Data[i*8 + 1] << 16) | (Data[i*8 + 2] << 8) | Data[i*8 + 3]);
        z = (unsigned int)((Data[i*8 + 4] << 24) | (Data[i*8 + 5] << 16) | (Data[i*8 + 6] << 8) | Data[i*8 + 7]);

        for(j = 0; j < 32; j++)
        {
            sum += delta;
            y += ((z<<4) + a) ^ (z + sum) ^ ((z>>5) + b);
            z += ((y<<4) + c) ^ (y + sum) ^ ((y>>5) + d);
        }

        Data[i*8 + 0] = (unsigned char)((y >> 24) & 0xFF);
        Data[i*8 + 1] = (unsigned char)((y >> 16) & 0xFF);
        Data[i*8 + 2] = (unsigned char)((y >> 8) & 0xFF);
        Data[i*8 + 3] = (unsigned char)((y >> 0) & 0xFF);

        Data[i*8 + 4] = (unsigned char)((z >> 24) & 0xFF);
        Data[i*8 + 5] = (unsigned char)((z >> 16) & 0xFF);
        Data[i*8 + 6] = (unsigned char)((z >> 8) & 0xFF);
        Data[i*8 + 7] = (unsigned char)((z >> 0) & 0xFF);

    }
    return 0;
    
}

int tea_decrypt(const unsigned char *Key,unsigned char *Data,int BlockCount)
{
    unsigned int y, z,sum,i,j; /* set up */
    unsigned int delta=0x9e3779b9; /* a key schedule constant */
    unsigned int a, b, c, d;    /* cache key */

    a = (unsigned int)((Key[0] << 24) | (Key[1] << 16) | (Key[2] << 8) | Key[3]);
    b = (unsigned int)((Key[4] << 24) | (Key[5] << 16) | (Key[6] << 8) | Key[7]);
    c = (unsigned int)((Key[8] << 24) | (Key[9] << 16) | (Key[10] << 8) | Key[11]);
    d = (unsigned int)((Key[12] << 24) | (Key[13] << 16) | (Key[14] << 8) | Key[15]);

    for(i = 0; i < BlockCount; i++)
    {
        sum=0xC6EF3720;
        y = (unsigned int)((Data[i*8 + 0] << 24) | (Data[i*8 + 1] << 16) | (Data[i*8 + 2] << 8) | Data[i*8 + 3]);
        z = (unsigned int)((Data[i*8 + 4] << 24) | (Data[i*8 + 5] << 16) | (Data[i*8 + 6] << 8) | Data[i*8 + 7]);
        for(j = 0; j < 32; j++)
        {
            z -= ((y<<4) + c) ^ (y + sum) ^ ((y>>5) + d);
            y -= ((z<<4) + a) ^ (z + sum) ^ ((z>>5) + b);
            sum -= delta;  
        }

        Data[i*8 + 0] = (unsigned char)((y >> 24) & 0xFF);
        Data[i*8 + 1] = (unsigned char)((y >> 16) & 0xFF);
        Data[i*8 + 2] = (unsigned char)((y >> 8) & 0xFF);
        Data[i*8 + 3] = (unsigned char)((y >> 0) & 0xFF);

        Data[i*8 + 4] = (unsigned char)((z >> 24) & 0xFF);
        Data[i*8 + 5] = (unsigned char)((z >> 16) & 0xFF);
        Data[i*8 + 6] = (unsigned char)((z >> 8) & 0xFF);
        Data[i*8 + 7] = (unsigned char)((z >> 0) & 0xFF);
    }

    return 0;

}
void main(void)
{
    unsigned char i;
    unsigned char buf[32];
    const unsigned char key[16] = {0x00,0x01,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0xe,0xf,0x00};
    const unsigned char data[32] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x01};


    memcpy((void*)buf,data,32);
    tea_encrypt(key,buf,4);
    printf("sc_tea cipher data: \n");
    for(i = 0; i < 32; i++)
    {
        printf("%02x ",buf[i]);   
    }
    printf("\n");
    if(tea_decrypt(key,buf,4) == 0x0)
    {
        printf("sc_tea plain data: \n");
        for(i = 0; i < 32; i++)
        {
            printf("%02x ",buf[i]);   
        }
        printf("\n");
    }
    return;
}
