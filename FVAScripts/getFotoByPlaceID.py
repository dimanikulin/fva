# -*- coding: utf-8 -*-
import sys
import csv
import subprocess
import os, stat

with open("issues3.csv", newline='', encoding='utf-8') as csvfile:
    reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in reader:
        if  row[0] == 'FVA_ERROR_NO_GEO' and row[2] == sys.argv[1] :
            with open("../../#data#/fvaPlaces.csv", newline='', encoding='utf-8') as csvfile:
                placereader = csv.reader(csvfile, delimiter=',', quotechar='|')         
                for placerow in placereader:
                    if placerow[0] == sys.argv[1]:
                        print(row[1] + ',' + placerow[4] + ',' + placerow[5])
