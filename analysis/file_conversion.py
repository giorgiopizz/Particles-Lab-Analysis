from sys import argv
import os

def convert(filename):
        l = []
        with open(filename+".txt") as file:
            a = file.readline()
            while(a):
                l.append(a)
                a = file.readline()

        tempi = []
        i = 0
        #print(l)
        while i < len(l):
            if l[i]=="###\n":
                #i skip this part, it's just for the conversion program
                i+=2
            else:
                #print(l[i])
                try:

                    n = int(l[i])*4/1000
                    tempi.append(n)
                except:
                    pass

            i+=1

        with open(filename+"_corr.txt", "w") as file:
            for i in tempi:
                file.write(str(i)+"\n")

def merge(filename1, filename2, filenameTot):
    #merge file1 with file2
    l = []
    with open(filename1+".txt") as file1:
        a = file1.readline()
        while(a):
            l.append(a)
            a = file1.readline()

    with open(filename2+".txt") as file2:
        a = file2.readline()
        while(a):
            l.append(a)
            a = file2.readline()

    with open(filenameTot+".txt", "w+") as file_t:
        for i in l:
            file_t.write(i)
if len(argv)>1:
    if argv[1] == '-c':

        convert(argv[2])

    elif argv[1] == '-m':

        merge(argv[2], argv[3], argv[4])

else:
    # conversion of big initial analysis
    s = "tempi_cerbero_sopra_iniziali"
    prefix = "tempi/"
    suffixes = ['up', 'down', 'tot']
    for suffix in suffixes:
        for i in range(3):
            if i==0:
                first_file = prefix + s + "_CORR_" + suffix
            else:
                first_file = "tmp_" + str(i)
            second_file = prefix + s + "_" + str(i+2) + "_CORR_" + suffix
            #merge(first_file, second_file, "tmp"+str(i+1))
            merge(first_file, second_file, "tmp_"+str(i+1))
            if i==2:
                os.rename(os.path.join(os.getcwd(),"tmp_"+str(i+1)+".txt"), os.path.join(os.getcwd(), 'tempi', s + "_merged_" + suffix + ".txt") )
