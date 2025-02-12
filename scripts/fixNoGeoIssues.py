# -*- coding: utf-8 -*-
import sys
import csv
import subprocess
import os, stat
import win32com.client

# we open the issues csv
with open("issues3.csv", newline='', encoding='utf-8') as csvfile:
    # we take line by line from there 
    reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in reader:
        # if error is set to FVA_ERROR_NO_GEO and placeid in issues csv equals placeid passed as input param  
        if  row[0] == 'FVA_ERROR_NO_GEO' and row[2] == sys.argv[1] :
            # we go to fvaPlaces.csv 
            with open("../../#data#/fvaPlaces.csv", newline='', encoding='utf-8') as csvfile:
                # we take line by line from there
                placereader = csv.reader(csvfile, delimiter=',', quotechar='|')         
                for placerow in placereader:
                    # if place id from fvaPlaces.csv equals placeid passed as input param  
                    if placerow[0] == sys.argv[1]:
                        os.chmod(row[1], stat.S_IWRITE) # clear read only file attribute
                        print(row[1] + ',' + placerow[4] + ',' + placerow[5]) # save to log
                        # update GPS coordinates in a file in FS using  exiftool(-k).exe 
                        p = subprocess.call(['../exiftool(-k).exe','-GPSLatitude=' + placerow[4], '-GPSLongitude=' + placerow[5],'-GPSLatitudeRef=' + placerow[6],'-GPSLongitudeRef=' + placerow[7],'-GPSAltitude=0 -GPSAltitudeRef=0', row[1]])
                        shell = win32com.client.Dispatch("WScript.Shell")
                        # press Enter to go to next file
                        shell.SendKeys("{ENTER}", 0) 
