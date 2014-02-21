# -*- coding: utf-8 -*-
"""
Created on Mon Nov 19 15:52:54 2012

@author: jaabell
"""

import sympy as sym
import scipy as sp

x = sym.Symbol("x")
y = sym.Symbol("y")
z = 1 - x - y

N = sym.matrices.zeros((12,2))

N[0,0] = x*(2*x - 1)
N[1,1] = x*(2*x - 1)
N[2,0] = y*(2*y - 1)
N[3,1] = y*(2*y - 1)
N[4,0] = z*(2*z - 1)
N[5,1] = z*(2*z - 1)
N[6,0] = 4*x*y
N[7,1] = 4*x*y
N[8,0] = 4*y*z
N[9,1] = 4*y*z
N[10,0] = 4*x*z
N[11,1] = 4*x*z

Mq = sym.integrate(sym.integrate(N*N.T,(y,0,1-x) ),(x,0,1))*2

print Mq*180

fid = open("mem_mass.txt",'w')

fid.write("// Me = \n")
for i in range(12):
    for j in range(12):
        fid.write("Mc({},{}) = {}; ".format(i,j,Mq[i,j]*180))
    fid.write(" \n")



alpha  = sym.Symbol("alpha")

x11 = sym.Symbol("x11")
x12 = sym.Symbol("x12")
x13 = sym.Symbol("x13")
x22 = sym.Symbol("x22")
x23 = sym.Symbol("x23")
x33 = sym.Symbol("x33")

x21 = -x12
x31 = -x13
x32 = -x23

y11 = sym.Symbol("y11")
y12 = sym.Symbol("y12")
y13 = sym.Symbol("y13")
y22 = sym.Symbol("y22")
y23 = sym.Symbol("y23")
y33 = sym.Symbol("y33")

y21 = -y12
y31 = -y13
y32 = -y23

q11 = sym.Symbol("q_11")
q12 = sym.Symbol("q_12")
q13 = sym.Symbol("q_13")
q22 = sym.Symbol("q_22")
q23 = sym.Symbol("q_23")
q33 = sym.Symbol("q_33")

q21 = -q12
q31 = -q13
q32 = -q23

p11 = sym.Symbol("p_11")
p12 = sym.Symbol("p_12")
p13 = sym.Symbol("p_13")
p22 = sym.Symbol("p_22")
p23 = sym.Symbol("p_23")
p33 = sym.Symbol("p_33")

p21 = -p12
p31 = -p13
p32 = -p23

Tcr_ = sym.matrices.zeros((12,9))
Tcr_[0,0] = 1
Tcr_[1,1] = 1
Tcr_[2,3] = 1
Tcr_[3,4] = 1
Tcr_[4,6] = 1
Tcr_[5,7] = 1

Tcr_[6,2] = q12
Tcr_[6,5] = q21
Tcr_[6,8] = q33

Tcr_[7,2] = p21
Tcr_[7,5] = p12
Tcr_[7,8] = p33

Tcr_[8,2] = q11
Tcr_[8,5] = q23
Tcr_[8,8] = q32

Tcr_[9,2] = p11
Tcr_[9,5] = p32
Tcr_[9,8] = p23

Tcr_[10,2] = q13
Tcr_[10,5] = q22
Tcr_[10,8] = q31

Tcr_[11,2] = p31
Tcr_[11,5] = p22
Tcr_[11,8] = p13

Tcr_ = Tcr_.subs(q11,0).subs(q22,0).subs(q33,0).subs(p11,0).subs(p22,0).subs(p33,0)

one_half = sym.sympify("1/2")
Tch = sym.matrices.eye(12)
Tch[6,0] = one_half
Tch[6,2] = one_half
Tch[7,1] = one_half
Tch[7,3] = one_half
Tch[8,2] = one_half
Tch[8,4] = one_half
Tch[9,3] = one_half
Tch[9,5] = one_half
Tch[10,4] = one_half
Tch[10,0] = one_half
Tch[11,5] = one_half
Tch[11,1] = one_half

print Tcr_
print Tch

Tcr = Tch*Tcr_

Tcr = Tcr.subs(p12, alpha*x12/8)
Tcr = Tcr.subs(p23, alpha*x23/8)
Tcr = Tcr.subs(p13, alpha*x13/8)
Tcr = Tcr.subs(q12, alpha*y12/8)
Tcr = Tcr.subs(q23, alpha*y23/8)
Tcr = Tcr.subs(q13, alpha*y13/8)

print Tcr

fid.write("\n //Tcr = \n")
for i in range(12):
    for j in range(9):
        fid.write("Tcr({},{}) = {}; ".format(i,j,Tcr[i,j]))
    fid.write(" \n")

M = Tcr.T*Mq*Tcr*180

print M

fid.write("\n //M = \n")
for i in range(9):
    for j in range(9):
        fid.write("Ml({},{}) = {}; \n".format(i,j,M[i,j]))
        

fid.close()
