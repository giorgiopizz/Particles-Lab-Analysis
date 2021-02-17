import sqlite3
from sys import argv
from math import ceil
import multiprocessing


def dead_time(filename, j, n_obs, times):
    conn = sqlite3.connect(filename)
    c = conn.cursor()
    r = c.execute('SELECT time_stamp FROM events where id>? and id<?',(j*n_obs,(j+1)*n_obs+1))
    i=0
    prev_time = 0
    for row in r:
        if prev_time !=0:
            times.append(row[0]-prev_time)

        prev_time = row[0]

def rate_tot(filename):
    conn = sqlite3.connect(filename)
    c = conn.cursor()
    t_i = c.execute('SELECT MIN(time_stamp) FROM events').fetchone()[0]
    t_f = c.execute('SELECT MAX(time_stamp) FROM events').fetchone()[0]
    n = c.execute('SELECT COUNT(*) FROM events').fetchone()[0]
    return n/(t_f-t_i)


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
