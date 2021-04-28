# -*- coding: utf-8 -*-

# python updatePlaceIdForFolder.py "C:\FVANOGEO\283" 283 >> placeId283.csv

import sys
import csv
import subprocess
import os, stat
import win32com.client

newName = "../../#data#/fvaFile.csv_bef_pid_upd_to_" + sys.argv[2] 
os.rename("../../#data#/fvaFile.csv", newName)
f = open("../../#data#/fvafile.csv", "a" , newline='\n', encoding='utf-8')

with open(newName, newline='', encoding='utf-8') as csvfile:
    reader = csv.reader(csvfile, delimiter=',', quotechar='|')         
    for row in reader:
       for filename in os.listdir(sys.argv[1]):
           if row[1].upper() == filename.upper():
              idBefore = row[2] 
              row[2] = sys.argv[2]
              fullFileName = sys.argv[1] + "/" + filename 
              with open("../../#data#/fvaPlaces.csv", newline='', encoding='utf-8') as csvfile:
                  placereader = csv.reader(csvfile, delimiter=',', quotechar='|')         
                  for placerow in placereader:
                     if placerow[0] == sys.argv[2]:                                                                                                 
                        os.chmod(fullFileName, stat.S_IWRITE) # clear read only file attribute
                        print(fullFileName + ',' + idBefore + ',' + row[2] + ',' + placerow[4] + ',' + placerow[5])
                        p = subprocess.call(['../exiftool(-k).exe','-GPSLatitude=' + placerow[4], '-GPSLongitude=' + placerow[5],'-GPSLatitudeRef=' + placerow[6],'-GPSLongitudeRef=' + placerow[7],'-GPSAltitude=0 -GPSAltitudeRef=0', fullFileName])
                        shell = win32com.client.Dispatch("WScript.Shell")
                        shell.SendKeys("{ENTER}", 0) 
       f.write(','.join(row) + "\n")   
f.close() 