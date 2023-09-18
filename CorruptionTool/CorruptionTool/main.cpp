//
//  main.cpp
//  CorruptionTool
//
//  Created by Curtis Robinson on 8/9/23.
//  OWC

#include <iostream>
#include "FillDisk.hpp"

int main(int argc, char * argv[]) {
    
    CorruptionTool  parityTestTool;
    
    parityTestTool.UpdateArgs(argc, argv);
    
    if(parityTestTool.helpFlag)
    {
        parityTestTool.ShowHelp();
    }
    else if (parityTestTool.runFlag)
    {
        if (!parityTestTool.fileORdisk) //True when outputting to disk
        {
            parityTestTool.InitTool();
            
            parityTestTool.WriteDataToDisk();
            
            parityTestTool. DenitTool();
            
            parityTestTool.DisplayRunStatsDisk();
        }
        else if (parityTestTool.fileORdisk) //True when outputting to file
        {
            parityTestTool.InitTool();
            
            parityTestTool.WriteDataToFile();
            
            parityTestTool. DenitTool();
            
            parityTestTool.DisplayRunStatsFile();
        }
        else
        {
            cerr << "\nInternal Program Error!\n" << endl;
        }
    }
    
    return 0;
}
