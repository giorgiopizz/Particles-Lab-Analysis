from sys import argv


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

if argv[1] == '-c':

    convert(argv[2])

elif argv[1] == '-m':

    merge(argv[2], argv[3], argv[4])
