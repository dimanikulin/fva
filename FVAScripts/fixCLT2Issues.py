import sys
import csv
import subprocess
import os, stat

#fixCLT2Issues.py issues.csv FVA_ERROR_MISMATCH_TAKEN_TIME > fixedTakenTime

with open(sys.argv[1], newline='') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in spamreader:
        if row[0] == sys.argv[2]: #FVA_ERROR_MISMATCH_TAKEN_TIME 
            #print(', '.join(row))
            # converting into yyyy:mm:dd-hh:mm:ss
            tsParameter = list(row[2])
            tsParameter[4] = ':'
            tsParameter[7] = ':'
            tsParameter[13] = ':'
            tsParameter[16] = ':'
            #print ('-ts' + ''.join(tsParameter))
            os.chmod(row[1], stat.S_IWRITE) # clear read only file attribute
            subprocess.call(['../jhead.exe', '-ts' + ''.join(tsParameter) , row[1]])