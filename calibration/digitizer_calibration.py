import sqlite3
import matplotlib.pyplot as plt
import numpy as np
import DT5751read as dt
from sys import argv
from math import ceil
import multiprocessing
from functions_analysis import db_analysis_up_down, up_or_down, ch_max





if __name__ == "__main__":
    conn = sqlite3.connect(argv[2])
    c = conn.cursor()
    r = c.execute('SELECT samples FROM samples where event_id>11')
    for row in r:
        signal = np.array(convert_samples(row[0]))
        ground = (signal.max()+signal.min())/2
        th_50 = (ground + signal.min())/2
        th_75 = (ground + 3* signal.min())/4

        y_ground = np.ones(len(signal)) * ground
        y_th_50 = np.ones(len(signal)) * th_50
        y_th_75 =  np.ones(len(signal)) * th_75
        fig1, ax1 = plt.subplots(1, 1, tight_layout=True)
        ax1.plot(np.arange(ch_max), signal,label='Ch0')
        ax1.plot(np.arange(ch_max), y_ground,label='ground')
        ax1.plot(np.arange(ch_max), y_th_50,label='th_50')
        ax1.plot(np.arange(ch_max), y_th_75,label='th_75')
