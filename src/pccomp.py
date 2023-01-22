import sys

stderr = open("/dev/stderr", "w")
ifp = ofp = None

try:    
    ifp = open(sys.argv[1], "r")
    ofp = open(sys.argv[2], "w")
except (IndexError):
    stderr.write("Usage: pccomp <source> <output>\n")
    sys.exit(1)
except (FileNotFoundError):
    stderr.write("Error: File "+sys.argv[1]+" and/or "+sys.argv[2]+" is unreadable\n")
    sys.exit(2)

hexbytes = ifp.read().split()

for byte in hexrytes:
    ofp.write(chr(byte))
ifp.close()
ofp.close()
