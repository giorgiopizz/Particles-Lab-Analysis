import re

a ="""   1  p0           1.03254e+02     fixed
   2  p1           6.85072e-01   9.80427e-02  -0.00000e+00   2.39169e-06
   3  p2           4.48517e+02     fixed
   4  p3           2.20000e+00     fixed
   5  p4           1.71124e+01     fixed """

pattern = re.compile("p[\d][ ]*([\d.e+-]*)")

params = re.findall(pattern,a)
s = "parameters = "
for i in params:
    s+=str(i)+", "
print(s[:len(s)-2])
