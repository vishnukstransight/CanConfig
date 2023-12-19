#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>  // Add this for malloc function
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

const char* const fhtp_cota_filename_str = "sample.cfg" ;
#pragma pack(1)
typedef struct 
{
	char canid[9];
	uint8_t idmod;
	uint8_t frametype;
	uint8_t dlc;
	char databytes[19];
	uint8_t tx_msg_type;
	uint16_t cycletime;
}SCanCfgTx;
#pragma pack()
#pragma pack(1)
typedef struct
{
    char canrxid[9];//7E9
    uint8_t rxidmod;//0
    uint8_t rxframetype;//0
    uint8_t rxmsgtype;//0
    uint16_t rxcycletime;//10
    uint8_t rxfilterno;//5
    uint8_t rx_filterlength1;
    uint8_t rx_filterlength2;
    uint8_t rx_filterlength3;
    uint8_t rx_filterlength4;
    uint8_t rx_filterlength5;
    char rxfilterdata1[33];
    char rxfilterdata2[33];
    char rxfilterdata3[33];
    char rxfilterdata4[33];
    char rxfilterdata5[33];
}SCanCfgRx;
#pragma pack()
#pragma pack(1)
typedef struct
{
	char version[10]; // uint8_t = 1 byte(00)
    int channel;
    int Glb_mode;
    char Baudrate[10]; //uint32_t = 4byte(0000 0000)
    int Num_Tx;
    int Num_Rx;
}SCanCfgCan;
#pragma pack()
SCanCfgCan OCanCfgCan;
SCanCfgTx  *pCanCfgTx;
SCanCfgRx  *pCanCfgRx;
char *pData = 0;
char *pTemp = 0;
char *pNext = 0;
char *token = 0;
void split_data(char *buffer, char *pTx, char *pRx)
{
	int can_count = 0,tx_count = 0,rx_count = 0;

	char *line = strtok(buffer,"\n");

	// DEBUG_PRINT("Value in line  = %s",line);

	while(line != NULL)
	{
		
		if(strncmp(line,"TX",2) == 0)
		{
			// DEBUG_PRINT("Value in line in TX = %s",line);
			// strcpy(pTx + tx_count, line);
			// tx_count += strlen(line); 
			memcpy(pTx + tx_count, line, strlen(line) +1);
			tx_count += strlen(line);
		}
		else if(strncmp(line,"RX",2) == 0)
		{
			// DEBUG_PRINT("Value in line in RX = %s",line);
			// strcpy(pRx + rx_count, line);
			// rx_count += strlen(line); 
			memcpy(pRx + rx_count, line, strlen(line)+1);
			rx_count += strlen(line);

		}

		line = strtok(NULL,"\n");
	}


}
void cota_mgr_verify_cota_file_download()
{

FILE *fd;
#if 1
        int ret=0;

	fd = fopen(fhtp_cota_filename_str,"r");

	if( fd == NULL)
	{
		printf("........fopen error");
	}




	ret = fseek(fd,0,SEEK_END);

	
	//if(ret <= NULL)
	//{
		//printf("fseek failed");
	//}

	ret = ftell(fd);

	printf("downloaded file size in bytes = %d",ret);


	fseek(fd,0,SEEK_SET);
	char *pBuffer = malloc(ret + 1);
	if(pBuffer == NULL)
	{	
	printf("size of pBuffer = %zu",sizeof(pBuffer));
	printf("Error allocating buffer");
	fclose(fd);
	}
	
		// //read the file contents into the buffer
	size_t bytes_read = fread(pBuffer, 1, ret, fd); 
	
	if(bytes_read != ret)
	{
		printf("Error reading file \n");
		free(pBuffer);
		fclose(fd);
	}
	
		//add a null terminator to the end of the buffer
	pBuffer[ret] = '\0';
	//print the file contents to the console
	printf("Value in buffer = %s\n",pBuffer);
	
	//******************************
	pData = pBuffer;
	
	pTemp = strtok_r(pBuffer,"\n",&pData);
	printf("value in pTemp = %s\n",pTemp);
	printf("value in pData = %s\n",pData);
	char *pCan = malloc(50 * sizeof(char));
	memcpy(pCan,pTemp,strlen(pTemp));
	printf("value in pCan = %s\n",pCan);
	printf("................value in pData = %s\n",pData);
	pNext = pCan;
	pTemp = strtok_r(pCan,",",&pNext);

	printf("value in pTemp = %s\n",pTemp);

	printf("value in pNext = %s\n",pNext);
	
	if(strcmp(pTemp,"CAN") == 0)
	{
		sscanf(pNext,"%9[^,],%d,%d,%9[^,],%d,%d",OCanCfgCan.version,&OCanCfgCan.channel,&OCanCfgCan.Glb_mode,
											OCanCfgCan.Baudrate,&OCanCfgCan.Num_Tx,&OCanCfgCan.Num_Rx);
											// Read a string up to 9 characters (excluding commas)
              								// Read an integer
											// Read an integer
											// Read a string up to 9 characters (excluding commas)
											// Read an integer
											// Read an integer
	}

	printf("version = %s",OCanCfgCan.version);
	
	printf("channel = %d",OCanCfgCan.channel);
	printf("Glb_mode = %d",OCanCfgCan.Glb_mode);
	printf("Baudrate = %s",OCanCfgCan.Baudrate);
	
	printf("Num_Tx = %d",OCanCfgCan.Num_Tx);
	printf("Num_Rx = %d",OCanCfgCan.Num_Rx);
#endif

char *pTx = malloc(32 * 41);
if (pTx == NULL) {
    printf("Memory allocation failed for pTx");
}
//strcpy(pTx, "TX,7C1,1,1,8,00AA11AABB000011,0,500,TX,7C2,0,0,8,00010012BAAFBB44,0,500,TX,142EFF95,1,0,8,000122AABB0011AA,0,500,TX,142EFF96,1,0,8,000122CB11005600,0,500");
char *pRx = malloc(30*198);
if (pRx == NULL) {
    printf("Memory allocation failed for pRx");
}

split_data(pData,pTx,pRx);

free(pBuffer);
#if 0
strcpy(pRx, "RX,7E9,0,0,0,10,1,8,0,0,0,0,00AA01AB02AC03AD04AE05AF06CC07DD,00xx01xx02xx03xx04xx05xx06xx07xx,00xx01xx02xx03xx04xx05xx06xx07xx,00xx01xx02xx03xx04xx05xx06xx07xx,00xx01xx02xx03xx04xx05xx06xx07xx,RX,253DEE80,0,0,0,10,2,8,8,0,0,0,00AA01AB02AC03AD04AE05AF06CC07DD,00AA01AB02AC03AD04AE05AF06CC07DD,00xx01xx02xx03xx04xx05xx06xx07xx,00xx01xx02xx03xx04xx05xx06xx07xx,00xx01xx02xx03xx04xx05xx06xx07xx,RX,253DEE81,0,0,0,10,3,8,8,8,0,0,00AA01AB02AC03AD04AE05AF06CC07DD,00AA01AB02AC03AD04AE05AF06CC07DD,00AA01AB02AC03AD04AE05AF06CC07DD,00xx01xx02xx03xx04xx05xx06xx07xx,00xx01xx02xx03xx04xx05xx06xx07xx,RX,253DEE82,0,0,0,10,4,8,8,8,8,0,00AA01AB02AC03AD04AE05AF06CC07DD,00AA01AB02AC03AD04AE05AF06CC07DD,00AA01AB02AC03AD04AE05AF06CC07DD,00AA01AB02AC03AD04AE05AF06CC07DD,00xx01xx02xx03xx04xx05xx06xx07xx,RX,253DEE82,0,0,0,10,5,8,8,8,8,8,00AA01AB02AC03AD04AE05AF06CC07DD,00AA01AB02AC03AD04AE05AF06CC07DD,00AA01AB02AC03AD04AE05AF06CC07DD,00AA01AB02AC03AD04AE05AF06CC07DD,00AA01AB02AC03AD04AE05AF06CC07DD");
#endif
	//printf("..........value in pTx = %s\n",pTx);
	//printf("..........value in pRx = %s\n",pRx);
	pCanCfgTx = (SCanCfgTx *)malloc(sizeof(SCanCfgTx) * 32);

	if(pCanCfgTx == NULL)
	{
		printf("Memory allocation failed for pRx");
	}
	pNext = pTx;
	pTemp = strtok_r(pTx,",",&pNext);

	//printf("Value in pTemp in Tx = %s\n",pTemp);
	//printf("Value in pNext in Tx = %s\n",pNext);

	if(strcmp(pTemp,"TX") == 0)
	{
		int i = 0;
		pData = pNext;
		token = strtok(pNext,"TX");
		while(token != NULL)
		{
		      if(i == 0)
		      {
		      #if 0
			printf("Value in token  = %s\n",token);
			printf("Value in i  = %d\n",i);
		      #endif
			sscanf(token,"%9[^,],%hhd,%hhd,%hhd,%19[^,],%hhd,%hd",pCanCfgTx[i].canid,&pCanCfgTx[i].idmod,&pCanCfgTx[i].frametype,
						&pCanCfgTx[i].dlc,pCanCfgTx[i].databytes,&pCanCfgTx[i].tx_msg_type,&pCanCfgTx[i].cycletime);
			token = strtok(NULL,"TX");
			// token = strtok_r(NULL,"TX",&pData);
			#if 0
			printf("Value in canid[%d]  = %s\n",i,pCanCfgTx[i].canid);
			printf("Value in idmod[%d]  = %d\n",i,pCanCfgTx[i].idmod);
			printf("Value in frametype[%d]  = %d\n",i,pCanCfgTx[i].frametype);
			printf("Value in dlc[%d]  = %d\n",i,pCanCfgTx[i].dlc);
			printf("Value in databytes[%d]  = %s\n",i,pCanCfgTx[i].databytes);
			printf("Value in tx_msg_type[%d]  = %d\n",i,pCanCfgTx[i].tx_msg_type);
			printf("Value in cycletime[%d]  = %d\n",i,pCanCfgTx[i].cycletime);
			#endif
			i++;
		      }
			else
		      {
		       #if 0
			printf("Value in token  = %s\n",token+1);
			printf("Value in i  = %d\n",i);
		       #endif
			sscanf(token+1,"%9[^,],%hhd,%hhd,%hhd,%19[^,],%hhd,%hd",pCanCfgTx[i].canid,&pCanCfgTx[i].idmod,&pCanCfgTx[i].frametype,
						&pCanCfgTx[i].dlc,pCanCfgTx[i].databytes,&pCanCfgTx[i].tx_msg_type,&pCanCfgTx[i].cycletime);
			token = strtok(NULL,"TX");
			// token = strtok_r(NULL,"TX",&pData);
			#if 0
			printf("Value in canid[%d]  = %s\n",i,pCanCfgTx[i].canid);
			printf("Value in idmod[%d]  = %d\n",i,pCanCfgTx[i].idmod);
			printf("Value in frametype[%d]  = %d\n",i,pCanCfgTx[i].frametype);
			printf("Value in dlc[%d]  = %d\n",i,pCanCfgTx[i].dlc);
			printf("Value in databytes[%d]  = %s\n",i,pCanCfgTx[i].databytes);
			printf("Value in tx_msg_type[%d]  = %d\n",i,pCanCfgTx[i].tx_msg_type);
			printf("Value in cycletime[%d]  = %d\n",i,pCanCfgTx[i].cycletime);
			#endif
			i++;
		      }
		}
	}	
	
	
	pCanCfgRx = (SCanCfgRx *)malloc(sizeof(SCanCfgRx) * 30);

	if(pCanCfgRx == NULL)
	{
		printf("Memory allocation failed for pRx");
	}

	pNext = pRx;

	pTemp = strtok_r(pRx,",",&pNext);

	//printf("Value in pTemp in Rx = %s\n",pTemp);

	//printf("Value in pNext in Rx = %s\n",pNext);

if(strcmp(pTemp,"RX") == 0)
    {
      int  i = 0;
        token = strtok(pNext,"RX");
        while(token != NULL)
        {
            if(i == 0)
            {
                // DEBUG_PRINT("Value in pRx in Rx = %s\n",pRx);
                printf("Value in token  = %s\n",token);
                printf("Value in i  = %d\n",i);
                sscanf(token,"%9[^,],%hhd,%hhd,%hhd,%hd,%hhd,%hhd,%hhd,%hhd,%hhd,%hhd,%33[^,],%33[^,],%33[^,],%33[^,],%33[^,]",pCanCfgRx[i].canrxid,&pCanCfgRx[i].rxidmod,
                        &pCanCfgRx[i].rxframetype,&pCanCfgRx[i].rxmsgtype,&pCanCfgRx[i].rxcycletime,&pCanCfgRx[i].rxfilterno,
                        &pCanCfgRx[i].rx_filterlength1,&pCanCfgRx[i].rx_filterlength2,&pCanCfgRx[i].rx_filterlength3,
                        &pCanCfgRx[i].rx_filterlength4,&pCanCfgRx[i].rx_filterlength5,pCanCfgRx[i].rxfilterdata1,
                        pCanCfgRx[i].rxfilterdata2,pCanCfgRx[i].rxfilterdata3,pCanCfgRx[i].rxfilterdata4,
                        pCanCfgRx[i].rxfilterdata5);
                token = strtok(NULL,"RX");
                printf("Value in canrxid[%d]  = %s\n",i,pCanCfgRx[i].canrxid);
                printf("Value in rxidmod[%d]  = %d\n",i,pCanCfgRx[i].rxidmod);
                printf("Value in rxframetype[%d]  = %d\n",i,pCanCfgRx[i].rxframetype);
                printf("Value in rxmsgtype[%d]  = %d\n",i,pCanCfgRx[i].rxmsgtype);
                printf("Value in rxcycletime[%d]  = %d\n",i,pCanCfgRx[i].rxcycletime);
                printf("Value in rxfilterno[%d]  = %d\n",i,pCanCfgRx[i].rxfilterno);
                printf("Value in rx_filterlength1[%d]  = %d\n",i,pCanCfgRx[i].rx_filterlength1);
                printf("Value in rx_filterlength2[%d]  = %d\n",i,pCanCfgRx[i].rx_filterlength2);
                printf("Value in rx_filterlength3[%d]  = %d\n",i,pCanCfgRx[i].rx_filterlength3);
                printf("Value in rx_filterlength4[%d]  = %d\n",i,pCanCfgRx[i].rx_filterlength4);
                printf("Value in rx_filterlength5[%d]  = %d\n",i,pCanCfgRx[i].rx_filterlength5);
                printf("Value in rxfilterdata1[%d]  = %s\n",i,pCanCfgRx[i].rxfilterdata1);
                printf("Value in rxfilterdata2[%d]  = %s\n",i,pCanCfgRx[i].rxfilterdata2);
                printf("Value in rxfilterdata3[%d]  = %s\n",i,pCanCfgRx[i].rxfilterdata3);
                printf("Value in rxfilterdata4[%d]  = %s\n",i,pCanCfgRx[i].rxfilterdata4);
                printf("Value in rxfilterdata5[%d]  = %s\n",i,pCanCfgRx[i].rxfilterdata5);
                i++;
            }
            else
            {
                // DEBUG_PRINT("Value in pRx in Rx = %s\n",pRx);
                printf("Value in token  = %s\n",token);
                printf("Value in i  = %d\n",i);
            sscanf(token+1,"%9[^,],%hhd,%hhd,%hhd,%hd,%hhd,%hhd,%hhd,%hhd,%hhd,%hhd,%33[^,],%33[^,],%33[^,],%33[^,],%33[^,]",pCanCfgRx[i].canrxid,&pCanCfgRx[i].rxidmod,
                        &pCanCfgRx[i].rxframetype,&pCanCfgRx[i].rxmsgtype,&pCanCfgRx[i].rxcycletime,&pCanCfgRx[i].rxfilterno,
                        &pCanCfgRx[i].rx_filterlength1,&pCanCfgRx[i].rx_filterlength2,&pCanCfgRx[i].rx_filterlength3,
                        &pCanCfgRx[i].rx_filterlength4,&pCanCfgRx[i].rx_filterlength5,pCanCfgRx[i].rxfilterdata1,
                        pCanCfgRx[i].rxfilterdata2,pCanCfgRx[i].rxfilterdata3,pCanCfgRx[i].rxfilterdata4,
                        pCanCfgRx[i].rxfilterdata5);
                token = strtok(NULL,"RX");
                printf("Value in canrxid[%d]  = %s\n",i,pCanCfgRx[i].canrxid);
                printf("Value in rxidmod[%d]  = %d\n",i,pCanCfgRx[i].rxidmod);
                printf("Value in rxframetype[%d]  = %d\n",i,pCanCfgRx[i].rxframetype);
                printf("Value in rxmsgtype[%d]  = %d\n",i,pCanCfgRx[i].rxmsgtype);
                printf("Value in rxcycletime[%d]  = %d\n",i,pCanCfgRx[i].rxcycletime);
                printf("Value in rxfilterno[%d]  = %d\n",i,pCanCfgRx[i].rxfilterno);
                printf("Value in rx_filterlength1[%d]  = %d\n",i,pCanCfgRx[i].rx_filterlength1);
                printf("Value in rx_filterlength2[%d]  = %d\n",i,pCanCfgRx[i].rx_filterlength2);
                printf("Value in rx_filterlength3[%d]  = %d\n",i,pCanCfgRx[i].rx_filterlength3);
                printf("Value in rx_filterlength4[%d]  = %d\n",i,pCanCfgRx[i].rx_filterlength4);
                printf("Value in rx_filterlength5[%d]  = %d\n",i,pCanCfgRx[i].rx_filterlength5);
                printf("Value in rxfilterdata1[%d]  = %s\n",i,pCanCfgRx[i].rxfilterdata1);
                printf("Value in rxfilterdata2[%d]  = %s\n",i,pCanCfgRx[i].rxfilterdata2);
                printf("Value in rxfilterdata3[%d]  = %s\n",i,pCanCfgRx[i].rxfilterdata3);
                printf("Value in rxfilterdata4[%d]  = %s\n",i,pCanCfgRx[i].rxfilterdata4);
                printf("Value in rxfilterdata5[%d]  = %s\n",i,pCanCfgRx[i].rxfilterdata5);
                i++;
            }
        }
    }
	free(pCanCfgTx);
	free(pCanCfgRx); 
	//free(pCan);
	free(pTx);
	free(pRx);
	//ql_fclose(fd);
        

}
int main()
{
cota_mgr_verify_cota_file_download();
}
