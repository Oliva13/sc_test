#include <stdio.h>
#include <string.h>
#include "pcsclite.h"
#include "winscard.h"
#include "reader.h"

int sendAPDU(BYTE *, DWORD, BYTE * , DWORD *);
static void show_error(LONG);

static SCARDCONTEXT hContext;
static SCARDHANDLE phCard;

int init_sc(void)
{
	int tmp_cnt;
	int i;
	LONG rv;
	DWORD dwReaders = SCARD_AUTOALLOCATE;
	char *rp = NULL;
	char *readers = NULL;
	char *mszGroups; 	
	DWORD dwActiveProtocol;
	
	/*DWORD dwSendLength, pcbRecvLenth;
	SCARD_IO_REQUEST pioRecvPci;
	BYTE pbRecvBuffer[256];*/
	
	BYTE pbAtr[MAX_ATR_SIZE];
	DWORD dwAtrLen, dwState, dwReaderLen, dwProtocol;
	printf("Trying to establish SC context......");
	rv = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &hContext);
	if (rv == SCARD_S_SUCCESS) 
	{
	    printf("OK\n");
	    printf("Trying to get readers' info......");
		rv = SCardListReaders(hContext, NULL, (LPSTR)&readers, &dwReaders);
   	    if (rv == SCARD_S_SUCCESS) 
		{
			printf("OK\n");
			/*  TODO: show all readers here later */
			if (readers != NULL) 
			{
				printf("Available readers:\n");
				rp = readers;
				i = 1;
				while('\0' != *rp) 
				{
					printf("%d): %s\n", i++, rp);	
					rp += strlen(rp) + 1;
				}
				printf("WARNING: Using first available reader: %s\n", readers);
				printf("Trying to connect to card.....");
				rv = SCardConnect(hContext, readers, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0, &phCard, &dwActiveProtocol);
				if (rv == SCARD_S_SUCCESS) 
				{
					printf("OK\n");
					printf("Trying to get card's ATR .....");
					dwAtrLen = sizeof(pbAtr);
					rv = SCardStatus(phCard, NULL, &dwReaderLen, &dwState, &dwProtocol, pbAtr, &dwAtrLen);
					if ( rv == SCARD_S_SUCCESS ) 
					{
						printf("ATR: ");
						for ( i = 0; i < dwAtrLen; i++) printf ("%02x ", pbAtr[i]);
						printf("\n\n");
						return (0);
					} else {printf("FAILURE: %s\n", pcsc_stringify_error(rv)); return (-1);}
				} else {printf("FAILURE: %s\n", pcsc_stringify_error(rv)); return (-1);}	
			} else {printf("Readers is null\n"); return (-1);}	
		} else {printf("FAILURE\n"); return (-1);}
	} else {printf("FAILURE\n"); return (-1);}
}

int sendAPDU(BYTE *command, DWORD comm_len, BYTE *response, DWORD *resp_len)
{
	LONG res;
	SCARD_IO_REQUEST pioRecvPci;
	/*printf("SCardTransmit()......");*/
	res = SCardTransmit( phCard, SCARD_PCI_T0, command, comm_len, &pioRecvPci, response, resp_len ); 
	if (res == SCARD_S_SUCCESS) 
	{
		return (0);							
	} else { printf("error: %s\n", pcsc_stringify_error(res)); return (-1);}
}

