#!/usr/local/bin/python3
from ctypes import sizeof
import math
#--------------------------------------------------------------------------------
class Time:
	
	def __init__(self, string):
		split = string.split(":")
		
		if len(split) != 3:
			raise Exception("size of time string is not 3")
		else:
			self.hour = int(split[0])
			self.min = int(split[1])

			self.microseconds = Time.parseSecondsString(split[2])
			
	def parseSecondsString(string) -> int:
		result = 0

		split = string.split('.')

		if len(split) != 3:
			raise Exception("size of second string is not 3")
		else:
			s = ""
			s += split[0] # seconds
			s += split[1] # milli
			s += split[2] # micro
			result = int(s)

		return result

	def total(self):
		a = (self.hour * 60 * 60 * pow(10, 6))
		b = (self.min * 60 * pow(10, 6))
		result = a + b + self.microseconds
		return result

#--------------------------------------------------------------------------------
def getElapsedTime(before, after):
	result = 0

	a = Time(after)
	b = Time(before)

	# print("After: " + str(a.total()))
	# print("Before: " + str(b.total()))

	result = a.total() - b.total()

	return result

# Main Method
#--------------------------------------------------------------------------------
print("Opening Files...")
fI = open ("/Users/owc/Desktop/SpeedTestFiles/SpeedInput.txt","r") # need to pass file is as parameter to command line call
fileLength = len(fI.readlines())
print("File length: " + str(fileLength))
 
fO = open("/Users/owc/Desktop/SpeedTestFiles/SpeedOutput.txt","a")

keyword1 =  "started"
keyword2 =  "Copying"
keyword3 =  "Verifying"
keyword4 =  "Archiving"
keyword5 =  "Cleaning"
keyword6 =  "finished"
keyword7 = "Thumbnails"
keyword8 = "Exporting"

fO.write("\n--------------------------     Log Entry    ----------------------\n")

# fO.write("\n------------------------ Table of Contents ------------------------\n\n")
# fI.seek(0) #setting file handeler to the beginning of the file 
# storedIds = [""]
# count = 0 # counter for while loop to break out 
# IDnum = 1 # current number in the ToC (Table of Contents)
# list = [] # initialization of list of used names 
# while True:
#     s = fI.readline()
#     L = s.split()
#     fO.write(IDnum + ". " + L[8])
#     count +=1
#     list.append(L[8]) # adding the current JobID to the list of used Ids
#     for element in list:
#         if # element is in the list go to next line 
#         if len(list) == # legnth of list is the same as the size of the file break out of the loop 

count = 0
fI.seek(0) #setting file handeler to the beginning of the file 
fO.write("\n------------------------------------------------------------------\n\n")
while count < fileLength:
            s = fI.readline()
            L = s.split()

            currTotalTime = 0

            if keyword1 in L: #started
                start  = L[2]
                n = fI.readline()
                Z  = n.split()
                finish = Z[2]
                L = Z
                time = getElapsedTime(start,finish)
                initHeader =  "Job ID: " + Z[8]+ "   \n \nJob Prep Duration: "
                fO.write(initHeader)
                output = str(time)+ "\n"
                fO.write(output)
                currTotalTime += time
                fO.write("---------------------------------------------------------|\n")

            if keyword2 in L: #Copying
                start  = L[2]
                fI.readline()
                fI.readline()
                n = fI.readline()
                Z  = n.split()
                L = Z
                finish = Z[2]
                time = getElapsedTime(start,finish)
                fO.write("Copying Duration: ")
                output = str(time)+ "\n"
                fO.write(output)
                currTotalTime += time
                fO.write("---------------------------------------------------------|\n")
            
            if keyword3 in L: #Verifying
                start  = L[2]
                n = fI.readline()
                Z  = n.split()
                finish = Z[2]
                L = Z
                time = getElapsedTime(start,finish) 
                fO.write("Verifying Duration: ")
                output = str(time)+ "\n"
                fO.write(output)
                currTotalTime += time
                fO.write("---------------------------------------------------------|\n")

            if keyword7 in L: #Thumbnails 
                start  = L[2]
                n = fI.readline()
                Z  = n.split()
                finish = Z[2]
                L = Z
                time = getElapsedTime(start,finish) 
                fO.write("Generating Thumbnails Duration: ")
                output = str(time)+ "\n"
                fO.write(output)
                currTotalTime += time
                fO.write("---------------------------------------------------------|\n")

            if keyword4 in L: #Archiving
                start  = L[2]
                n = fI.readline()
                Z  = n.split()
                finish = Z[2]
                L = Z
                time = getElapsedTime(start,finish) 
                fO.write("Archiving Duration: ")
                output = str(time)+ "\n"
                fO.write(output)
                currTotalTime += time
                fO.write("---------------------------------------------------------|\n")

            if keyword8 in L: #Exporting
                start  = L[2]
                n = fI.readline()
                Z  = n.split()
                finish = Z[2]
                L = Z
                time = getElapsedTime(start,finish) 
                fO.write("Exporting Reports Duration: ")
                output = str(time)+ "\n"
                fO.write(output)
                currTotalTime += time
                fO.write("---------------------------------------------------------|\n")
                
            if keyword5 in L: #Cleaning
                start  = L[2]
                n = fI.readline()
                Z  = n.split()
                finish = Z[2]
                L = Z
                time = getElapsedTime(start,finish)
                fO.write("Job Clean Up Duration: ")
                output = str(time)+ "\n"
                fO.write(output)
                currTotalTime += time
                fO.write("---------------------------------------------------------|\n")

            if currTotalTime > 0:
                fO.write("Total Job Time: ")
                fO.write(str(currTotalTime)+"\n")
                fO.write("---------------------------------------------------------|\n\n\n")
            count = count + 1
#--------------------------------------------------------------------------------
print("Closing Files...")
fI.close()  # close the file at the end 
fO.close()  # close the file at the end 

#--------------------------------------------------------------------------------

print("Report Completed\n")

# path for the copy that log cd Containers/com.owc.CopyThat/Data/file:/Users/owc/Library/Containers/com.owc.CopyThat/Data/CopyThat