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
screen = pygame.display.set_mode((pix_w,pix_h))
clock = pygame.time.Clock()
fps = 50
pygame.display.set_caption("Data Visualisation")
runthrough=False

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
    global frame_active
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
        #print(x,y,phi)
        rods.append(Rod(float(x),float(y),float(phi),l,w,dw,dh))

    if not runthrough:
        with open(filename+"/order_parameter.txt", 'a') as f:
            f.write(str(order_parameter(rods))+"\n")


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

while frame_active:
    font = pygame.font.SysFont(None, 24)
    img = font.render(str(fps)+' frames per second', True, (0,0,0))
    screen.blit(img, (0, 0))

    for i in range(int(fileno)):
        fpsrange = fps//30
        if fpsrange > 1:
            if i%fpsrange != 0:
                #plot_file(filename+"/"+str(i)+".txt")
                continue
        ev = pygame.event.get()
        for event in ev:
            if event.type== pygame.QUIT:
                frame_active = False
            if event.type == pygame.MOUSEBUTTONUP:
                pos = pygame.mouse.get_pos()
                if(pos[1] < 30):
                    fps = int(300*(pos[0] + 2*max(0,pos[0]-300))/pix_w)
                    img = font.render(str(fps)+' frames er second', True, (0,0,0))
        if frame_active == False:
            break
        screen.fill((255,255,255))
        plot_file(filename+"/"+str(i)+".txt")
        vertices = [[0,0],[pix_w,0],[pix_w,30],[0,30]]
        pygame.draw.polygon(screen, (255,0,0),vertices)
        vertices = [[0,0],[int(pix_w*(i+1)/int(fileno)),0],[int(pix_w*(i+1)/int(fileno)),30],[0,30]]
        pygame.draw.polygon(screen, (0,255,0),vertices)
        screen.blit(img, (0, 0))

        pygame.display.flip()

        clock.tick(min(fps,60))

    if not  runthrough:
        plot_order()
    runthrough=True
    #if runthrough==0:
    #    runthrough=1