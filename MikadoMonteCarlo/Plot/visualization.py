import pygame
import numpy as np
import sys
from tkinter.filedialog import askdirectory

frame_active = True

pygame.init()

pix_w = 1000# (pix_w - 100)/(pix_h - 100) must be equal to w/h from real domain
pix_h = 1000
screen = pygame.display.set_mode((pix_w,pix_h))
clock = pygame.time.Clock()
pygame.display.set_caption("Data Visualisation")

if len(sys.argv)>0:
    filename = askdirectory()
else:
    filename=str(argv[0])

def plot_file(name):
    global frame_active
    rods = []
    print(name)
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



    for event in pygame.event.get():
        if event.type== pygame.QUIT:
            frame_active = False
    screen.fill((255,255,255))
    for rod in rods:
        rod.draw()
    pygame.display.flip()
    clock.tick(20)




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
    for i in range(int(1000/20)):
        if frame_active == False:
            break
        plot_file(filename+"/"+str(i)+".txt")