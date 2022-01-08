import sys
import csv
import subprocess
import os, stat
import win32com.client
from exif import Image # to work with Exif information

#function to convert decimal GPS to DMS 
def decdeg2dms(dd):
    # 1 option - it gives exception
    #is_positive = dd >= 0
    #dd = abs(dd)
    #minutes,seconds = divmod(dd*3600,60)
    #degrees,minutes = divmod(minutes,60)
    #degrees = degrees if is_positive else -degrees
    #return (degrees,minutes,seconds)

    # 2 option - it gives exception
    #d = int(Decimal)
    #m = int((Decimal - d) * 60)
    #s = (Decimal - d - m/60) * 3600.00
    #return (d,m,s)

    # it works but it gives different GPS position in a Map comparing to exiftool(-k).exe
    # it was decided to leave only exiftool(-k).exe as tool for setting GPS coordinates to file
    dd1 = abs(float(dd))
    cdeg = int(dd1)
    minsec = dd1 - cdeg
    cmin = int(minsec * 60)
    csec = (minsec % 60) / float(3600)    
    if float(dd) < 0: cdeg = cdeg * -1
    raise Exception ("NOT IMPLEMENTED")
    return cdeg,cmin,csec 

# we save previous file with other file name
newName = "../../#data#/fvaFile.csv_bef_pid_upd_to_" + sys.argv[2] 
os.rename("../../#data#/fvaFile.csv", newName)

# file to write updated records
f = open("../../#data#/fvafile.csv", "a" , newline='\n', encoding='utf-8')

# we read previous file content
with open(newName, newline='', encoding='utf-8') as csvfile:
    reader = csv.reader(csvfile, delimiter=',', quotechar='|')

    # we read each line there         
    for row in reader:

       # we process each file in  inpur dir we work with
       for filename in os.listdir(sys.argv[1]):

           # if file name in fvaFile.csv equals to current file name in input dir
           if row[1].upper() == filename.upper(): 
              idBefore = row[2] # save previos PlaceID value for history
              row[2] = sys.argv[2] #update with new PlaceID
              fullFileName = sys.argv[1] + "/" + filename

              # we go to fvaPlaces.csv 
              with open("../../#data#/fvaPlaces.csv", newline='', encoding='utf-8') as csvfile:
                  placereader = csv.reader(csvfile, delimiter=',', quotechar='|')         
                  for placerow in placereader: # we take line by line from there

                     # if place id from fvaPlaces.csv equals placeid passed as input param
                     if placerow[0] == sys.argv[2]:                                                                                                 
                        os.chmod(fullFileName, stat.S_IWRITE) # clear read only file attribute
                        print(fullFileName + ',' + idBefore + ',' + row[2] + ',' + placerow[4] + ',' + placerow[5]) # save to log
                        try: 
                            # update GPS coordinates in a memory
                            raise Exception ("NOT IMPLEMENTED") # because it does not work
                            fiximage = Image(fullFileName)
                            fiximage.gps_latitude      = decdeg2dms(placerow[4]) # TODO - to fix rounding issue
                            fiximage.gps_longitude     = decdeg2dms(placerow[5]) # TODO - to fix rounding issue
                            fiximage.gps_latitude_ref  = placerow[6]
                            fiximage.gps_longitude_ref = placerow[7]
                            fiximage.gps_altitude      = 0
                            fiximage.gps_altitude_ref  = 0

                            # update GPS coordinates in a file in FS from memory
                            with open(fullFileName, 'wb') as fixedfile:   
                                fixedfile.write(fiximage.get_file())

                        # we got exeption from python exif lib, lets try to do it by exiftool(-k).exe 
                        except BaseException as error:                           
                            #print('An exception occurred: {}'.format(error))

                            # update GPS coordinates in a file in FS using  exiftool(-k).exe
                            p = subprocess.call(['exiftool(-k).exe','-GPSLatitude=' + placerow[4], '-GPSLongitude=' + placerow[5],'-GPSLatitudeRef=' + placerow[6],'-GPSLongitudeRef=' + placerow[7],'-GPSAltitude=0 -GPSAltitudeRef=0', fullFileName])                            

                            # press Enter to go to next file
                            shell = win32com.client.Dispatch("WScript.Shell")
                            shell.SendKeys("{ENTER}", 0)

       # write updated record to new fva csv file  
       f.write(','.join(row) + "\n")   
f.close() 
