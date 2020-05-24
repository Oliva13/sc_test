#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char done;
    unsigned char buf[256];
    unsigned char recvbuf[256];
    unsigned long recvlen;
    int len = 254;
    int i, k;
    buf[0] = 0x00;
    buf[1] = 0x84;
    buf[2] = 0x00;
    buf[3] = 0x00;
    buf[4] = len;
    unsigned long commlen;
    if (!init_sc()) 
    {

	   commlen = 5;
	   recvlen = (unsigned long) sizeof(recvbuf);
	   printf("-----> %02x %02x %02x %02x %02x\n", buf[0], buf[1], buf[2], buf[3], buf[4]);
	   for(k = 0; k < 1024; k++)
           {
		if ( !sendAPDU(buf, commlen, recvbuf, &recvlen)) 
		{
			printf("recvlen = %ld\n", recvlen);
			printf("count - %d\n", k);
			printf("<----- ");
			for (i = 0; i < recvlen; i++) printf("%02x ", recvbuf[i]);
			printf("\n\n");
		}
		else
		{
		    printf("Error: sendAPDU\n");
		}
    	   } 
    }
    else printf("\nInit SC......FAILURE\n");
    return (0);
}

