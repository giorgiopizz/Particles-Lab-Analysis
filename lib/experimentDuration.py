import os,sys,inspect
current_dir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parent_dir = os.path.dirname(current_dir)

def experimentDuration(filename):
    conn = sqlite3.connect(filename)
    c = conn.cursor()
    t_i = c.execute('SELECT MIN(time_stamp) FROM events').fetchone()[0]
    t_f = c.execute('SELECT MAX(time_stamp) FROM events').fetchone()[0]
    return (t_f-t_i)



if __name__ == "__main__":
    p = os.path.join(parent_dir, 'dati')
    files = os.listdir(p)
    for file in files:
        print("File: ", file, ":")
        print(experimentDuration(os.path.join(p, file)))
