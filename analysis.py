import sqlite3
import matplotlib.pyplot as plt
import numpy as np
import DT5751read as dt
from sys import argv
from math import ceil
import multiprocessing
from functions_analysis import db_analysis, check_signals, ch_max


if __name__ == "__main__":

    if argv[1] == '-db':
        conn = sqlite3.connect(argv[2])
        c = conn.cursor()

        n_obs = 2000
        lenght = c.execute('SELECT COUNT(*) from events').fetchone()
        print(lenght[0])
        reps = ceil(lenght[0]/n_obs)
        print(reps)
        jobs = []
        manager = multiprocessing.Manager()
        tempi = manager.list()
        for k in range(reps):
            p = multiprocessing.Process(target=db_analysis, args=(k, n_obs, tempi))
            jobs.append(p)
            p.start()
        for proc in jobs:
            proc.join()

        with open(argv[3]+'.txt', 'w') as file:
            for i in tempi:
                file.write(str(i)+"\n")





    elif argv[1] == '-xml':
        data = dt.DT5751reader(argv[2])

        evento = data.get()
        tempi = []
        while(evento):

            signals = [np.array(evento['channels'][0][:ch_max]),np.array(evento['channels'][1][:ch_max])]
            check_signals(signals, tempi)
            evento = data.get()

        with open(argv[3]+'.txt', 'w') as file:
            for i in tempi:
                file.write(str(i)+"\n")
