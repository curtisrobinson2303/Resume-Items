//
//  main.cpp
//  PanicLogTool
//
//  Created by Curtis Robinson on 8/14/23.
//  Other World Computing

#include "PanicLog.hpp"

char * UnmangeLogFile(const char * sourceBufferPtr, uint32_t bufferLength);

int main(int argc, const char * argv[])
{
    ssize_t  BytesRead;
    
    uint8_t SourceFD;
    uint8_t DestinationFD;
    
    const char * CMDSourceFilePath;
    const char * CMDDestinationFilePath;
    
    char * SourceBuffer = new char[1];
    char * ResultBuffer = new char[1];//
    
    //Initializing Variables
    CMDSourceFilePath = argv[1];
    CMDDestinationFilePath = argv[2];

    SourceFD = open(CMDSourceFilePath, O_RDONLY);
    DestinationFD = open(CMDDestinationFilePath, O_WRONLY | O_CREAT | O_TRUNC);
    
    BytesRead = read(SourceFD, SourceBuffer, 1073741824);
    
    ResultBuffer = UnmangeLogFile(SourceBuffer, strlen(SourceBuffer));
    
    cout << strlen(ResultBuffer) << endl;
    int i;
    for(i = 0; i < strlen(ResultBuffer)*8; i++)
    {
        cout << ResultBuffer[i];
    }
    
    close(SourceFD);
    close(DestinationFD);
    
    return 1;
}





//    ====================================================================================
//    ShowHelp()
//    ====================================================================================
void ShowHelp()
{
    std::cout << "### PanicLog Testing Tool:\n"
        << "    \n"
        << "    pathname                # pathname to file, e.g: "
        << "/User/example.panic or /User/example.txt\n\n"
        << "examples:\n"
        << "    PanicLogTool /User/input.panic /User/output.txt\n\n";
}
