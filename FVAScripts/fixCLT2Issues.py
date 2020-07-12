import sys
import csv
import subprocess
import os, stat

#fixCLT2Issues.py issues.csv FVA_ERROR_MISMATCH_TAKEN_TIME > fixedMismatchTakenTime
#fixCLT2Issues.py issues.csv FVA_ERROR_NULL_TAKEN_TIME > fixedEmptyTakenTime

with open(sys.argv[1], newline='') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in spamreader:
        if row[0] == sys.argv[2] and row[0] == 'FVA_ERROR_MISMATCH_TAKEN_TIME':
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
                tsParameter[12] = '0'
            if tsParameter[14] == '#':
                tsParameter[14] = '0'
            if tsParameter[15] == '#':
                tsParameter[15] = '0'  

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
                tsParameter[12] = '0'
            if tsParameter[14] == '#':
                tsParameter[14] = '0'
            if tsParameter[15] == '#':
                tsParameter[15] = '0'  
  
            #print ('-ts' + ''.join(tsParameter))
            os.chmod(row[1], stat.S_IWRITE) # clear read only file attribute
            subprocess.call(['../jhead.exe', '-mkexif', row[1]])
            subprocess.call(['../jhead.exe', '-ts' + ''.join(tsParameter) , row[1]])
