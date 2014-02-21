import os
from datetime import date

searchdirs = ["../C_Binding_Commands/MODELING/", "../C_Binding_Commands/SIMULATION/"]
fin = open("feiparser.yy",'r')
fout = open("dsl_to_ebl_report.txt","w")

fout.write("Implementation report.\n")

for thisdir in searchdirs:
    
    dircontents = os.listdir(thisdir)
    dircontents.sort()
    
    fout.write("\n"+thisdir+"\n\n")
    
    count_implemented = 0
    count_total       = 0
    for dsl in dircontents:
        if dsl.find(".h") > 0:        
            found = False
            fout.write("{0:80s}".format(dsl[:-2:]))
        
            for line in fin:
                if line.find(dsl[:-2:]) > 0:
                    found = True
                    count_implemented += 1
                    break
                    
            if found:
                fout.write("[  :) ]")
            else:
                fout.write("[ >:( ]")
            fout.write("\n")
            fin.seek(0)
            
            count_total += 1
        pass
    
    fout.write("\n{} of {} DSLs have been bound into FEI code. \n".format(count_implemented, count_total))
    fout.write("{0:5.1f}% progress.\n".format(float(count_implemented)/float(count_total) * 100))

fin.close()
fout.close()
