data = open("instructions", "r").readlines()

def hexpad(n, l, c):
    h = hex(n)[2:]
    while (len(h)<l):
        h=c+h
    return h

i=-4
for line in data:
    i = i + 4
    print("0x"+hexpad(i,2,'0'), line[:-1])
    print("0x"+hexpad(i+1,2,'0'), line[:-1]+"c")
    print("0x"+hexpad(i+2,2,'0'), line[:-1]+"i")
    print("0x"+hexpad(i+3,2,'0'), line[:-1]+"ic")
