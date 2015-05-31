import h5py
import scipy as sp
import scipy.fftpack as fft
import matplotlib.pyplot as plt
import time

DRM_motion_filename = ""
Ne = 4
Nb = 4
elements = [10]
all_nodes = [37, 38, 39, 40, 41, 42, 43, 44]

t = sp.linspace(0,10,1001)
w = 2*sp.pi/0.5
d = sp.sin(w*t)
a = -w**2*sp.sin(w*t)

h5file = h5py.File(DRM_motion_filename,"w")

h5file.create_dataset("Elements", data=elements)
h5file.create_dataset("DRM Nodes", data=all_nodes)
h5file.create_dataset("Is Boundary Node", data=is_boundary_node)  #This array has 1 if the node at the corresponding position in "DRM nodes" array is a boundary node and zero if not

h5file.create_dataset("Number of Exterior Nodes", data=Ne)
h5file.create_dataset("Number of Boundary Nodes", data=Nb)

#Write timestamp (time format used is that of c "asctime" Www Mmm dd hh:mm:ss yyyy  example: Tue Jan 13 10:17:09 2009)
localtime = time.asctime( time.localtime(time.time()) )
h5file.create_dataset("Created",data=str(localtime))

h5file.create_dataset("Time", data=t)

acc = h5file.create_dataset("Accelerations", (3*Nt,len(t)), dtype=sp.double)
dis = h5file.create_dataset("Displacements", (3*Nt,len(t)), dtype=sp.double)

h5file.close()


