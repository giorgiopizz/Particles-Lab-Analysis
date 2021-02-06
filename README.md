# Particles Lab Analysis
***
File converters
- analysis.py -> simple analysis, run with -db or -xml to specify input file format, also include as third parameter name for the output file (without ".txt")
- analysis_up_down.py: creates separate files for up decays and down decays. Parameters are the same as for analysis.py
- conversione_file.py: with -c it converts a txt file in microseconds. With -m it merges the first two input files into the third one
***

Fit
- fit2.cpp: run with ``./fit2 fit.cfg`` where the fit.cfg is the config file containing settings for the fit and also the file with data to plot
