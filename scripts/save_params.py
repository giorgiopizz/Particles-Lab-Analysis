import re

a ="""   1  p0           7.91607e+02   2.16708e+02   1.43246e-01   6.98852e-06
   2  p1           8.35622e-01   2.60822e-01   1.08283e-04   8.38404e-04
   3  p2           1.35374e+03   2.76118e+02   4.76451e-02  -3.82527e-06
   4  p3           2.20780e+00   2.07410e-01   5.81078e-05  -1.15356e-02
   5  p4           5.40262e+01   5.28776e+00   6.00339e-03   3.08141e-04"""

pattern = re.compile("p[\d][ ]*([\d.e+-]*)")

params = re.findall(pattern,a)
s = "parameters = "
for i in params:
    s+=str(i)+", "
print(s[:len(s)-2])
