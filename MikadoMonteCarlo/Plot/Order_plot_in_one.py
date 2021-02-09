import numpy as np
import re
from tkinter import Tk
from tkinter.filedialog import askdirectory
import matplotlib.pyplot as plt
import os

Tk().withdraw()

class Rod:
    def __init__(self, x, y, phi, length, width, domain_w, domain_h):
        self.x = x
        self.y = y
        self.phi = phi
        self.length = length
        self.width = width



def order_parameter(rods):
    angles=np.array([r.phi for r in rods])
    return np.mean(2*np.cos(angles)**2-1)

def plot_order():
    for i in range(len(filenames)):
        with open(filenames[i]+"/order_parameter.txt", 'r') as f:
            #print(f.readlines())
            tmp=np.array([el.split(';') for el in f.readlines()])
            #print(tmp)
            order_p=np.array(tmp[:,0], dtype=float)
            rej_p=np.array(tmp[:,1], dtype=float)
            plt.plot(order_p, label=def_parameter[i][:3])



    #plt.title(r'Order parameter $S_f= \frac{1}{N} \sum_{i=0}^N 2*cos^2(\phi_i)-1$')
    plt.xlabel('timesteps/1000')
    plt.ylabel('order parameter')
    plt.legend()
    plt.savefig(dir+"/comb_order_parameter.png")
    plt.close()
    #plt.show()

    '''
    plt.plot(rej_p[1:])
    plt.xlabel('timesteps/1000')
    plt.ylabel('unchanged rods over plotinterval')
    plt.savefig(filename+"/rejection_parameter.png")
    plt.close()


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
    plt.close()
    #plt.show()'''






filenames=[]
def_parameter=[]
dir = askdirectory(initialdir='../cmake-build-debug/PlotFiles/')
for file in os.listdir(dir):
    if '.png' in file:
        continue
    filenames.append(dir+"/"+file)
    print(filenames[-1])

    try :
        def_parameter.append(re.search(r'(?<=ellipsoidal_potential:)\S*?(?=-)',file).group(0))

    except AttributeError:
        print("attribERROR",file)

plot_order()
