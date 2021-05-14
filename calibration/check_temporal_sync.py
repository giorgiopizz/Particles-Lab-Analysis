import sqlite3
import matplotlib.pyplot as plt
import numpy as np
from sys import argv
from os import rename
from math import ceil
import multiprocessing

# from file_conversion import convert, merge

import os,sys,inspect
current_dir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parent_dir = os.path.dirname(current_dir)+"/lib"
sys.path.insert(0, parent_dir)

import DT5751read as dt

from functions_analysis import sync_db, ch_max



if __name__ == "__main__":

    if argv[1] == '-db':
        conn = sqlite3.connect(argv[2])
        c = conn.cursor()

        n_obs = 200
        lenght = c.execute('SELECT COUNT(*) from events').fetchone()
        # lenght = [10]
        print(lenght[0])
        reps = ceil(lenght[0]/n_obs)
        print(reps)
        # WITH multiprocessing
        jobs = []
        manager = multiprocessing.Manager()
        tempi = manager.list()
        for k in range(reps):
            p = multiprocessing.Process(target=sync_db, args=(argv[2], k, n_obs, tempi))
            jobs.append(p)
            p.start()
        for proc in jobs:
            proc.join()

        tempi_n = np.array(tempi)
        tempi_n = 4*tempi_n

        print(np.min(tempi_n), np.max(tempi_n), np.mean(tempi_n),np.std(tempi_n))
        # WITHOUT multiprocessing
        # tempi_up = []
        # tempi_down = []
        # for k in range(reps):
        #     db_analysis_up_down(argv[2], k, n_obs,tempi_up, tempi_down)
        # with open(argv[3]+'_sync.txt', 'w') as file:
        #     for i in tempi_up:
        #         file.write(str(i)+"\n")
        #
        # convert(argv[3])
