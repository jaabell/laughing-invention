# -*- coding: utf-8 -*-
"""
Created on Wed Sep 26 14:09:09 2012

This script computes the analytical form of the consistent mass matrix for a 3 
node triangular element with drilling. The formulation used is based on the
Appendix A of:
    
C.A. Felippa, P.G. Bergan, "A triangular bending element based on an energy-orthogonal
free formulation", Comp. Meth. in App. Mech. and Engr. 61 (1987) 129-160.



@author: jaabell
"""


import sympy as sym
import scipy as sp

x = sym.Symbol("x", real = True)
y = sym.Symbol("y", real = True)

x1 = sym.Symbol("x_1", real = True)
x2 = sym.Symbol("x_2", real = True)
x3 = sym.Symbol("x_3", real = True)
y1 = sym.Symbol("y_1", real = True)
y2 = sym.Symbol("y_2", real = True)
y3 = sym.Symbol("y_3", real = True)


x_12 = x1-x2
y_12 = y1-y2
x_23 = x2-x3
y_23 = y2-y3
x_31 = x3-x1
y_31 = y3-y1

x_21 = -x_12
y_21 = -y_12
x_32 = -x_23
y_32 = -y_23
x_13 = -x_31
y_13 = -y_31

xc = (x1+x2+x3)/3
yc = (y1+y2+y3)/3

L1 = sym.sqrt( x_21**2 + y_21**2 )
L2 = sym.sqrt( x_13**2 + y_13**2 )
L3 = sym.sqrt( x_32**2 + y_32**2 )

s = (L1 + L2 + L3)/2
#A = sym.Symbol("A", real = True)
A2 = x2*y3 - x3*y2 + x3*y1- x1*y3 +x1*y2 - x2*y1
lam = 1/sym.sqrt(A2/2)

xi_1 = (x2*y3 - x3*y2 + (x)*y_23 + (y)*x_32 )/A2
xi_2 = (x3*y1 - x1*y3 + (x)*y_31 + (y)*x_13 )/A2
xi_3 = (x1*y2 - x2*y1 + (x)*y_12 + (y)*x_21 )/A2
#xi_1 = (x2*y3 - x3*y2 + (x-xc)*y_23 + (y-yc)*x_32 )/(A2)
#xi_2 = (x3*y1 - x1*y3 + (x-xc)*y_31 + (y-yc)*x_13 )/(A2)
#xi_3 = (x1*y2 - x2*y1 + (x-xc)*y_12 + (y-yc)*x_21 )/(A2)

w = sym.matrices.Matrix([[
    1 + xi_1 - xi_2,
    1 + xi_2 - xi_3,
    1 + xi_3 - xi_1,
    (xi_1 - xi_2)**2,
    (xi_2 - xi_3)**2,
    (xi_3 - xi_1)**2,
    (xi_1 - xi_2)**3,
    (xi_2 - xi_3)**3,
    (xi_3 - xi_1)**3]])

thx = sym.matrices.zeros((9,1))
thy = sym.matrices.zeros((9,1))

for i in range(9):
    thx[i] = sym.diff(w[i],y)
    thy[i] = sym.diff(-w[i],x)
    
Gp = sym.matrices.Matrix(sp.zeros((9,9)))

# On node 1
i = 0
for exp in w.subs(x,x1).subs(y,y1):
    Gp[0,i] = sym.simplify(exp)
    i +=  1

i = 0
for exp in thx.subs(x,x1).subs(y,y1):
    Gp[1,i] = sym.simplify(exp)
    i +=  1

i = 0
for exp in thy.subs(x,x1).subs(y,y1):
    Gp[2,i] = sym.simplify(exp)
    i +=  1

# On node 2
i = 0
for exp in w.subs(x,x2).subs(y,y2):
    Gp[3,i] = sym.simplify(exp)
    i +=  1

i = 0
for exp in thx.subs(x,x2).subs(y,y2):
    Gp[4,i] = sym.simplify(exp)
    i +=  1

i = 0
for exp in thy.subs(x,x2).subs(y,y2):
    Gp[5,i] = sym.simplify(exp)
    i +=  1

# On node 3
i = 0
for exp in w.subs(x,x3).subs(y,y3):
    Gp[6,i] = sym.simplify(exp)
    i +=  1

i = 0
for exp in thx.subs(x,x3).subs(y,y3):
    Gp[7,i] = sym.simplify(exp)
    i +=  1

i = 0
for exp in thy.subs(x,x3).subs(y,y3):
    Gp[8,i] = sym.simplify(exp)
    i +=  1

print Gp