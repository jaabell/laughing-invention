#!/usr/bin/python

import scipy as sp
import matplotlib.pyplot as plt

files = ["out_von.txt", "out_drucker.txt"]

for f in files:

	data = sp.loadtxt(f,skiprows=1)

	eps = data[:,0:6]
	sig = data[:,6::]


	p = data[:,12]
	ev = data[:,13]
	q = data[:,14]
	eq = data[:,15]


	k = 1.0;
	M = k
	# M = sp.sqrt(2.0/3.0)*k

	# plt.figure()
	# plt.subplot(3,2,1)
	# plt.plot(eps[:,0])
	# plt.subplot(3,2,2)
	# plt.plot(eps[:,1])
	# plt.subplot(3,2,3)
	# plt.plot(eps[:,2])
	# plt.subplot(3,2,4)
	# plt.plot(eps[:,3])
	# plt.subplot(3,2,5)
	# plt.plot(eps[:,4])
	# plt.subplot(3,2,6)
	# plt.plot(eps[:,5])

	# plt.figure()
	# plt.subplot(3,2,1)
	# plt.plot( sig[:,0])
	# plt.subplot(3,2,2)
	# plt.plot( sig[:,1])
	# plt.subplot(3,2,3)
	# plt.plot( sig[:,2])
	# plt.subplot(3,2,4)
	# plt.plot( sig[:,3])
	# plt.subplot(3,2,5)
	# plt.plot( sig[:,4])
	# plt.subplot(3,2,6)
	# plt.plot( sig[:,5])

	plt.figure(1)
	plt.subplot(3,2,1)
	plt.plot(eps[:,0], sig[:,0])
	plt.subplot(3,2,2)
	plt.plot(eps[:,1], sig[:,1])
	plt.subplot(3,2,3)
	plt.plot(eps[:,2], sig[:,2])
	plt.subplot(3,2,4)
	plt.plot(eps[:,3], sig[:,3])
	plt.subplot(3,2,5)
	plt.plot(eps[:,4], sig[:,4])
	plt.subplot(3,2,6)
	plt.plot(eps[:,5], sig[:,5])

	plt.figure(2)
	plt.plot(p,q,".-")
	plt.plot([0,p.max()],[0,M*p.max()],"k--")
	plt.plot([0,p.max()],[0,-M*p.max()],"k--")
	plt.grid("on")
	plt.xlim([0,p.max()])
	plt.xlabel("p")
	plt.ylabel("q")

	plt.figure(3)
	plt.subplot(2,1,1)
	plt.plot(p)
	plt.xlabel("Step number")
	plt.ylabel("p")
	plt.subplot(2,1,2)
	plt.plot(q)
	plt.xlabel("Step number")
	plt.ylabel("q")
	# plt.figure()
	# plt.subplot(2,1,1)
	# plt.plot(ev,p)

	# plt.xlabel("ev")
	# plt.ylabel("p")

	# plt.subplot(2,1,2)

	# plt.plot(eq,q)
	# plt.xlabel("eq")
	# plt.ylabel("q")

plt.show()
