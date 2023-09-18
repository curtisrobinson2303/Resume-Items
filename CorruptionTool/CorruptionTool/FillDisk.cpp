//
//  FillDisk.cpp
//  CorruptionTool
//
//  Created by Curtis Robinson on 8/9/23.
//  OWC

#include "FillDisk.hpp"

uint64_t * OutputBuffer;
uint64_t * ParitySelfCalc;
uint64_t * ParityDriverCalc;

uint64_t StripeSize;
uint64_t TotalBytesRead;
uint64_t StripeUnitSize;
uint64_t TotalBytesWritten;



#pragma mark   USER INPUT HANDELERS


//    ====================================================================================
//    UpdateArgs()
//    ====================================================================================
void CorruptionTool::UpdateArgs(int argcs, char * argvs[])
{
    if(string(argvs[1]) == "help")
    {
        helpFlag = true;
        runFlag = false;
    }
    else
    {
        runFlag = true;
        helpFlag = false;
        
        // Checking if user output is directed to the file system or to the disk
        if ( strcmp(argvs[1],"file") == 0){ // true if the strings are identical
            fileORdisk = 1;
        }
        else if ( strcmp(argvs[1],"disk") == 0) // true if the strings are identical
        {
            fileORdisk = 0;
        }
        else
        {
            cerr << "Please check CMD Line Arguments" << endl;
            exit(1);
        }
        
        // Command Line Argument Stored
        FileSize = convertTo64(argvs[2]);
        fIO_Size = convertTo64(argvs[3]);
        VolumeOffset = convertTo64(argvs[4]);
        ParityDiskPath = argvs[5];
        VolumeDiskPath = argvs[6];
        OutputFilePath = argvs[7];
        
        StripeSize = 49152;         // Fixed Values for now, can adjust later. -> 48kb
        StripeUnitSize = 16384;     // Fixed Values for now, can adjust later. -> 16kb
        
        if (FileSize % StripeSize != 0)      //This will adjust the filesize so it is divisable by the StripeSize
        {
            FileSize =  FileSize - (FileSize % StripeSize);
        }
        
        if (VolumeOffset % StripeSize != 0) //This will adjust the VolumeOffset so it is divisible by the StripeSize
        {
            VolumeOffset =  VolumeOffset - (VolumeOffset % StripeSize);
        }

        if ((fIO_Size % StripeSize) != 0)
        {
            cout
            << "\nCommand Line Argument Error: the BufferSize must be a multiple of"
            << "48kb up to 16MB" << endl;
            exit(1);
        }
    }
}

//    ====================================================================================
//    InitTool()
//    ====================================================================================
void CorruptionTool::InitTool()
{
    BufferSize = fIO_Size;      //File IO size must be same as output buffer size
    originalOffset = VolumeOffset;
    
    TotalBytesRead = 0;
    ParityCalcErrorCounter = 0;

    uint64_t *OutputBuffers = new uint64_t[BufferSize/8];
    OutputBuffer = OutputBuffers;
    
    uint64_t *paritySelfCalcs = new uint64_t[StripeUnitSize/8];
    ParitySelfCalc = paritySelfCalcs;
    
    uint64_t *parityDriverCalc = new uint64_t[StripeUnitSize/8];
    ParityDriverCalc = parityDriverCalc;
}

//    ====================================================================================
//    DenitTool()
//    ====================================================================================
void CorruptionTool::DenitTool()
{
    delete OutputBuffer;
    delete ParitySelfCalc;
    delete ParityDriverCalc;
}




#pragma mark   RAW DISK FUNCTIONS


