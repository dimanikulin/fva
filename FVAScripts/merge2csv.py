import sys
import fileinput

mainF = open(sys.argv[1], "a")
f = open(sys.argv[2], "r") 
mainF.write("\n")
mainF.write(f.read())