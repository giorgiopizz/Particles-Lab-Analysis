import os
import sys
from sys import argv
from pathlib import Path

try:
    os.mkdir("results/" + argv[1])
    os.mkdir("results/" + argv[1] + "/fit")
    os.mkdir("results/" + argv[1] + "/stability")
except:
    print("errore")
Path("results/" + argv[1] + "/fit/analysis.txt").touch()
Path("results/" + argv[1] + "/stability/analysis.txt").touch()