//    ====================================================================================
//    WriteDataToDisk()
//    ====================================================================================x
void CorruptionTool::WriteDataToDisk()
{
    cout << "Running Disk Write..." << endl;
    
    int fd_TestVolume;
    int fd_ParityDisk;
    int fd_OutputErrorFile;

    ssize_t bytesToWrite = 0;
    ssize_t bytesWritten = 0;
    uint64_t bytesRemaining = 0;
    uint64_t bytesInBlockRemaining = 0;
    
    bytesRemaining = FileSize;
    
    fd_ParityDisk = open(ParityDiskPath, O_RDONLY);
    fd_TestVolume = open(VolumeDiskPath, O_RDWR | O_CREAT);
    fd_OutputErrorFile = open(OutputFilePath, O_RDWR | O_CREAT | O_TRUNC);
    
    chmod(VolumeDiskPath, S_IRGRP | S_IROTH | S_IRUSR  | S_IWUSR);

    if ((fd_TestVolume < 0) || (fd_ParityDisk < 0) || (fd_OutputErrorFile < 0))
    {
        cerr << "\n\n#### Error: Unable to open the disk or file at: "
        << VolumeDiskPath << ", " << ParityDiskPath << " and " << OutputFilePath << ".\n"
        << "Do you have mounted volumes on this disk?  (If so unmount them.)\n"
        << "Are you running as root? (If not, sudo this command.)\n"
        << "Do you have write access to this directory if you are creating a "
        << "file?\n"
        << "Are you using the exists flag when the file doesn't exist?\n";
        
        runSuccess = false;
    }
    else
    {
        while (bytesRemaining > 0)
        {
            bytesInBlockRemaining = MIN(bytesRemaining, fIO_Size);
            while (bytesInBlockRemaining > 0)
            {
                FillBufferWithRandom();
                bytesToWrite = (size_t) MIN(bytesInBlockRemaining, fIO_Size);
                bytesWritten = pwrite(fd_TestVolume, OutputBuffer, bytesToWrite, VolumeOffset);
                
                if(bytesWritten == fIO_Size)
                {
                    EmbedParityCheckDisk(fd_ParityDisk, fd_OutputErrorFile, bytesWritten);
                }
                
                if(bytesWritten != fIO_Size)
                {
                    EmbedParityCheckDisk(fd_ParityDisk, fd_OutputErrorFile, bytesWritten);
                }
                
                if (bytesToWrite == bytesWritten)
                {
                    bytesRemaining -= bytesWritten;
                    bytesInBlockRemaining -= bytesWritten;
                    VolumeOffset += bytesWritten;
                    TotalBytesWritten += bytesWritten;
                }
                else
                {
                    perror("IO ERROR");
                    exit(1);
                }
                
                UpdateProgressBar(TotalBytesWritten, FileSize);
            }
        }
        
        if(FileSize == TotalBytesWritten)
        {
            runSuccess = true;
        }
        else
        {
            cerr << "Internal Error --  Must Debug" << endl;
        }
    }
}

//    ====================================================================================
//    EmbedParityCheck()
//    ====================================================================================
void CorruptionTool::EmbedParityCheckDisk(int parityFileDescriptor, int outputFileDescriptor, uint64_t BytesJustWritten)
{
    uint32_t i = 0;
    
    uint64_t *Aptr;
    uint64_t *Bptr;
    uint64_t *Cptr;
    
    uint64_t count = 0;
    uint64_t BytesRead = 0;
    uint64_t parityDiskOffset = 0;
    uint64_t TempVolumeOffset = VolumeOffset;
    uint64_t ConstStripeArraySize;
    uint64_t bytesOffsetAdjustment;

    Aptr = OutputBuffer;
    Bptr = OutputBuffer;
    Cptr = OutputBuffer;
    
    ConstStripeArraySize = StripeSize >> 3 ;
    
    // Set Inital Positions in Output Buffer;
    
    Aptr += 0;
    Bptr += StripeUnitSize / 8;
    Cptr += (StripeUnitSize / 8) * 2;
    
    
    if( BytesJustWritten % StripeSize == 0)
    {
        count = BytesJustWritten / StripeSize;
        
        parityDiskOffset = TempVolumeOffset/3;
        
        for(i = 0; i < count; i++)
        {
            if( i > 0 )
            {
                // Makes sure that the first time the loop is called the pointers position does not change
                Aptr += ConstStripeArraySize;
                Bptr += ConstStripeArraySize;
                Cptr += ConstStripeArraySize;
            }
            
            BytesRead = pread(parityFileDescriptor, ParityDriverCalc, StripeUnitSize, parityDiskOffset + (i * StripeUnitSize) );
            TotalBytesRead += BytesRead;
            
            ParityComparisonAndCheckDisk(Aptr, Bptr, Cptr, VolumeOffset, outputFileDescriptor);
        }
    }
    
    else if (BytesJustWritten % StripeSize != 0)
    {
        bytesOffsetAdjustment = BytesJustWritten % StripeSize;
        
        if(bytesOffsetAdjustment == StripeUnitSize)
        {
            count++;
        }
        else if(bytesOffsetAdjustment == (StripeUnitSize * 2))
        {
            count += 2;
        }
        
        BytesJustWritten = BytesJustWritten - bytesOffsetAdjustment;
        count += BytesJustWritten / StripeSize;
        parityDiskOffset = TempVolumeOffset/3;
        
        for(i = 0; i < count; i++)
        {
            if( i > 0 )
            {
                // Preserves first iteration buffer position
                Aptr += ConstStripeArraySize;
                Bptr += ConstStripeArraySize;
                Cptr += ConstStripeArraySize;
            }
            
            BytesRead = pread(parityFileDescriptor, ParityDriverCalc, StripeUnitSize, parityDiskOffset + (i * StripeUnitSize) );
            TotalBytesRead += BytesRead;
            
            ParityComparisonAndCheckDisk(Aptr, Bptr, Cptr, VolumeOffset, outputFileDescriptor);
        }
    }
}

