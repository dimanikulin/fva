import csv
import os, stat

# we save previous file with other file name
newName =  + "_n"
os.rename(sys.argv[1] , newName)

# file to write updated records
f = open(sys.argv[1], "a" , newline='\n', encoding='utf-8')

# we read previous file content
with open(newName, newline='', encoding='utf-8') as csvfile:
    reader = csv.reader(csvfile, delimiter=',', quotechar='|')

    # we read each line there         
    for row in reader:

       # we process each file in input dir we work with
       for filename in os.listdir(sys.argv[2]):

           # if file name in fvaFile.csv equals to current file name in input dir
           if row[1].upper() == filename.upper(): 
              row[8] = sys.argv[3] #update with new EventID
              row[9] = sys.argv[4] #update with new people ids
              
       # write updated record to new fva csv file  
       f.write(','.join(row) + "\n")   
f.close() 
