import sqlite3
import matplotlib.pyplot as plt
import numpy as np
import DT5751read as dt
from sys import argv
from math import ceil
import multiprocessing

# FUNZIONI
def delta(arr1):
    # derivata
    der = []
    for i in range(1,len(arr1)):
        der.append(arr1[i]-arr1[i-1])
    return np.array(der)


def minmax(derivata):
    condition =(derivata > 200) | (derivata<-200)
    start = ch_max
    stop = 0
    for j in range(len(condition)):
        if condition[j] and j<start:
            start = j
        if condition[j] and j>stop:
            stop = j
    if start>20:
        return [start-20,stop+20]
    else:
        return [start,stop+20]


def sovrapposizione_quadra(signals):
    #signals is a list of np.arrays
    for i in range(2):
        #loop sui canali, controllo se la quadra d
        #(np.min(signals[i])+np.max(signals[i]))/2
        half_square = 1250
        cond_array = signals[i]<half_square
        #the other signal
        a = signals[(i+1)%2][cond_array][signals[(i+1)%2][cond_array]<half_square]
        if len(a)>0:
            return True
        else:
            return False

def pos_impulso(der):
    derivata_attiva = False
    #max_encountered = False
    #m=0
    inizio_impulsi = []
    for i in range(len(der)):
        if der[i] < -200 and not derivata_attiva:
            inizio_impulsi.append(i)
            derivata_attiva=True
            m = -10000
        elif der[i] >-200 and derivata_attiva :
            derivata_attiva = False
        """elif m != 0 and der[i]<m:
            max_encountered = True
        elif max_encountered and der[i]<200 and der[i]>-200:
            max_encountered = False
            derivata_attiva = False
            m=0"""
    return inizio_impulsi


def start_impulso(der):
    # non mi piace la funzione sopra che identifica al posizione dello start dell'impulso
    # qui sfruttando la derivata guardo solo se la derivata supera un certo valore soglia val_th
    val_th = -400
    found = False
    posizioni = []
    # appena diventa true memorizzo la posizione
    for i in range(len(der)):
        if not found and der[i]<val_th:
            posizioni.append(i)
            found = True
        elif found and der[i]>val_th:
            found = False
    #quindi quello che si fa è guardare quando derivata supera il valore soglia, appena lo supera si registra la posizione
    # appena invece si scende sotto la soglia bisogna resettare found
    return posizioni


def doppia(der):
    #start e stop della quadra
    """start = pos_impulso(der)
    stop = pos_impulso(der*(-1))"""
    start = start_impulso(der)
    stop = start_impulso(der*(-1))
    if len(start)!=len(stop):
        # non è una doppia ma va scartata perché ci siamo persi o uno start o uno stop
        return True

    for i in range(len(start)):
        if stop[i]>start[i]:
            #allora è lo stop associato a questo start
            lasso = stop[i]-start[i]
            if i+1<len(start) and start[i+1]-stop[i]<lasso:

                return True
        else:
            #ci siamo persi uno start di un impulso
            return True

            """#lo stop i+1 è associato a start i
            #controllo per essere sicuro
            if stop[i+1]>start[i]:
                lasso = stop[i+1]-start[i]
                if start[i+1]-stop[i+1]<lasso:
                    return True
            else:
                #dò errore
                raise"""
    return False

def save_vec (mode, tempi, j):
    with open("tempi.txt",mode) as file:
        for i in tempi:
            file.write(str(i)+"\n")
        file.write("###\n"+str(j)+"\n***\n")


def check_last_analysis():
    j = -1
    try:
        with open("tempi.txt") as file:
            l = file.readline()

            while l:
                if l=="###\n":
                    l = file.readline()
                    j = int(l)
                l = file.readline()
    except:
        pass
    return j

def convert_samples(sample):
    l = []
    for i in sample.split(b' '):
        l.append(int(i.decode("utf-8")))
    return l
