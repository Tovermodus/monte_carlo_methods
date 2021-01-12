import sys

import pygame
import numpy as np
import matplotlib.pyplot as plt
frame_active = True

pygame.init()

pix_w = 1000# (pix_w - 100)/(pix_h - 100) must be equal to w/h from real domain
pix_h = 1000
progressbar_height = 10
timesteps = int(sys.argv[1])
screen = pygame.display.set_mode((pix_w,pix_h))
clock = pygame.time.Clock()
pygame.display.set_caption("Data Visualisation")
open("../cmake-build-debug/PlotFiles/order_parameter.txt", 'w').close()
runthrough=False

def order_parameter(rods):
    angles=np.array([r.phi for r in rods])
    return np.mean(2*np.cos(angles)**2-1)

def plot_order():
    with open("../cmake-build-debug/PlotFiles/order_parameter.txt", 'r') as f:
        order_p=np.array(f.readlines(), dtype=float)
        plt.plot(order_p)
        plt.title(r'Order parameter $S_f= \frac{1}{N} \sum_{i=0}^N 2*cos^2(\phi_i)-1$')
        plt.xlabel('timesteps')
        plt.ylabel('order parameter')
        plt.savefig("../cmake-build-debug/PlotFiles/order_parameter.png")
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
        with open("../cmake-build-debug/PlotFiles/order_parameter.txt", 'a') as f:
            f.write(str(order_parameter(rods))+"\n")


    for event in pygame.event.get():
        if event.type== pygame.QUIT:
            frame_active = False
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
    for i in range(int(timesteps)):
        if frame_active == False:
            break

        screen.fill((255,255,255))
        plot_file("../cmake-build-debug/PlotFiles/"+str(i)+".txt")

        vertices = [[0,0],[pix_w,0],[pix_w,progressbar_height],[0,progressbar_height]]
        pygame.draw.polygon(screen, (255,0,0),vertices)
        vertices = [[0,0],[int(pix_w*(i+1)/timesteps),0],[int(pix_w*(i+1)/timesteps),progressbar_height],[0,progressbar_height]]
        pygame.draw.polygon(screen, (0,255,0),vertices)

        pygame.display.flip()
        clock.tick(100)

    if not  runthrough:
        plot_order()
    runthrough=True
    #if runthrough==0:
    #    runthrough=1