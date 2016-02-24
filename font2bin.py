### This file written 2016 by Oscar Mattsson
###
### For copyright and licensing, see file COPYING

path = "font.png"

from PIL import Image
im = Image.open(path)
pix = im.load()
width, height = im.size
letters = []
for letter in range(0, 26):
    l = []
    for y in range(0, 5):
        for x in range(0, 5):
            l.append(str((((pix[x + letter*6,y][0] + pix[x + letter*6,y][1] + pix[x + letter*6,y][2]) / 3) + 126) / 255))
    letters.append(l);


f = open("icon.txt","w")
f.write("# Move to mipslabdata.c\n")
f.write("const uint8_t const letters[26][25] = {\n")
for letter in range(0, 26):
    f.write("\t{\n")
    for y in range(0, 5):
        s = "\t\t"
        for x in range(0, 5):
            s+= str(letters[letter][x + y*5]) + ", "
        f.write(s + "\n")
    f.write("\t},\n")
f.write("};")
f.write("\n\n# Move to mipslab.h\n")
f.write("/* Declare bitmap array containing letters */\n")
f.write("extern const uint8_t const letters[26][25];")
f.close()
print "Code output to: icon.txt"
