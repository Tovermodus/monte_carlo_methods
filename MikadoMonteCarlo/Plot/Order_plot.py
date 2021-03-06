import numpy as np
import sys
import re
from tkinter import Tk
from tkinter.filedialog import askdirectory
import matplotlib.pyplot as plt



class Rod:
    def __init__(self, x, y, phi, length, width, domain_w, domain_h):
        self.x = x
        self.y = y
        self.phi = phi
        self.length = length
        self.width = width



old_rods = [Rod(0, 0, 0, 0, 0, 0, 0 )]



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
        #print(f.readlines())
        tmp=np.array([el.split(';') for el in f.readlines()])
        #print(tmp)
        order_p=np.array(tmp[:,0], dtype=float)
        rej_p=np.array(tmp[:,1], dtype=float)
        plt.plot(order_p)
        #plt.title(r'Order parameter $S_f= \frac{1}{N} \sum_{i=0}^N 2*cos^2(\phi_i)-1$')
        plt.xlabel('timesteps/1000')
        plt.ylabel('order parameter')
        plt.savefig(filename+"/order_parameter.png")
        plt.show()

        plt.plot(rej_p[1:])
        plt.xlabel('timesteps/1000')
        plt.ylabel('unchanged rods over plotinterval')
        plt.savefig(filename+"/rejection_parameter.png")
        plt.show()

        fig, ax = plt.subplots(2, sharex='col')
        ax[0].plot(order_p[1:])
        #ax[0].set_title(r'Order parameter $S_f= \frac{1}{N} \sum_{i=0}^N 2*cos^2(\phi_i)-1$', y=1.05)
        ax[0].set_xlabel('timesteps/1000')
        ax[0].set_ylabel('order parameter')
        ax[1].plot(rej_p[1:])
        #ax[1].set_title(r'rejection rate estimation')
        ax[1].set_xlabel('timesteps/1000')
        ax[1].set_ylabel('unchanged rods over plotinterval')
        fig.set_size_inches(10,10)
        plt.savefig(filename+"/combined.png")
        plt.show()

def plot_file(name):
    rods = []

    global old_rods
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
    same_count=0
    for i in range(2, len(lines)):
        x, y, phi = lines[i].split(' ')
        x, y, phi =float(x), float(y), float(phi)
        if len(old_rods) > 1:
            if (x == old_rods[i-2].x) and (y == old_rods[i-2].y) and (phi == old_rods[i-2].phi):
                same_count += 1
        rods.append(Rod(x,y,phi,l,w,dw,dh))


    with open(filename+"/order_parameter.txt", 'a') as f:
        f.write(str(order_parameter(rods))+';'+str(same_count)+"\n")
    f.close()
    #print(len(old_rods),len(rods))
    old_rods = rods

def print_last_time_avg_rej():
    with open(filename+"/order_parameter.txt", 'r') as f:
        #print(f.readlines())
        tmp=np.array([el.split(';') for el in f.readlines()])
        #print(tmp)
        order_p=np.array(tmp[:,0], dtype=float)
        rej_p=np.array(tmp[:,1], dtype=float)
        avg_rej_p = rej_p[:-5]+rej_p[4:-1]+rej_p[3:-2]+rej_p[2:-3]+rej_p[1:-4]+rej_p[5:]
        plt.plot(avg_rej_p)
        plt.show()
        m = np.max(avg_rej_p)
        print(filename)
        for i in range(len(avg_rej_p)-1,0,-1):
            if avg_rej_p[i] < m*0.9:
                print(i)
                break
        for i in range(len(avg_rej_p)-1,0,-1):
            if avg_rej_p[i] < m*0.95:
                print(i)
                break
        for i in range(len(avg_rej_p)-1,0,-1):
            if avg_rej_p[i] < m*0.99:
                print(i)
                break



for i in range(int(fileno)):
    plot_file(filename+"/"+str(i)+".txt")

plot_order()
print_last_time_avg_rej()