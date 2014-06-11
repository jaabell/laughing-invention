#!/usr/bin/python
#plot_node_output.py - Plot all DOF outputs in a subplot for a given node tag
import sys

#Import the plotting tools
import matplotlib.pyplot as plt
import scipy as sp


def print_usage():
    print """
essipre_genwave_ricker.py

Generate a Ricker Wave for input into ESSI 

Usage: 
    
    essipre_genwave_ricker.py dt tmax d0 Tp ts basename

    dt       = time interval                         [ seconds]
    tmax     = final time                            [ seconds]
    d0       = Amplitude
    Tp       = predominant period                    [ seconds]
    ts       = center time (time of maximum) of wave [ seconds]
    basename = basename of the generated files
         generated files will be:
             + <basename>_displacements.dat
             + <basename>_velocities.dat
             + <basename>_accelerations.dat
             + <basename>_parameters.txt

---
CompGeoMech 2014 - Jose Abells
"""
    return 0

#Plot all dofs of this file and node
narg  =len(sys.argv)

if narg == 7:
	dt   = float(sys.argv[1]) #0.01   #Interval
	tmax = float(sys.argv[2])  #9    #Total time
	d0   = float(sys.argv[3])  #1.0    #Amplitude
	Tp   = float(sys.argv[4])  #0.10   #Predominant period
	ts   = float(sys.argv[5])  #0.2     #Time of maximum
	basename = sys.argv[6]
else:
    print_usage()
    exit(-1)



t = sp.arange(0, tmax, dt)

pi = sp.pi
exp = sp.exp

y = d0*(-1 + 2*pi**2*(t - ts)**2/Tp**2)*exp(-pi**2*(t - ts)**2/Tp**2)
yp = -pi**2*d0*(-1 + 2*pi**2*(t - ts)**2/Tp**2)*(2*t - 2*ts)*exp(-pi**2*(t - ts)**2/Tp**2)/Tp**2 + 2*pi**2*d0*(2*t - 2*ts)*exp(-pi**2*(t - ts)**2/Tp**2)/Tp**2
ypp = -2*pi**2*d0*(-1 + 2*pi**2*(t - ts)**2/Tp**2)*exp(-pi**2*(t - ts)**2/Tp**2)/Tp**2 + 4*pi**2*d0*exp(-pi**2*(t - ts)**2/Tp**2)/Tp**2 + pi**4*d0*(-1 + 2*pi**2*(t - ts)**2/Tp**2)*(2*t - 2*ts)**2*exp(-pi**2*(t - ts)**2/Tp**2)/Tp**4 - 4*pi**4*d0*(2*t - 2*ts)**2*exp(-pi**2*(t - ts)**2/Tp**2)/Tp**4

data1 = sp.vstack((t,y)).T
sp.savetxt(basename + "_displacement.dat",y)

data2 = sp.vstack((t,yp)).T
sp.savetxt(basename + "_velocity.dat",yp)

data3 = sp.vstack((t,ypp)).T
sp.savetxt(basename + "_acceleration.dat",ypp)

writeout = ["Called:" + sys.argv[0] + " {} {} {} {} {} {}".format(dt, tmax, d0, Tp, ts, basename),
"dt       = {}".format(dt),
"tmax     = {}".format(tmax),
"d0       = {}".format(d0),
"Tp       = {}".format(Tp),
"ts       = {}".format(ts)]

fid = open(basename + "_parameters.txt","w")
for l in writeout:
	print l
	fid.write(l+"\n")
fid.close()



plt.figure()
plt.subplot(311)
plt.title("Ricker 2 Wavelet")
plt.plot(t,y)
plt.ylabel("d(t)")
plt.subplot(312)
plt.plot(t,yp)
plt.ylabel("v(t)")
plt.subplot(313)
plt.plot(t,ypp)
plt.ylabel("a(t)")
plt.xlabel("t")

plt.show()
#plt.ylabel("y")
