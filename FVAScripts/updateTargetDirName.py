import sys
import fileinput

with fileinput.FileInput(sys.argv[1], inplace=True, backup='.bak') as file:
    for line in file:
        print(line.replace('TARGET_FOLDER_NAME', sys.argv[2]), end='')
