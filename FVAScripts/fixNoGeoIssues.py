# -*- coding: utf-8 -*-
import sys
import csv
import subprocess
import os, stat
import win32com.client

#python fixNoGeoIssues.py 1 >> fixNoGeoIssuesPlaceID-1.txt 
#../exiftool(-k).exe -all= -tagsfromfile @ -all:all -unsafe -icc_profile C:/FVA/2004/2004.06.05-07/2004-06-05-##-##-00.jpg
#../exiftool(-k).exe -GPSAltitude='0.0' -GPSLatitudeRef=N -GPSLongitudeRef=E -GPSAltitudeRef=0 "C:\FVA\2001\2001.08.20\2001-08-20-##-##-02.JPG"  

#C:\FVANOGEO\Oksana\2007.08.22-30\219/2007-08-25-13-07-59.jpg,14,219,44.57113903709,34.3399139274

with open("issues3.csv", newline='', encoding='utf-8') as csvfile:
    reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in reader:
        if  row[0] == 'FVA_ERROR_NO_GEO' and row[2] == sys.argv[1] :
            with open("../../#data#/fvaPlaces.csv", newline='', encoding='utf-8') as csvfile:
                placereader = csv.reader(csvfile, delimiter=',', quotechar='|')         
                for placerow in placereader:
                    if placerow[0] == sys.argv[1]:
                        os.chmod(row[1], stat.S_IWRITE) # clear read only file attribute
                        print(row[1] + ',' + placerow[4] + ',' + placerow[5])
                        p = subprocess.call(['../exiftool(-k).exe','-GPSLatitude=' + placerow[4], '-GPSLongitude=' + placerow[5],'-GPSLatitudeRef=' + placerow[6],'-GPSLongitudeRef=' + placerow[7],'-GPSAltitude=0 -GPSAltitudeRef=0', row[1]])
                        shell = win32com.client.Dispatch("WScript.Shell")
                        shell.SendKeys("{ENTER}", 0) 
