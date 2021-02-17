import sqlite3
import matplotlib.pyplot as plt
import numpy as np
from sys import argv
from math import ceil
import multiprocessing
import os,sys,inspect
current_dir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parent_dir = os.path.dirname(current_dir)
sys.path.insert(0, parent_dir)
import lib.DT5751read as dt
from lib.functions_analysis import convert_samples, ch_max, delta, start_impulso, intersect



def mean(l):
    return sum(l)/len(l)


def discr_calib(signals,i):
    signal = signals[0]
    #ground = (signal.max()+signal.min())/2
    ground = signal[:2000].mean()
    #print(ground, true_ground)
    th_50 = (ground + signal.min())/2
    th_75 = (ground + 3* signal.min())/4

    discr = signals[1]

    der = delta(discr)
    t2 = start_impulso(der)[0]

    t1_50 = intersect(signal, th_50)[0]
    t1_75 = intersect(signal, th_75)[0]
    deltat_50 = t2-t1_50
    deltat_75 = t2-t1_75

    bool_50 = False
    if abs(deltat_75)<abs(deltat_50) and deltat_75>0:
        #it's 75% threashold
        pass

    else:
        bool_50 = True

    # PLOT options
    # signal = signal[500:2000]
    # discr = discr[500:2000]
    # y_ground = np.ones(len(signal)) * ground
    # y_th_50 = np.ones(len(signal)) * th_50
    # y_th_75 =  np.ones(len(signal)) * th_75
    # fig1, ax1 = plt.subplots(1, 1, tight_layout=True)
    # ax1.plot(np.arange(len(signal)), signal,label='Ch0')
    # ax1.plot(np.arange(len(signal)), discr,label='Ch1')
    # ax1.plot(np.arange(len(signal)), y_ground,label='ground')
    # ax1.plot(np.arange(len(signal)), y_th_50,label='th_50')
    # ax1.plot(np.arange(len(signal)), y_th_75,label='th_75')
    # plt.legend()
    # fig1.savefig("immagini/discriminator_calibration"+str(i)+".jpg")
    # plt.close()

    if bool_50:
        return "50",deltat_50
    else:
        return "75",deltat_75

if __name__ == "__main__":
    conn = sqlite3.connect("calib_discr_ch_1.db")
    c = conn.cursor()
    r = c.execute('SELECT samples FROM samples where event_id>5')
    i=0
    times = {"50":[],"75":[]}
    for row in r:
        if i%2==0:
            signals=[np.array(convert_samples(row[0]))]

        else:
            signals.append(np.array(convert_samples(row[0])))
            t, time = discr_calib(signals, i)
            times[t].append(time)
        i+=1
    for i in range(3):
        t_50 = mean(times["50"][i*20:(i+1)*20])
        t_75 = mean(times["75"][i*20:(i+1)*20])
        std_50 = np.std(times["50"][i*20:(i+1)*20])
        std_75 = np.std(times["75"][i*20:(i+1)*20])
        print("for {}-th discriminator we got \nt_50: {}±{}\nt_75: {}±{}".format(i+1, t_50,std_50,t_75,std_75))
