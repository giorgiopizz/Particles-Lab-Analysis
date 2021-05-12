import sqlite3
import numpy as np
import matplotlib.pyplot as plt
ch_max = 4096

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


def square_overlap(signals):
    #signals is a list of np.arrays

    for i in range(2):
        #loop over channels, check for each square if there's overlap with the other

        cond_array = signals[i]<max(signals[i])-500
        # the other signal is signals[(i+1)%2]
        # check if there's overlap between the two
        # subarray of the other channel where this channel is active

        a = signals[(i+1)%2][cond_array]
        # subarray of the above where also the other channel is active
        b = a[a<max(signals[(i+1)%2])-500]
        if len(b)>0:
            return True
        else:
            return False


def intersect(signal, value):
    positions = []
    found = False
    for i in range(len(signal)):
        if not found and signal[i]<value:
            positions.append(i)
            found = True
        elif found and signal[i]>value:
            found = False
    return positions



def start_impulso(der):
    # non mi piace la funzione sopra che identifica al posizione dello start dell'impulso
    # qui sfruttando la derivata guardo solo se la derivata supera un certo valore soglia val_th
    val_th = -400
    return intersect(der, val_th)
    #quindi quello che si fa è guardare quando derivata supera il valore soglia, appena lo supera si registra la posizione
    # appena invece si scende sotto la soglia bisogna resettare found



def double_square(der):
    # should return a list of all the real starts of pulse
    # if a double/triple... square is present should return only the first start
    # start e stop della quadra
    start = start_impulso(der)
    stop = start_impulso(der*(-1))
    # print(start, stop)
    if len(start)!=len(stop) or len(start)==0:
        # non è una doppia ma va scartata perché ci siamo persi o uno start o uno stop
        return []
    for i in range(len(start)):
        if start[i]>=stop[i]:
            # then the i-th stop is not associated with the i-th start
            return []

    # time width of a square pulse
    #time_width = max(np.array(stop)-np.array(start))
    time_width = np.mean(np.array(stop)-np.array(start))
    # print(time_width)
    pulses_start = []
    for i in range(len(start)):

        #lasso = stop[i]-start[i]
        # if i>=1 and stop[i-1]-start[i]<2*time_width:
        if i>=1 and start[i]-stop[i-1] < 2*time_width:
            # there is a double square
            #print('buttato')
            continue
        else:
            pulses_start.append(start[i])
    # print(pulses_start)
    return start



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

def plot(signals,i,keep='tenuto'):
    der1 = delta(signals[0])
    der2 = delta(signals[1])
    interval = [min(minmax(der1)[0],minmax(der2)[0]),max(minmax(der1)[1],minmax(der2)[1])]
    fig1, ax1 = plt.subplots(1, 1, tight_layout=True)
    ax1.plot(np.arange(ch_max)[interval[0]:interval[1]],signals[0][interval[0]:interval[1]],label='Ch0')
    ax1.plot(np.arange(ch_max)[interval[0]:interval[1]],signals[1][interval[0]:interval[1]], label='Ch1')
    ax1.set_title("Evento "+str(i)+" "+keep)
    ax1.set_xlabel("time")
    plt.legend()
    fig2, ax2 = plt.subplots()
    #print(len())
    ax2.plot(np.arange(ch_max-1)[interval[0]:interval[1]],der1[interval[0]:interval[1]])
    ax2.plot(np.arange(ch_max-1)[interval[0]:interval[1]],der2[interval[0]:interval[1]])
    #plt.show()
    fig1.savefig("immagini/plot_nuovi_3_"+str(i)+".jpg")
    fig2.savefig("immagini/plot_nuovi_3_"+str(i)+"_der.jpg")
    plt.cla()
    plt.clf()
    plt.close('all')

