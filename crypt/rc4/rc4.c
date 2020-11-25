#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int initST(unsigned char *S, unsigned char *T, unsigned char *K, int len)
{
        int i = 0;

        for(i=0; i<256; i++)
        {
                S[i] = i;
                T[i] = K[i%len];
        }

        return 0;
}

int initS(unsigned char *S, unsigned char *T)
{
        unsigned char tmp = 0x00;
        int i = 0;
        int j = 0;

        for(i=0; i<256; i++)
        {
                j = (j + S[i] + T[i]) % 256;
                tmp = S[j];
                S[j] = S[i];
                S[i] = tmp;
        }

        return 0;
}

int initK(unsigned char *S, unsigned char *K, int len)
{
        unsigned char tmp = 0x00;
        int i = 0;
        int j = 0;
        int r = 0;
        int t = 0;

        for(r=0; r<len; r++)
        {
                i = (i + 1) % 256;
                j = (j + S[i]) % 256;
                tmp = S[j];
                S[j] = S[i];
                S[i] = tmp;
                t = (S[i] + S[j]) % 256;
                K[r] = S[t];
        }

        return 0;
}

int rc4_enc(unsigned char *K, unsigned char *M, unsigned char *E, int len)
{
        int i = 0;

        for(i=0; i<len; i++)
        {
                E[i] = (M[i] ^ K[i]);
        }

        return 0;
}

int main()
{
        unsigned char S[256];
        unsigned char T[256];
        unsigned char K[256];
        unsigned char M[256];
        unsigned char E[256];
        unsigned char C[256];

        memset(S, 0x00, sizeof(S));
        memset(T, 0x00, sizeof(T));
        memset(K, 0x00, sizeof(K));
        memset(E, 0x00, sizeof(E));
        memset(C, 0x00, sizeof(C));

        strcpy(K, "123");
        strcpy(M, "你好");

        initST(S, T, K, 4);
        initS(S, T);
        initK(S, K, 256);

        rc4_enc(K, M, E, 256);
        rc4_enc(K, E, C, 256);

        printf("%s", E);

        return 0;
}
