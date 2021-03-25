import re

a ="""   1  p0           8.14644e+01   1.05894e+02   1.71763e-01   2.46179e-05
   2  p1           6.10250e-01   5.86841e-01   6.70765e-04   9.91392e-03
   3  p2           5.10168e+02   4.53024e+01   3.09998e-02   1.85832e-04
   4  p3           2.14881e+00   1.15096e-01   9.47583e-05   3.40290e-02
   5  p4           1.99093e+01   1.54845e+00   3.34120e-03   6.16446e-05"""

pattern = re.compile("p[\d][ ]*([\d.e+-]*)")

params = re.findall(pattern,a)
s = "parameters = "
for i in params:
    s+=str(i)+", "
print(s[:len(s)-2])