def check_signals(signals,tempi):
    if not sovrapposizione_quadra(signals):

        derivata1 = delta(signals[0])
        derivata2 = delta(signals[1])
        interval = [min(minmax(derivata1)[0],minmax(derivata2)[0]),max(minmax(derivata1)[1],minmax(derivata2)[1])]
        #interval = [minmax(derivata1)[0],minmax(derivata1)[1]]
        #ulteriore controllo per vedere se ci sono esattamente un impulso di start ed uno di stop (tot = 2)
        impulsi1 = pos_impulso(derivata1)
        impulsi2 = pos_impulso(derivata2)
        #print('impulsi: '+str(i))
        #print(impulsi1)
        #print(impulsi2)
        #print('doppia: '+str(i))
        #print(doppia(derivata1))
        #print(doppia(derivata2))

        if (len(impulsi1) + len(impulsi2)) == 2:
            if not (doppia(derivata1) or doppia(derivata2)):
                # fig1, ax1 = plt.subplots(1, 1, tight_layout=True)
                # ax1.plot(np.arange(ch_max)[interval[0]:interval[1]],signals[0][interval[0]:interval[1]],label='Ch0')
                # ax1.plot(np.arange(ch_max)[interval[0]:interval[1]],signals[1][interval[0]:interval[1]], label='Ch1')

                if len(impulsi1) == 2:
                    time_diff = impulsi1[1]-impulsi1[0]
                elif len(impulsi2) == 2:
                    time_diff = impulsi2[1]-impulsi2[0]
                else:
                    if impulsi1[0]>impulsi2[0]:
                        print("errore")
                    time_diff = abs(impulsi1[0]-impulsi2[0])

                tempi.append(time_diff)
                # ax1.set_title("Evento "+str(i)+" diff. temp.= "+str(time_diff))
                # ax1.set_xlabel("time")
                # plt.legend()
                # fig2, ax2 = plt.subplots()
                # #print(len())
                # ax2.plot(np.arange(ch_max-1)[interval[0]:interval[1]],derivata1[interval[0]:interval[1]])
                # ax2.plot(np.arange(ch_max-1)[interval[0]:interval[1]],derivata2[interval[0]:interval[1]])
                # #plt.show()
                # fig1.savefig("immagini/plot_corretti_"+str(i)+".jpg")
    #plt.close()

def up_or_down(signals, tempi_up, tempi_down):
    if not sovrapposizione_quadra(signals):

        derivata1 = delta(signals[0])
        derivata2 = delta(signals[1])
        interval = [min(minmax(derivata1)[0],minmax(derivata2)[0]),max(minmax(derivata1)[1],minmax(derivata2)[1])]
        #ulteriore controllo per vedere se ci sono esattamente un impulso di start ed uno di stop (tot = 2)
        impulsi1 = pos_impulso(derivata1)
        impulsi2 = pos_impulso(derivata2)

        if (len(impulsi1) + len(impulsi2)) == 2:
            if not (doppia(derivata1) or doppia(derivata2)):

                if len(impulsi1) == 2:
                    #up decay
                    time_diff = impulsi1[1]-impulsi1[0]
                    tempi_up.append(time_diff)
                elif len(impulsi1)==1 and len(impulsi2)==1 and impulsi1[0]<impulsi2[0]:
                    #down decay
                    time_diff = impulsi2[0]-impulsi1[0]
                    tempi_down.append(time_diff)

                else:
                    print("errore")


def db_analysis(j, n_obs, tempi):
    conn = sqlite3.connect(argv[2])
    c = conn.cursor()
    r = c.execute('SELECT samples FROM samples where event_id>? and event_id<?',(j*n_obs,(j+1)*n_obs+1))
    i=0
    for row in r:
        #print(i)
        if i%2==0:
            signals=[np.array(convert_samples(row[0]))]

        else:
            signals.append(np.array(convert_samples(row[0])))
            check_signals(signals,tempi)
        i+=1



def db_analysis_up_down(j, n_obs, tempi_up, tempi_down):
    conn = sqlite3.connect(argv[2])
    c = conn.cursor()
    r = c.execute('SELECT samples FROM samples where event_id>? and event_id<?',(j*n_obs,(j+1)*n_obs+1))
    i=0
    for row in r:
        #print(i)
        if i%2==0:
            signals=[np.array(convert_samples(row[0]))]

        else:
            signals.append(np.array(convert_samples(row[0])))
            up_or_down(signals, tempi_up, tempi_down)
        i+=1


