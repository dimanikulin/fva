# -*- coding: utf-8 -*-
import sys
import csv
import subprocess
import os, stat


#fixCLT2Issues.py issues.csv FVA_ERROR_MISMATCH_TAKEN_TIME >> fixedMismatchTakenTime.txt
#fixCLT2Issues.py issues.csv FVA_ERROR_NULL_TAKEN_TIME >> fixedEmptyTakenTime.txt
#fixCLT2Issues.py issues.csv FVA_ERROR_NO_DEV_ID >> fixedNotLinkedDevId.txt
#fixCLT2Issues.py issues.csv FVA_ERROR_EMPTY_DEVICE >> fixedEmptyDevice.txt
#fixCLT2Issues.py issues.csv FVA_ERROR_UNKNOWN_DEVICE >> fixedUnknownDevice.txt
#fixCLT2Issues.py issues.csv FVA_ERROR_LINKED_WRONG_DEVICE >> fixedLinkedWrongDevice.txt

with open(sys.argv[1], newline='', encoding='utf-8') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in spamreader:
        #yield [unicode(cell, 'utf-8') for cell in row]
        if row[0] == sys.argv[2] and row[0] == 'FVA_ERROR_MISMATCH_TAKEN_TIME':
            print(', '.join(row))
            # converting into yyyy:mm:dd-hh:mm:ss
            tsParameter = list(row[2])
            tsParameter[4] = ':'
            tsParameter[7] = ':'
            tsParameter[13] = ':'
            tsParameter[16] = ':'
            if tsParameter[11] == '#':
                tsParameter[11] = '0'
            if tsParameter[12] == '#':
                tsParameter[12] = '1'
            if tsParameter[14] == '#':
                tsParameter[14] = '0'
            if tsParameter[15] == '#':
                tsParameter[15] = '1'  
            if tsParameter[17] == '#':
                tsParameter[17] = '0'
            if tsParameter[18] == '#':
                tsParameter[18] = '1'  

            #print ('-ts' + ''.join(tsParameter))
            os.chmod(row[1], stat.S_IWRITE) # clear read only file attribute
            subprocess.call(['../jhead.exe', '-ts' + ''.join(tsParameter) , row[1]])

        if row[0] == sys.argv[2] and row[0] == 'FVA_ERROR_NULL_TAKEN_TIME':  
            #print(', '.join(row))
            # converting into yyyy:mm:dd-hh:mm:ss
            tsParameter = list(row[2])
            tsParameter[4] = ':'
            tsParameter[7] = ':'
            tsParameter[13] = ':'
            tsParameter[16] = ':'
            if tsParameter[11] == '#':
                tsParameter[11] = '0'
            if tsParameter[12] == '#':
                tsParameter[12] = '1'
            if tsParameter[14] == '#':
                tsParameter[14] = '0'
            if tsParameter[15] == '#':
                tsParameter[15] = '1'  
            if tsParameter[17] == '#':
                tsParameter[17] = '0'
            if tsParameter[18] == '#':
                tsParameter[18] = '1'  
  
            #print ('-ts' + ''.join(tsParameter))
            os.chmod(row[1], stat.S_IWRITE) # clear read only file attribute
            subprocess.call(['../jhead.exe', '-mkexif', row[1]])
            subprocess.call(['../jhead.exe', '-rgt', row[1]])
            subprocess.call(['../jhead.exe', '-ts' + ''.join(tsParameter) , row[1]])

        if row[0] == sys.argv[2] and row[0] == 'FVA_ERROR_NO_DEV_ID':
            print(', '.join(row))

        if row[0] == sys.argv[2] and row[0] == 'FVA_ERROR_EMPTY_DEVICE' and row[2] == '177':
            #print(', '.join(row))
            os.chmod(row[1], stat.S_IWRITE) # clear read only file attribute
            #subprocess.call(['../jhead.exe', "-te", "source.JPG" , row[1]])
            #subprocess.call(['../jhead.exe', '-rgt', row[1]])
            print(row[1])
            subprocess.call(['../exiftool(-k).exe','-model=Era Nano 6','-make=Fly',  row[1]])
        if row[0] == sys.argv[2] and row[0] == 'FVA_ERROR_UNKNOWN_DEVICE' and row[2] == '163':
            #print(', '.join(row))
            os.chmod(row[1], stat.S_IWRITE) # clear read only file attribute
            subprocess.call(['../jhead.exe', "-te", "source.JPG" , row[1]])
            #subprocess.call(['../jhead.exe', '-rgt', row[1]])
            #subprocess.call(['../exiftool(-k).exe','-model=Era Nano 6','-make=Fly',  row[1]])
        if row[0] == sys.argv[2] and row[0] == 'FVA_ERROR_LINKED_WRONG_DEVICE' and row[2] == '3':
            #print(', '.join(row))
            os.chmod(row[1], stat.S_IWRITE) # clear read only file attribute
            subprocess.call(['../jhead.exe', "-te", "source.JPG" , row[1]])
            subprocess.call(['../jhead.exe', '-rgt', row[1]])
