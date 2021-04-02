import sqlite3
import matplotlib.pyplot as plt
import numpy as np
from sys import argv
from os import rename
from math import ceil
import platform, multiprocessing

from file_conversion import convert, merge

import os,sys,inspect

current_dir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parent_dir = os.path.dirname(current_dir)+"/lib"
sys.path.insert(0, parent_dir)
import DT5751read as dt
from functions_analysis import db_plot


if __name__ == "__main__":
    if platform.system() == "Darwin":
        multiprocessing.set_start_method('spawn')
    if argv[1] == '-db':
        conn = sqlite3.connect(argv[2])
        c = conn.cursor()

        n_obs = 2000
        lenght = c.execute('SELECT COUNT(*) from events').fetchone()
        print(lenght[0])
        reps = ceil(lenght[0]/n_obs)
        print(reps)
        # WITH multiprocessing
        jobs = []
        manager = multiprocessing.Manager()
        tempi_up = manager.list()
        tempi_down = manager.list()
        for k in range(reps):
            p = multiprocessing.Process(target=db_plot, args=(argv[2], k, n_obs, tempi_up, tempi_down))
            jobs.append(p)
            p.start()
        for proc in jobs:
            proc.join()
        print(tempi_up)
        print("##########")
        print(tempi_down)
        # WITHOUT multiprocessing
        # tempi_up = []
        # tempi_down = []
        # for k in range(reps):
        #     db_analysis_up_down(argv[2], k, n_obs,tempi_up, tempi_down)

        """
        with open(argv[3]+'_up.txt', 'w') as file:
            for i in tempi_up:
                file.write(str(i)+"\n")
        with open(argv[3]+'_down.txt', 'w') as file:
            for i in tempi_down:
                file.write(str(i)+"\n")

        convert(argv[3]+'_up')
        convert(argv[3]+'_down')

        # current_dir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
        # parent_dir = os.path.dirname(current_dir)+"/tempi"
        # print(parent_dir)
        merge(argv[3]+'_up_corr', argv[3]+'_down_corr', argv[3]+'_tot')

  if 'tempi' in os.listdir():
            try:
                os.rename(os.path.join(os.getcwd(),argv[3]+'_up_corr.txt'), os.path.join(os.getcwd(), 'tempi', argv[3]+'_up.txt'))
                os.rename(os.path.join(os.getcwd(),argv[3]+'_down_corr.txt'), os.path.join(os.getcwd(), 'tempi', argv[3]+'_down.txt'))
                os.rename(os.path.join(os.getcwd(),argv[3]+'_tot.txt'), os.path.join(os.getcwd(), 'tempi', argv[3]+'_tot.txt'))
            except:
                os.remove(os.path.join(os.getcwd(), 'tempi', argv[3]+'_up.txt'))
                os.remove(os.path.join(os.getcwd(), 'tempi', argv[3]+'_down.txt'))
                os.remove(os.path.join(os.getcwd(), 'tempi', argv[3]+'_tot.txt'))
                os.rename(os.path.join(os.getcwd(),argv[3]+'_up_corr.txt'), os.path.join(os.getcwd(), 'tempi', argv[3]+'_up.txt'))
                os.rename(os.path.join(os.getcwd(),argv[3]+'_down_corr.txt'), os.path.join(os.getcwd(), 'tempi', argv[3]+'_down.txt'))
                os.rename(os.path.join(os.getcwd(),argv[3]+'_tot.txt'), os.path.join(os.getcwd(), 'tempi', argv[3]+'_tot.txt'))

        else:
            parent_dir = os.path.abspath(os.path.join(os.getcwd(), os.pardir))
            try:
                os.rename(os.path.join(os.getcwd(),argv[3]+'_up_corr.txt'), os.path.join(parent_dir, 'tempi', argv[3]+'_up.txt'))
                os.rename(os.path.join(os.getcwd(),argv[3]+'_down_corr.txt'), os.path.join(parent_dir, 'tempi', argv[3]+'_down.txt'))
                os.rename(os.path.join(os.getcwd(),argv[3]+'_tot.txt'), os.path.join(parent_dir, 'tempi', argv[3]+'_tot.txt'))
            except:
                os.remove(os.path.join(parent_dir, 'tempi', argv[3]+'_up.txt'))
                os.remove(os.path.join(parent_dir, 'tempi', argv[3]+'_down.txt'))
                os.remove(os.path.join(parent_dir, 'tempi', argv[3]+'_tot.txt'))
                os.rename(os.path.join(os.getcwd(),argv[3]+'_up_corr.txt'), os.path.join(parent_dir, 'tempi', argv[3]+'_up.txt'))
                os.rename(os.path.join(os.getcwd(),argv[3]+'_down_corr.txt'), os.path.join(parent_dir, 'tempi', argv[3]+'_down.txt'))
                os.rename(os.path.join(os.getcwd(),argv[3]+'_tot.txt'), os.path.join(parent_dir, 'tempi', argv[3]+'_tot.txt'))


        os.remove(argv[3]+'_up.txt')
        os.remove(argv[3]+'_down.txt')"""