ch_max = 4096
if __name__ == "__main__":

    #DEVO LAVORARE SOLO CON UN CANALE, IL CANALE 1
    #"finale_cerbero_sopra.xml"
    if argv[1] == '-db':
        conn = sqlite3.connect(argv[2])
        c = conn.cursor()

        n_obs = 2000
        # j=check_last_analysis()+1
        # print(j)
        lenght = c.execute('SELECT COUNT(*) from events').fetchone()
        print(lenght[0])
        reps = ceil(lenght[0]/n_obs)
        print(reps)
        jobs = []
        manager = multiprocessing.Manager()
        tempi_up = manager.list()
        tempi_down = manager.list()
        for k in range(reps):
            p = multiprocessing.Process(target=db_analysis_up_down, args=(k, n_obs, tempi_up, tempi_down))
            jobs.append(p)
            p.start()
        for proc in jobs:
            proc.join()
        #print(tempi)
        with open(argv[3]+'_up.txt', 'w') as file:
            for i in tempi_up:
                file.write(str(i)+"\n")
        with open(argv[3]+'_down.txt', 'w') as file:
            for i in tempi_down:
                file.write(str(i)+"\n")





    elif argv[1] == '-xml':
        data = dt.DT5751reader(argv[2])

        evento = data.get()
        n_obs = 2000
        j=check_last_analysis()+1
        print(j)
        if j!=0:
            #devo portarmi fino a quel j con i get
            i=0
            print("Caricamento")
            while i< j*n_obs and evento!= None:
                evento=data.get()
                print(str(round(i/(j*n_obs)*100,2))+"%")
                i+=1
        print("Caricamento finito")


        while(evento):
            i = j * n_obs
            tempi = []
            while(evento!= None and i< n_obs*(j+1)):
                ch_max = 4096
                signals = [np.array(evento['channels'][0][:ch_max]),np.array(evento['channels'][1][:ch_max])]
                #print('sovrapposizione: '+str(i))
                #print(sovrapposizione_quadra(signals))

                if not sovrapposizione_quadra(signals):

                    derivata1 = delta(signals[0])
                    derivata2 = delta(signals[1])
                    interval = [min(minmax(derivata1)[0],minmax(derivata2)[0]),max(minmax(derivata1)[1],minmax(derivata2)[1])]
                    #interval = [minmax(derivata1)[0],minmax(derivata1)[1]]
                    #ulteriore controllo per vedere se ci sono esattamente un impulso di start ed uno di stop (tot = 2)
                    impulsi1 = pos_impulso(derivata1)
                    impulsi2 = pos_impulso(derivata2)
                    #print('impulsi: '+str(i))
                    #print(impulsi1)
                    #print(impulsi2)
                    #print('doppia: '+str(i))
                    #print(doppia(derivata1))
                    #print(doppia(derivata2))

                    if (len(impulsi1) + len(impulsi1)) == 2:
                        if not (doppia(derivata1) or doppia(derivata2)):
                            fig1, ax1 = plt.subplots(1, 1, tight_layout=True)
                            ax1.plot(np.arange(ch_max)[interval[0]:interval[1]],signals[0][interval[0]:interval[1]],label='Ch0')
                            ax1.plot(np.arange(ch_max)[interval[0]:interval[1]],signals[1][interval[0]:interval[1]], label='Ch1')

                            if len(impulsi1) == 2:
                                time_diff = impulsi1[1]-impulsi1[0]
                            elif len(impulsi2) == 2:
                                time_diff = impulsi2[1]-impulsi2[0]
                            else:
                                time_diff = abs(impulsi1[0]-impulsi2[0])

                            tempi.append(time_diff)
                            ax1.set_title("Evento "+str(i)+" diff. temp.= "+str(time_diff))
                            ax1.set_xlabel("time")
                            plt.legend()
                            fig2, ax2 = plt.subplots()
                            #print(len())
                            ax2.plot(np.arange(ch_max-1)[interval[0]:interval[1]],derivata1[interval[0]:interval[1]])
                            ax2.plot(np.arange(ch_max-1)[interval[0]:interval[1]],derivata2[interval[0]:interval[1]])
                            #plt.show()
                            fig1.savefig("immagini/plot_corretti_"+str(i)+".jpg")
                plt.close()
                evento = data.get()
                i+=1


            if j==0:
                save_vec("w", tempi,j)
            else:
                save_vec("a", tempi,j)
            j+=1
