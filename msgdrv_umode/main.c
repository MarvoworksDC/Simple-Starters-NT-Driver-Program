#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <WINIOCTL.H>

#include "..\ioctl.h"

void printError(TCHAR* msg, unsigned int eNum)
{
    TCHAR* p;
    TCHAR sysMsg[256];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, eNum, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),/* Default language */sysMsg, 256, NULL);
    //Trim the end of the line and terminate it with a null
    p = sysMsg;
    while((*p > 31) || (*p == 9))
    ++p;
    do{*p-- = 0; }
       while((p > sysMsg) && ((*p == '.') || (*p < 33)));
    //display the message
    printf("WARNING: %s failed with error %d(%s)\n\n", msg, eNum, sysMsg);
}

int main(int argc, char *argv[])
{
    HANDLE hFile;
    DWORD dwReturn;
    char szTemp[256] = {0};    
    
    SetConsoleTitle("::-::-::Driver CreateFile().Connection Testing::-::-::");
    
    hFile = CreateFile("\\\\.\\MsgDrv", GENERIC_READ | GENERIC_WRITE, 0 , NULL, OPEN_EXISTING, 0, NULL);
    
    if(hFile == INVALID_HANDLE_VALUE)
    {
          printf("Error %d occured while CreateFile() to the Driver\n\n", GetLastError()); 
          printError("CreateFile()", GetLastError());              
    }else
    {
        printf("CreateFile() to the Driver successful!\n\n");
          
        WriteFile(hFile, "** Devmarvo on the machine sys", sizeof("** Devmarvo on the machine sys"), &dwReturn, NULL);
        
        printf("WriteFile completed successful!\n\n");
        printf("Message sent to the driver(** Devmarvo on the machine sys)\n\n");
         
        ReadFile(hFile, &szTemp, 256, &dwReturn, NULL);
        printf("ReadFile completed successful!\n\n");
        
        printf("Message received from processor:\n");
        printf(szTemp);
        printf("\n\n");    
        
        ZeroMemory(szTemp, sizeof(szTemp));
        DeviceIoControl(hFile, IOCTL_MSGDRV_NEITHER_IO, "** Hello from User Mode Neither I/O", sizeof("** Hello from User Mode Neither I/O"), szTemp, sizeof(szTemp), &dwReturn, NULL);
        printf(szTemp);
        printf("\n\n");              
                            
        CloseHandle(hFile);  
    }
  
  system("PAUSE");	
  return 0;
}
