from exif import Image
import os, stat
import sys
import datetime

for filename in os.listdir(sys.argv[1]):
    fullPath = sys.argv[1] + "/" + filename
    fiximage = Image(fullPath)
    if fiximage.has_exif:
        print(filename + " is good")
        print(f"{fiximage.datetime_original}\n")
    else:
        print(filename + " is NOT good")
        with open(fullPath, "rb") as fixfile:
            time = datetime.datetime.fromtimestamp(os.path.getmtime(fullPath))
            print(time)
            fiximage.datetime_original = str(time)
            fiximage.subsec_time_original = '000'

        os.chmod(fullPath, stat.S_IWRITE) # clear read only file attribute
        with open(fullPath, 'wb') as fixedfile:   
           fixedfile.write(fiximage.get_file())

  
