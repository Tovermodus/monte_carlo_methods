import pygame
import numpy as np
import sys
import re
from tkinter import Tk
from tkinter.filedialog import askdirectory
import matplotlib.pyplot as plt

frame_active = True

pygame.init()

pix_w = 1000# (pix_w - 100)/(pix_h - 100) must be equal to w/h from real domain
pix_h = 1000
screen = pygame.Surface((pix_w,pix_h))


if len(sys.argv) < 2:
    Tk().withdraw()
    filename = askdirectory(initialdir='../cmake-build-debug/PlotFiles/')+'/'
else:
    filename=str(sys.argv[1])
    print(filename)
fileno=re.search(r'(?<=iterations:)\S*?(?=-)',filename).group(0)


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

    for rod in rods:
        rod.draw()
    f.close()




class Rod:
    def __init__(self, x, y, phi, length, width, domain_w, domain_h):
        self.x = x
        self.y = y
        self.phi = phi
        self.length = length
        self.width = width
        self.drawx = 50+x/domain_w*(pix_w - 100) #replace 1.0 with size of domain,
        self.drawy = 50+y/domain_h*(pix_h - 100)
        self.drawlength = length/domain_w*(pix_w - 100)
        self.drawwidth = width/domain_h*(pix_w - 100)
        self.line=self.drawwidth < 2
    def draw(self):
        length_radius_x = np.cos(self.phi)*self.drawlength/2
        length_radius_y = np.sin(self.phi)*self.drawlength/2
        if not self.line:
            vertices = []
            width_radius_x = np.sin(self.phi)*self.drawwidth/2
            width_radius_y = np.cos(self.phi)*self.drawwidth/2
            vertices.append([int(self.drawx + length_radius_x - width_radius_x),
                             pix_h-int(self.drawy + length_radius_y + width_radius_y)])
            vertices.append([int(self.drawx + length_radius_x + width_radius_x),
                             pix_h-int(self.drawy + length_radius_y - width_radius_y)])
            vertices.append([int(self.drawx - length_radius_x + width_radius_x),
                             pix_h-int(self.drawy - length_radius_y - width_radius_y)])
            vertices.append([int(self.drawx - length_radius_x - width_radius_x),
                             pix_h-int(self.drawy - length_radius_y + width_radius_y)])
            pygame.draw.polygon(screen,(0,0,0),vertices)
        else:
            pygame.draw.line(screen,(0,0,0),[int(self.drawx + length_radius_x),
                                             pix_h-int(self.drawy + length_radius_y)],
                             [int(self.drawx - length_radius_x),
                              pix_h-int(self.drawy - length_radius_y)])



font = pygame.font.SysFont(None, 24)

screen.fill((255,255,255))
plot_file(filename+"/"+str(int(fileno)-1)+".txt")


print(filename+'endscreen.png')
pygame.image.save(screen, filename+'/endscreen.png')