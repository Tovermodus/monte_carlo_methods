import numpy as np
import sys
import re
from tkinter import Tk
from tkinter.filedialog import askdirectory
import matplotlib.pyplot as plt




def order_parameter(rods):
    angles=np.array([r.phi for r in rods])
    return np.mean(2*np.cos(angles)**2-1)

if len(sys.argv) < 2:
    Tk().withdraw()
    filename = askdirectory(initialdir='../cmake-build-debug/PlotFiles/')
else:
    filename=str(sys.argv[1])

#print(filename)
#print(re.search(r'(?<=iterations:)\S*?(?=-)',filename))
fileno=re.search(r'(?<=iterations:)\S*?(?=-)',filename).group(0)
open(filename+"/order_parameter.txt", 'w').close()

def plot_order():
    with open(filename+"/order_parameter.txt", 'r') as f:
        order_p=np.array(f.readlines(), dtype=float)
        plt.plot(order_p)
        plt.title(r'Order parameter $S_f= \frac{1}{N} \sum_{i=0}^N 2*cos^2(\phi_i)-1$')
        plt.xlabel('timesteps')
        plt.ylabel('order parameter')
        plt.savefig(filename+"/order_parameter.png")
        plt.show()

def plot_file(name):
    rods = []
    f = open(name)
    lines = f.readlines()
    whline = lines[0][:-1]
    ws,hs =whline.split(' ')
    dh = float(hs)
    dw = float(ws)
    lwline = lines[1][:-1]
    ws,ls =lwline.split(' ')
    l = float(ls)
    w = float(ws)
    for i in range(2,len(lines)):
        x,y,phi = lines[i].split(' ')
        rods.append(Rod(float(x),float(y),float(phi),l,w,dw,dh))

    with open(filename+"/order_parameter.txt", 'a') as f:
        f.write(str(order_parameter(rods))+"\n")
    f.close()




class Rod:
    def __init__(self, x, y, phi, length, width, domain_w, domain_h):
        self.x = x
        self.y = y
        self.phi = phi
        self.length = length
        self.width = width

for i in range(int(fileno)):
    plot_file(filename+"/"+str(i)+".txt")

plot_order()