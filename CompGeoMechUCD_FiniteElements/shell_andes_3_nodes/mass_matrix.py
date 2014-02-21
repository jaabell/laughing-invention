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

xi_1 = sym.Symbol("xi_1")
xi_2 = sym.Symbol("xi_2")
xi_3 = 1 - xi_1 - xi_2

x_21 = sym.Symbol("x_21")
x_13 = sym.Symbol("x_13")
x_32 = sym.Symbol("x_32")
y_21 = sym.Symbol("y_21")
y_13 = sym.Symbol("y_13")
y_32 = sym.Symbol("y_32")

x21 = x_21
x13 = x_13
x32 = x_32
y21 = y_21
y13 = y_13
y32 = y_32

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

P =  sym.integrate(sym.integrate(w    ,(xi_2,0,1-xi_1) ),(xi_1,0,1))*2

Mq = sym.integrate(sym.integrate(w.T*w,(xi_2,0,1-xi_1) ),(xi_1,0,1))*2

print P
print Mq

for i in range(9):
    for j in range(9):
        print "Mq({},{}) =  {};".format(i,j,Mq[i,j]*1680)