//    ====================================================================================
//    ParityComparisonAndCheck()
//    ====================================================================================
void CorruptionTool::ParityComparisonAndCheckDisk(uint64_t* Aptr, uint64_t* Bptr, uint64_t* Cptr, uint64_t CurrentParityOffset, int outputFileDescriptor)
{
    uint64_t index;
    uint64_t count;
    uint64_t ParityDiskOffset;
    
    const char * errorText =
        "****Parity mismatch at disk offset: %lu \n"
        //"****Parity mismatch at buffer offset:      %lu\n"
        "\tSR Parity Value: %lu\n"
        "\tTestBench Parity Value: %lu\n\n";
    
    count = StripeUnitSize >> 3;
    
    //Calculates current buffers parity data.
    for(index = 0; index < count; index++)
    {
        ParitySelfCalc[index] = Aptr[index] ^ Bptr[index] ^ Cptr[index];
    }

    //Compare current buffer block and parity data from disk to determine corruption.
    for(index = 0; index < count; index++)
    {
        if (ParitySelfCalc[index] != ParityDriverCalc[index])
        {
            ParityDiskOffset = ((index * 8) + (CurrentParityOffset/3));
            
            dprintf(outputFileDescriptor, errorText,
                    (unsigned long) ParityDiskOffset,
                    //(unsigned long) &ParitySelfCalc[index],
                    (unsigned long) ParityDriverCalc[index],
                    (unsigned long) ParitySelfCalc[index]);
            
            ParityCalcErrorCounter++;
        }
    }
}


//    ====================================================================================
//    DisplayRunStatsDisk()
//    ====================================================================================
void CorruptionTool::DisplayRunStatsDisk()
{
    if (runSuccess)
    {
            cout << "\n   DISK OUTPUT COMPLETION STATS    " << endl;
            cout << "Specified Buffer Size: " << BufferSize << endl;
            cout << "Specified File IO Size: " << fIO_Size << endl;
            cout << "Specified Volume Offset: " << originalOffset << endl;
            cout << "Total Random Data Written: " << TotalBytesWritten << endl;
            cout << "Total Parity Data Read: " << TotalBytesRead << endl;
            cout << "Total parity Data mismatch errors: " << ParityCalcErrorCounter << endl;
    }
    else
    {
        cout << "\n **** TEST FAILED!**** \nPlease review CMD Line Inputs.\n" << endl;
    }
}



#pragma mark   FILE SYSTEM FUNCTIONS


//    ====================================================================================
//    WriteDataToDisk() --> In Progress
//    ====================================================================================x
void CorruptionTool::WriteDataToFile()
{
    cout << "Running File Write..." << endl;
    
    int fd_TestVolume;
    int fd_ParityDisk;
    int fd_OutputErrorFile;

    ssize_t bytesToWrite = 0;
    ssize_t bytesWritten = 0;
    uint64_t bytesRemaining = 0;
    uint64_t bytesInBlockRemaining = 0;
    
    bytesRemaining = FileSize;
    
    fd_ParityDisk = open(ParityDiskPath, O_RDONLY);
    fd_TestVolume = open(VolumeDiskPath, O_RDWR | O_CREAT);
    fd_OutputErrorFile = open(OutputFilePath, O_RDWR | O_CREAT | O_TRUNC);
    
    chmod(OutputFilePath, S_IRGRP | S_IROTH | S_IRUSR  | S_IWUSR);  // change file permissions for output and error log files
    chmod(VolumeDiskPath, S_IRGRP | S_IROTH | S_IRUSR  | S_IWUSR);

    if ((fd_TestVolume < 0) || (fd_ParityDisk < 0) || (fd_OutputErrorFile < 0))
    {
        cerr << "\n\n#### Error: Unable to open the disk or file at: "
        << VolumeDiskPath << ", " << ParityDiskPath << " and " << OutputFilePath << ".\n"
        << "Do you have mounted volumes on this disk?  (If so unmount them.)\n"
        << "Are you running as root? (If not, sudo this command.)\n"
        << "Do you have write access to this directory if you are creating a "
        << "file?\n"
        << "Are you using the exists flag when the file doesn't exist?\n";
        
        runSuccess = false;
    }
    else
    {
        while (bytesRemaining > 0)
        {
            bytesInBlockRemaining = MIN(bytesRemaining, fIO_Size);
            while (bytesInBlockRemaining > 0)
            {
                FillBufferWithRandom();
                bytesToWrite = (size_t) MIN(bytesInBlockRemaining, fIO_Size);
                bytesWritten = pwrite(fd_TestVolume, OutputBuffer, bytesToWrite, VolumeOffset);
                
                /*
                Once I know how to access the correct file system location for parity info enable
                if(bytesWritten == fIO_Size)
                {
                    EmbedParityCheck(fd_ParityDisk, fd_OutputErrorFile, bytesWritten);
                }

                if(bytesWritten != fIO_Size)
                {
                    EmbedParityCheck(fd_ParityDisk, fd_OutputErrorFile, bytesWritten);
                }
                */
                
                if (bytesToWrite == bytesWritten)
                {
                    bytesRemaining -= bytesWritten;
                    bytesInBlockRemaining -= bytesWritten;
                    VolumeOffset += bytesWritten;
                    TotalBytesWritten += bytesWritten;
                }
                else
                {
                    perror("IO ERROR");
                    exit(1);
                }
                
                UpdateProgressBar(TotalBytesWritten, FileSize);
            }
        }
        
        if(FileSize == TotalBytesWritten)
        {
            runSuccess = true;
        }
        else
        {
            cerr << "Internal Error --  Must Debug" << endl;
        }
    }
}

