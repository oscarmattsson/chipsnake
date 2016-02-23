### This file written 2016 by Oscar Mattsson
###
### For copyright and licensing, see file COPYING
def getByte(x):
    byte = ""
    for y in range(7, -1, -1):
        byte += str((((pix[x,y + 8*i][0] + pix[x,y + 8*i][0] + pix[x,y + 8*i][0]) / 3) + 126) / 255)
    return int(byte, 2)

path = raw_input("Image path: ")
name = path.split("/")[-1].split(".")[0]

from PIL import Image
im = Image.open(path)
pix = im.load()
width, height = im.size
icon = []
for i in range(0, 4):
    for x in range(0, width):
        icon.append(getByte(x))

f = open("icon.txt","w")
f.write("# Move to mipslabdata.c\n")
f.write("const uint8_t const " + name + "[] = {\n")
for y in range(0, width / 2):
    s = "\t"
    for x in range(0, 8):
        s+= str(icon[x + y*8]) + ", "
    f.write(s + "\n")
f.write("};")
f.write("\n\n# Move to mipslab.h\n")
f.write("/* Declare bitmap array containing " + name + " */\n")
f.write("extern const uint8_t const " + name + "[128];")
f.close()
print "Code output to: icon.txt"
