import os

data = open("instructions", "r").readlines()

def hexpad(n, l, c):
    h = hex(n)[2:]
    while (len(h)<l):
        h=c+h
    return h

i=-4
illn=1
l=""
for line in data:
    l=line.replace("\n","")
    if (line == "ill\n"):
        l = "ill"+str(illn)
        illn += 1
    i = i + 4
    print("0x"+hexpad(i,2,'0'), l)
    print("0x"+hexpad(i+1,2,'0'), l+"c")
    print("0x"+hexpad(i+2,2,'0'), l+"i")
    print("0x"+hexpad(i+3,2,'0'), l+"ic")
