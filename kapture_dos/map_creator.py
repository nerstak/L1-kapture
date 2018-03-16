import os
from PIL import Image

im=Image.open("nb.png","r")
pix=im.load()
column,line=im.size
big_array=[[] for b in range(line)]


for i in range(line):
    for j in range(column):
        value=pix[j,i]
        if value==(0,255,0):
            big_array[i].append("grass")
        elif value==(150,75,0):
            big_array[i].append("tree")
        elif value==(0,0,255):
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
