from sys import argv

if argv[1] == '-c':
    l = []
    with open(argv[2]+".txt") as file:
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

    with open(argv[2]+"_corr.txt", "w") as file:
        for i in tempi:
            file.write(str(i)+"\n")
elif argv[1] == '-m':
    #merge file1 with file2
    l = []
    with open(argv[2]+"_corr.txt") as file1:
        a = file1.readline()
        while(a):
            l.append(a)
            a = file1.readline()

    with open(argv[3]+"_corr.txt") as file2:
        a = file2.readline()
        while(a):
            l.append(a)
            a = file2.readline()

    with open(argv[4]+".txt", "w+") as file_t:
        for i in l:
            file_t.write(i)
