# -*- coding: utf-8 -*-
import sys
import csv
import subprocess
import os, stat
import win32com.client

# we save previous file with other file name
newName = "../../#data#/fvaFile.csv_bef_pid_upd_to_" + sys.argv[2] 
os.rename("../../#data#/fvaFile.csv", newName)

# file to write updated records
f = open("../../#data#/fvafile.csv", "a" , newline='\n', encoding='utf-8')

# we read previous file content
with open(newName, newline='', encoding='utf-8') as csvfile:
    reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    # we read each line there         
    for row in reader:
       # we process each file in  inpur dir we work with
       for filename in os.listdir(sys.argv[1]):
           # if file name in fvaFile.csv equals to current file name in input dir
           if row[1].upper() == filename.upper(): 
              idBefore = row[2] # save previos PlaceID value for history
              row[2] = sys.argv[2] #update with new PlaceID
              fullFileName = sys.argv[1] + "/" + filename
              # we go to fvaPlaces.csv 
              with open("../../#data#/fvaPlaces.csv", newline='', encoding='utf-8') as csvfile:
                  placereader = csv.reader(csvfile, delimiter=',', quotechar='|')         
                  for placerow in placereader: # we take line by line from there
                     # if place id from fvaPlaces.csv equals placeid passed as input param
                     if placerow[0] == sys.argv[2]:                                                                                                 
                        os.chmod(fullFileName, stat.S_IWRITE) # clear read only file attribute
                        print(fullFileName + ',' + idBefore + ',' + row[2] + ',' + placerow[4] + ',' + placerow[5]) # save to log
                        # update GPS coordinates in a file in FS using  exiftool(-k).exe
                        p = subprocess.call(['../exiftool(-k).exe','-GPSLatitude=' + placerow[4], '-GPSLongitude=' + placerow[5],'-GPSLatitudeRef=' + placerow[6],'-GPSLongitudeRef=' + placerow[7],'-GPSAltitude=0 -GPSAltitudeRef=0', fullFileName])
                        shell = win32com.client.Dispatch("WScript.Shell")
                        # press Enter to go to next file
                        shell.SendKeys("{ENTER}", 0)
       # write updated record to new file 
       f.write(','.join(row) + "\n")   
f.close() 
