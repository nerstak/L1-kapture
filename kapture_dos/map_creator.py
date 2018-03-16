import os
from PIL import Image

im=Image.open("nb.png","r")
pix=im.load()
column,line=im.size
big_array=[[] for b in range(line)]


for i in range(line):
    for j in range(column):
        value=pix[j,i]
        if value==(0,127,14):
            big_array[i].append("grass")
        elif value==(127,51,0):
            big_array[i].append("tree")
        elif value==(0,38,255):
            big_array[i].append("water")
        else:
            print("Error at "+str(i)+" "+str(j))

im.close()

name_file=str(input("Give the name of the file you want to create: "))
fil = open(name_file+".txt","w")
fil.write("COLUMN="+str(column)+",LINE="+str(line)+"\n")

for i in range(line):
    for j in range(column):
        fil.write(big_array[i][j]+",")
    fil.write("\n")
print("Done.")

fil.close()



""" for i in range(int(line)):
    redo=1
    while redo:
        redo=0
        print("\n"+" "*56,end='')
        print("1,2,3,4,5,6,7,8,9,10       ,15       ,20       ,25       ,30       ,35       ,40       ,45       ,50       ,45       ,50")
        for j in range(i):
            print(" "*56,end='')
            for k in big_array[j+1]:
                print(k+",",end='')
            print()
        new_line_str=str(input("Enter the '"+str(i)+"' line (with g/w/t and separated by comma): "))
        new_line=new_line_str.split(",")
        if len(new_line)!=int(column):
            redo=1
            print("Incrorrect number of columns")
        for k in new_line:
            if k not in ['g','w','t']:
                redo=1
                print("Incorrect number")
    big_array.append(new_line)
    for k in new_line:
        if k=='g':
            file.write("grass,")
        elif k=='w':
            file.write("water,")
        elif k=='t':
            file.write("tree,")
    file.write("\n") """

