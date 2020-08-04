import sys
import csv
import subprocess
import os, stat


f=open('fvaNotUniqueFileName.csv',"r")
NotUnique=[]
for line in f:
   NotUnique.append(line.strip('\n'))
#print (NotUnique)

fixed = open('fixed.csv', "a")

with open('fvaFile.csv', newline='') as csvfile:
    reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in reader:
        if row[0] in NotUnique:
            print(', '.join(row))
        else:
            fixed.write(','.join(row))
            fixed.write("\n")