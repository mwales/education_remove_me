#!/usr/bin/env python

import subprocess
import datetime

# All of the data files need to be in this directory and have a .csv extension
# They should all start at the same time (may not be 100% neccessary)
# Need to be in the following format:
#
# 18-12-02 16:27:34, alkaline, Small Load, 2.25, Large Load, 0.00
# 
# Use vi macros to mix up the arduino output to match

def allDataMapsHaveTime(mapFileCollection, timestamp):
    for smap in mapFileCollection:
        if timestamp not in smap:
            return False
    return True

def getDataForTime(mapFile, timestamp):
    #print "getDataForTime called for {}".format(timestamp)
    #jif timestamp in mapFile:
    return mapFile[timestamp]
    
def main():
    dirListing = subprocess.check_output(["ls"])
    fileNameList = dirListing.split()
    #print("Full list of files in directory: {}".format(fileNameList))

    dataFiles = []
    for singleFileName in fileNameList:
        if singleFileName.endswith(".csv"):
            print("Adding file {}".format(singleFileName))
            dataFiles.append(open(singleFileName, 'r'))

    mapCollection = []
    for dataFile in dataFiles:
        mapCollection.append(readFileIntoMap(dataFile))

    startTime = datetime.datetime(18, 11, 23, 14, 52)
    endTime = datetime.datetime(18, 12,10,0,0)

    #print(startTime)
    addMin = datetime.timedelta(minutes=1)
    #print(startTime + addMin)
    #print(startTime - addMin)
    while(startTime < endTime):

        if allDataMapsHaveTime(mapCollection, startTime):
            outputLine = str(startTime)
            outputLine += ","
            for eachMap in mapCollection:
                outputLine += ",".join( getDataForTime(eachMap, startTime))
                outputLine += ","

            print outputLine

        startTime = startTime + addMin

def readFileIntoMap(f):
    retData = {}
    for singleLine in f:
        singleLineParts = singleLine.split()
        #print singleLineParts

        dateParts = singleLineParts[0].split('-')
        timePart = singleLineParts[1].split(':')
        batteryType = singleLineParts[2].replace(',', '')
        smallLoadVolt = singleLineParts[5].replace(',', '')
        largeLoadVolt = singleLineParts[8].replace(',', '')


        combinedTime = datetime.datetime(int(dateParts[0]), int(dateParts[1]), int(dateParts[2]), int(timePart[0]), int(timePart[1]), 0)

        #print combinedTime, batteryType, smallLoadVolt, largeLoadVolt

        retData[combinedTime] = [batteryType, smallLoadVolt, largeLoadVolt]

    return retData

if __name__ == "__main__":
    main()

