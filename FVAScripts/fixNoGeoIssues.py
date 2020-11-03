# -*- coding: utf-8 -*-
import sys
import csv
import subprocess
import os, stat
from subprocess import Popen, PIPE
import win32com.client

#python fixNoGeoIssues.py 1 >> fixNoGeoIssuesPlaceID-1.txt 

#../exiftool(-k).exe -all= -tagsfromfile @ -all:all -unsafe -icc_profile C:/FVA/2004/2004.06.05-07/2004-06-05-##-##-00.jpg

with open("issues.csv", newline='', encoding='utf-8') as csvfile:
    reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in reader:
        if  row[0] == 'FVA_ERROR_NO_GEO' and row[2] == sys.argv[1] :
            with open("../../#data#/fvaPlaces.csv", newline='', encoding='utf-8') as csvfile:
                placereader = csv.reader(csvfile, delimiter=',', quotechar='|')         
                for placerow in placereader:
                    if placerow[0] == sys.argv[1]:
                        os.chmod(row[1], stat.S_IWRITE) # clear read only file attribute
                        print(row[1] + ',' + placerow[5] + ',' + placerow[6] )
                        p = subprocess.call(['../exiftool(-k).exe','-GPSLatitude=' + placerow[5],'-GPSLongitude='+placerow[6], '-q', row[1]])
                        shell = win32com.client.Dispatch("WScript.Shell")
                        shell.SendKeys("{ENTER}", 0) 
