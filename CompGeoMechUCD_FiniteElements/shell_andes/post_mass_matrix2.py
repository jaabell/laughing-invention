
import sympy.printing as prt

fid = open("Gp.txt","w")

for i in range(9):
    for j in range(9):
        cplusplus = "Gp({},{}) = {};\n".format(i,j, prt.ccode(Gp[i,j]))
        fid.write(cplusplus)
    fid.write("\n")
fid.close()