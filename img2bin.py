### This file written 2016 by Oscar Mattsson
###
### For copyright and licensing, see file COPYING

path = raw_input("Image path: ")
name = path.split("/")[-1].split(".")[0]

from PIL import Image
im = Image.open(path)
pix = im.load()
width, height = im.size
icon = []
for y in range(0, 32):
    for x in range(0, width):
        icon.append(str((((pix[x,y][0] + pix[x,y][1] + pix[x,y][2]) / 3) + 126) / 255))


f = open("icon.txt","w")
f.write("# Move to mipslabdata.c\n")
f.write("const uint8_t const " + name + "[] = {\n")
for y in range(0, height):
    s = "\t"
    for x in range(0, width):
        s+= str(icon[x + y*width]) + ", "
    f.write(s + "\n")
f.write("};")
f.write("\n\n# Move to mipslab.h\n")
f.write("/* Declare bitmap array containing " + name + " */\n")
f.write("extern const uint8_t const " + name + "[" + str(width*32) + "];")
f.close()
print str(width) + ", " + str(height) + ", Code output to: icon.txt"
