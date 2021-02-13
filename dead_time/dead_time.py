import sqlite3
from sys import argv
from math import ceil
import multiprocessing
from functions_analysis import dead_time, rate_tot


if __name__ == "__main__":


    conn = sqlite3.connect(argv[1])
    c = conn.cursor()

    n_obs = 2000
    lenght = c.execute('SELECT COUNT(*) from events').fetchone()
    print(lenght[0])
    reps = ceil(lenght[0]/n_obs)
    print(reps)
    jobs = []
    manager = multiprocessing.Manager()
    times = manager.list()
    for k in range(reps):
        p = multiprocessing.Process(target=dead_time, args=(argv[1], k, n_obs, times))
        jobs.append(p)
        p.start()
    for proc in jobs:
        proc.join()
    with open('dead_times.txt', 'w') as file:
        for i in times:
            file.write(str(i)+"\n")

    print(rate_tot(argv[1]))
