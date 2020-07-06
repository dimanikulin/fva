import sys
#import fileinput

#for line in fileinput.input(sys.argv[1], inplace=True):
#    if line.contains('TARGET_FOLDER_NAME'):
 #       newline = line.replace('TARGET_FOLDER_NAME', 'NEW_TARGET_FOLDER_NAME')
  #      print('{} {}'.format(fileinput.filelineno(), line), end='') # for Python 3
        # print "%d: %s" % (fileinput.filelineno(), line), # for Python 2

#from tempfile import mkstemp
#from shutil import move, copymode
#from os import fdopen, remove

#def replace(file_path, pattern, subst):
    #Create temp file
 #   fh, abs_path = mkstemp()
  #  with fdopen(fh,'w') as new_file:
   #     with open(file_path) as old_file:
    #        for line in old_file:
     #           new_file.write(line.replace(pattern, subst))
    #Copy the file permissions from the old file to the new file
    #copymode(file_path, abs_path)
    #Remove original file
    #remove(file_path)
    #Move new file
    #move(abs_path, file_path)

import fileinput

with fileinput.FileInput(sys.argv[1], inplace=True, backup='.bak') as file:
    for line in file:
        print(line.replace('TARGET_FOLDER_NAME', sys.argv[2]), end='')
