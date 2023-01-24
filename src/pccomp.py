#!/usr/bin/env python

import sys

stderr = open("/dev/stderr", "w")
ifp = ofp = None

try:    
    ifp = open(sys.argv[1], "r")
    ofp = open(sys.argv[2], "wb")
except (IndexError):
    stderr.write("Usage: pccomp <source> <output>\n")
    sys.exit(1)
except (FileNotFoundError):
    stderr.write("Error: File "+sys.argv[1]+" and/or "+sys.argv[2]+" is unreadable\n")
    sys.exit(2)

data = ifp.read()

mnemsd = open("instructions_comp", "r").readlines()
mnems={}

for line in mnemsd:
    tokens = line.split()
    mnems[tokens[1]] = int(tokens[0], 16)
defines = {}

labels = {}

section = "code"

valid_sections = ["data","code"]

i=0
lines = data.split("\n")
line = ""
rel = False

while (i<len(lines)):
    line = lines[i]
    tokens = line.replace("  ","\t").replace("\t","").split()
    try:
        if (tokens[0].startswith(".")):
            section = tokens[0][1:-1]
            print("Entering section",section)
        elif (section=="data"):
            if (tokens[0] == "db"):
                if (tokens[2].startswith("0x")): #hex
                    n = int(tokens[2], 16)
                elif (tokens[2].startswith("0b")): #binary
                    n = int(tokens[2], 2)
                elif (tokens[2].startswith("0")): #octal
                    n = int(tokens[2], 8)
                else: #decimal
                    n = int(tokens[2], 10)
                if (n>256):
                    print("Integer overflow warning at line",i+1,"("+str(n),"->",str(n%256)+")","\n\t"+line)
                defines[tokens[1]] = n%256
                print("Defined constant",tokens[1],"as",n%256)

            elif (tokens[0] == "ds"):
                s = ""
                for t in tokens[2:]:
                    s += t.replace("'", "").replace("\"","")+" "
                s = s[:-1]
                defines[tokens[1]] = s
                print("Defined constant",tokens[1],"as",s)
            else:
                print("Syntax error at line",i+1,"(\""+tokens[0]+"\" isn't a valid prefix):\n\t"+line)
        elif (section=="code"):
            mnem = tokens[0];
            if (tokens[0] == "label"):
                labels[tokens[1]] = i
            elif (tokens[0].startswith("$")):
                try:
                    if (type(defines[tokens[0][1:]]) == str):
                        ofp.write(bytes(str.encode(defines[tokens[0][1:]],encoding="ascii")))
                    else:
                        ofp.write(bytes([defines[tokens[0][1:]]]))
                except (KeyError):
                    print("Undefined constant at line",i+1,"\b:\n\t"+line)
                    sys.exit(127)
            else:
                ofp.write(bytes([mnems[mnem]]))
                for byte in tokens[1:]:
                    if (byte.startswith("$")):
                        try:
                            if (type(defines[byte[1:]]) == str):
                                ofp.write(bytes(str.encode(defines[byte[1:]],encoding="ascii")))
                            else:
                                ofp.write(bytes([defines[byte[1:]]]))
                        except (KeyError):
                            print("Undefined constant at line",i+1,"\b:\n\t"+line)
                            sys.exit(127)
                    elif (byte.startswith("%")):
                        n = labels[byte[1:]]
                        w = False
                        if (rel):
                            n = (n - i) + 127
                            ofp.write(bytes([n%256]))
                        else:
                            ofp.write(bytes([n/256,n%256]))
                    elif (byte == "rel"):
                        rel=True
                    else:
                        ofp.write(bytes([int(byte,16)]))
        if (not section in valid_sections):
            print("Invalid section error at line",i+1,"\b:\n\t"+data.split("\n")[i])

    except (IndexError):
        if (len(tokens)<1):
            pass
        else:
            print("Syntax error at line",i+1,"\b:\n\t"+line)
            sys.exit(127)
    i += 1


ifp.close()
ofp.close()
