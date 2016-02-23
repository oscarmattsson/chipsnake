### This file written 2016 by Oscar Mattsson
###
### For copyright and licensing, see file COPYING

path = "num.png"

from PIL import Image
im = Image.open(path)
pix = im.load()
width, height = im.size
nums = []
for num in range(0, 10):
    n = []
    for y in range(0, 5):
        for x in range(0, 3):
            n.append(str((((pix[x + num*4,y][0] + pix[x + num*4,y][1] + pix[x + num*4,y][2]) / 3) + 126) / 255))
    nums.append(n);


f = open("icon.txt","w")
f.write("# Move to mipslabdata.c\n")
f.write("const uint8_t const numbers[10][15] = {\n")
for num in range(0, 10):
    f.write("\t{\n")
    for y in range(0, 5):
        s = "\t\t"
        for x in range(0, 3):
            s+= str(nums[num][x + y*3]) + ", "
        f.write(s + "\n")
    f.write("\t},\n")
f.write("};")
f.write("\n\n# Move to mipslab.h\n")
f.write("/* Declare bitmap array containing numbers */\n")
f.write("extern const uint8_t const numbers[10][15];")
f.close()
print "Code output to: icon.txt"
