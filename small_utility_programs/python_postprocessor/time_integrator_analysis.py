from scipy import *
from scipy.interpolate import interp1d
from scipy.optimize import bisect
import scipy.linalg as lin

#Simple peak finder for these very smooth signals
def findpeaks(y, imax = -1):
    N = y.shape[0]
    if imax == -1:
        imax = N
    #print N
    i = 1
    peak_indices = []
    while (i < N-1):
        if( (y[i-1] < y[i]) and (y[i+1] <  y[i]) ):
            peak_indices.append(i)
        #if( (y[i-1] > y[i]) and (y[i+1] >  y[i]) ):
        #    peak_indices.append(i)
        i += 1
        if (i > imax):
            break
    return peak_indices

#Damping finder based on the half-power method
def measure_damping(f,Y):
    xi = []
    fs = []
    Ys = []
    N = Y.shape[0]
    i_pks = findpeaks(abs(Y), ceil(N/2))
    Yfun = interp1d(f,abs(Y),fill_value = inf, bounds_error = False)
    for i in i_pks:
        if f[i] > 10:
            break
        Ymax = abs(Y[i])
        Y3db = Ymax / sqrt(2.)
        #print "Ymax = {}, Y3db = {}".format(Ymax, Y3db)

        f0 = f[i]
        j1 = 1
        while(abs(Y[i - j1]) > Y3db and i - j1 > 1):
            j1 += 1
        j2 = 1
        while(abs(Y[i + j2]) > Y3db and i + j2 < N-1):
            j2 += 1


        if i - j1 < 0:
            continue
        try:
            f1 = bisect(lambda x: Yfun(x) - Y3db, f[i-j1], f[i])
            #print "i = {}, f1 = {}".format(i, f1)
        except ValueError:
            #print "i = {}, j1 = {}, f(a) = {}, f(b) = {}".format(i, j1, f[i-j1], f[i])
            continue


        if i + j2 >= N:
            continue
        try:
            f2 = bisect(lambda x: Yfun(x) - Y3db, f[i], f[i+j2])
            #print "i = {}, f2 = {}".format(i, f2)
        except ValueError:
            #print "i = {}, j2 = {}, f(a) = {}, f(b) = {}".format(i, j2, f[i+j2], f[i])
            continue

        df = f2 - f1

        dampingratio = df / f0 / 2.
        if dampingratio < .5:
            xi.append( dampingratio )
            fs.append(f0)
            Ys.append(Ymax)

    return xi, fs, Ys

def spectral_radius_hht(beta, gamma, alpha, w, dt):
    a = alpha
    b = beta
    g = gamma

    OM = w*dt
    OM2 = OM**2
    D = 1. + (1+a)*b*OM2
    A = 1/D*array([[   1 + a*b*OM2, 1                   , 0.5 - b                    ],
                   [  -g*OM2      , 1 - (1+a)*(g-b)*OM2 , 1-g-(1+a)*(0.5*g-b)*OM2    ],
                   [  -OM2        , -(1+a)*OM2          , -(1+a)*(.5-b)*OM2          ]])
    lambdas = lin.eigvals(A)
    
    return lambdas
    
# This was wrong.... 
#   the damping and period shits are dominated by the complex-conjugate lambdas... not the one with the
# largest absval.
#    imax = 0
#    for i in range(3):
#        if abs(lambdas[i]) > abs(lambdas[imax]):
#            imax = i
#    return lambdas[imax]


def hht_damping_and_shift(beta, gamma, alpha, w, dt, diftol = 1e-10):
    
    l = spectral_radius_hht(beta, gamma, alpha, w, dt)
    
    d01 = abs(abs(l[0]) - abs(l[1]))
    d02 = abs(abs(l[0]) - abs(l[2]))
    d12 = abs(abs(l[1]) - abs(l[2]))
    
    lam1 = l[0]
    # l[0] is the spurious root
    if d01 > d12 and d02 >  d12 and d12 <  diftol:
        lam1 = l[1]
    # l[1] is the spurious root
    elif d01 > d02 and d12 >  d02 and d02 <  diftol:
        lam1 = l[0]
    # l[2] is the spurious root
    elif d02 > d01 and d12 >  d01 and d01 <  diftol:
        lam1 = l[0]
    else:
        print "Strange case. Returning results based on first eigenvalue."
        print "d01 = {}, d02 = {}, d12 = {}".format(d01, d02, d12)
        print l
    
    A = real(lam1)
    B = (imag(lam1))
    
    OMBAR = arctan(B/A)
    
    wbar = OMBAR/dt
    
    xibar = -log(A**2 + B**2)/(2*OMBAR)
    
    return wbar, xibar
    