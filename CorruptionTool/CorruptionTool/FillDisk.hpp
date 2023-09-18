//
//  FillDisk.hpp
//  CorruptionTool
//
//  Created by Curtis Robinson on 8/9/23.
//  OWC

#ifndef FillDisk_hpp
#define FillDisk_hpp

//    ====================================================================================
//
//    ====================================================================================
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <random>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <CoreServices/CoreServices.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/storage/IOMedia.h>
#include <IOKit/IOBSD.h>
#include <mach/mach_port.h>

using namespace std;
#define ASCII_ESC 27

//    ====================================================================================
//    TFillDiskTool Class
//    ====================================================================================
class CorruptionTool
{
        private:
            char * ParityDiskPath;
            char * VolumeDiskPath;
            char * OutputFilePath;
            bool fIsDisk;
            char fPathname[PATH_MAX];
             
            uint64_t fSize;
            uint64_t VolumeOffset;
            uint64_t FileSize;
            uint64_t fIO_Size;
            uint64_t fSkipSize;
            uint64_t BufferSize;
            uint64_t fFillNumber;
            uint64_t originalOffset;
            uint32_t ParityCalcErrorCounter;
            
        public:
            bool fileORdisk = 0;
            bool helpFlag;
            bool runFlag;
            bool runSuccess;
            void UpdateArgs(int argcs, char * argvs[]);
            void InitTool();
            void DoWork();
            void ShowHelp();
            void DenitTool();
            void DisplayRunStatsFile();
            void DisplayRunStatsDisk();
            void WriteDataToFile();
            void WriteDataToDisk();
            void EmbedParityCheckDisk(int parityFileDescriptor, int outputFileDescriptor, uint64_t BytesJustWritten);
            
        private:
            void ConvertUnits(const char * value);
            uint64_t convertTo64(char * string);
            void FillBufferWithRandom();
            void UpdateProgressBar(uint64_t CurrentBytesWritten, uint64_t TotalFileSize);
            void ParityComparisonAndCheckDisk(uint64_t* Aptr, uint64_t* Bptr, uint64_t* Cptr, uint64_t CurrentParityOffset, int outputFileDescriptor);
    };

#endif /* FillDisk_hpp */