def up_or_down(signals, tempi_up, tempi_down, i, j):
    if not square_overlap(signals):
        der1 = delta(signals[0])
        der2 = delta(signals[1])
        interval = [min(minmax(der1)[0],minmax(der2)[0]),max(minmax(der1)[1],minmax(der2)[1])]

        real_pulses1 = double_square(der1)
        real_pulses2 = double_square(der2)
        if (len(real_pulses1) + len(real_pulses2)) == 2:
                if len(real_pulses1) == 2:
                    #up decay
                    time_diff = real_pulses1[1]-real_pulses1[0]

                    tempi_up.append(time_diff)
                    # double_square_width is the min time separation between two up signals
                    # a separation less than this value is a double square
                    # first value in ns, result in digitized time unit
                    # for cerbero above should be 100, for minosse above is sufficient 80 ns
                    #
                    # double_square_width = 100 / 4
                    # # we double check for double square, first with double_square we require the start of a pulse and
                    # # stop of the previous (in the same channel) to be separated in time more than the mean duration of
                    # # a square pulse. The with the below criteria we impose the start of the next pulse to be at a time
                    # # distance greater than 100 ns from the previous start
                    # #double_square_width = real
                    # if time_diff>double_square_width:
                    #     # we don't want double square with a gap under 80ns, because this was a false trigger
                    #     # i.e. the signal time width was so large that even a retarded start would overlap with the
                    #     # stop signal (which was the same signal of the start)
                    #     # look up for start and stop topology on the paper
                    #     tempi_up.append(time_diff)
                    # else:
                    #     return 'double square'
                elif len(real_pulses1)==1 and len(real_pulses2)==1 and real_pulses1[0]<real_pulses2[0]:
                    #down decay
                    time_diff = real_pulses2[0]-real_pulses1[0]
                    tempi_down.append(time_diff)

                else:
                    print("error", i, j)
                    print(real_pulses1, real_pulses2)
                return 'error'
        else:
            return 'too many pulses'
    else:
        return 'overlap'


def db_analysis(filename, j, n_obs, tempi):
    conn = sqlite3.connect(filename)
    c = conn.cursor()
    r = c.execute('SELECT samples FROM samples where event_id>? and event_id<?',(j*n_obs,(j+1)*n_obs+1))
    i=0
    for row in r:
        if i%2==0:
            signals=[np.array(convert_samples(row[0]))]

        else:
            signals.append(np.array(convert_samples(row[0])))
            check_signals(signals,tempi)
        i+=1



def db_analysis_up_down(filename, j, n_obs, tempi_up, tempi_down):
    conn = sqlite3.connect(filename)
    c = conn.cursor()
    r = c.execute('SELECT samples FROM samples where event_id>=? and event_id<?',(j*n_obs,(j+1)*n_obs))
    i=0
    for row in r:
        #print(i)
        if i%2==0:
            signals=[np.array(convert_samples(row[0]))]

        else:
            signals.append(np.array(convert_samples(row[0])))
            up_or_down(signals, tempi_up, tempi_down, i, j)

        i+=1
def db_plot(filename, j, n_obs, tempi_up, tempi_down, theOne):
    conn = sqlite3.connect(filename)
    c = conn.cursor()
    r = c.execute('SELECT samples FROM samples where event_id>=? and event_id<?',(j*n_obs,(j+1)*n_obs+1))
    i=0
    for row in r:
        #print(i)
        if i>=theOne-1 and i<theOne+1:
            if i%2==0:
                signals=[np.array(convert_samples(row[0]))]

            else:
                #print(i)
                signals.append(np.array(convert_samples(row[0])))
                init_len = len(tempi_up) + len(tempi_down)
                res = up_or_down(signals, tempi_up, tempi_down, i, j)
                if res == 'error':
                    print(i)
                    plot(signals, i, 'error')
                if len(tempi_up) + len(tempi_down) == init_len:
                    plot(signals,i,'buttato '+ res)
                else:
                    plot(signals,i)

                    # if i%2==0:
                    #     signals=[np.array(convert_samples(row[0]))]
                    #
                    #     signals.append(np.array(convert_samples(row[0])))
                    #     init_len = len(tempi_up) + len(tempi_down)
                    #     p = up_or_down(signals, tempi_up, tempi_down)
                    #     if len(tempi_up) + len(tempi_down) == init_len:
                    #         plot(signals,i,'buttato '+p)
                    #     else:
                    #         plot(signals,i)
        i+=1