//    ====================================================================================
//    DisplayRunStatsFile()
//    ====================================================================================
void CorruptionTool::DisplayRunStatsFile()
{
    if (runSuccess)
    {
        cout << "\n   File OUTPUT COMPLETION STATS    " << endl;
        cout << "Specified Buffer Size: " << BufferSize << endl;
        cout << "Specified File IO Size: " << fIO_Size << endl;
        cout << "Specified Volume Offset: " << originalOffset << endl;
        cout << "Total Random Data Written: " << TotalBytesWritten << endl;
        cout << "ignore -- Total Parity Data Read: " << TotalBytesRead << endl;
        cout << "ignore -- notworkingTotal parity Data mismatch errors: " << ParityCalcErrorCounter << endl;
    }
    else
    {
        cout << "\n **** TEST FAILED!**** \nPlease review CMD Line Inputs.\n" << endl;
    }
}



#pragma mark   HELPER FUNCTIONS


//    ====================================================================================
//    ShowHelp()
//    ====================================================================================
void CorruptionTool::ShowHelp()
{
    std::cout << "******************HELPME******************\n\n"
        << "### Parity Corruption Testing Tool:\n"
        << "    'disk' or 'file'        # specify output to file system or disk\n"
        << "    FileSize value          # specify the number of bytes to fill\n"
        << "                            # if not specified, fill 49152 bytes\n"
        << "    offset value            # number of bytes to skip before starting to "
        << "fill\n\n"
        << "    random                  # fill the entire disk with random data\n"
        << "    iosize value            # size of each transfer\n"
        << "    pathname                # bsd pathame to the disk or file, e.g: "
        << "/dev/rdisk1 or disk1s2\n\n"
        << "*Notes: The file IO size and the buffer size must be the same for this test tool to work.\n "
        << "    All of the value command line inputs must be in bytes and multiples of 16384 Bytes\n\n"
        << "examples:\n\n"
        << "    Outputting to file: \n"
        << "    \tsudo CorruptionTool file 4915200 3221225472 1048576 /dev/rdisk6s3 /output.dmg /errorLog.txt\n\n"
        << "    Outputting to disk: \n"
        << "    \tsudo CorruptionTool disk 4915200 3221225472 1048576 /dev/rdisk6s3 /dev/rdisk8 /errorLog.txt\n";
    
    runSuccess = true;
}

//    ====================================================================================
//    FillBufferWithRandom()
//    ====================================================================================
void CorruptionTool::FillBufferWithRandom()
{
    uint64_t index;
    uint64_t count;
    uint64_t * tempPtr;

    random_device rd;
    default_random_engine gen(rd());
    uniform_int_distribution<uint64_t> dis(0, numeric_limits<uint64_t>::max());
    
    count = BufferSize >> 3;
    tempPtr = (uint64_t *) OutputBuffer;
    
    for (index = 0; index < count; index++)
    {
        // *tempPtr = 0x2121534954525543; --> "Curtis!!"
        *tempPtr = dis(gen);
        tempPtr++;
    }
}

//    ====================================================================================
//    convertTo64()
//    ====================================================================================
uint64_t CorruptionTool::convertTo64(char * string)
{
    char* ptr;
    return strtoull(string, &ptr, 10);
}

//    ====================================================================================
//    UpdateProgress() -- > in progress
//    ====================================================================================
void CorruptionTool::UpdateProgressBar(uint64_t CurrentBytesWritten, uint64_t TotalFileSize)
{
    
    float PercentThrough;
    
    PercentThrough = (((float)CurrentBytesWritten) / ((float)TotalFileSize)) * 100;
    
    cout << (int) PercentThrough << "%";
    cout << "\r" ;
}
