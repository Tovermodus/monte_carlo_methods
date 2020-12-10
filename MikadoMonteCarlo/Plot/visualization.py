import pygame
import numpy as np
frame_active = True

pygame.init()

pix_w = 1000# (pix_w - 100)/(pix_h - 100) must be equal to w/h from real domain
pix_h = 1000
screen = pygame.display.set_mode((pix_w,pix_h))
clock = pygame.time.Clock()
pygame.display.set_caption("Data Visualisation")

def plot_file(name):
    global frame_active
    rods = []
    print(name)
    f = open(name)
    lines = f.readlines()
    lwline = lines[1][:-1]
    ws,ls =lwline.split(' ')
    l = float(ls)
    w = float(ws)
    for i in range(2,len(lines)):
        x,y,phi = lines[i].split(' ')
        #print(x,y,phi)
        rods.append(Rod(float(x),float(y),float(phi),l,w))



    for event in pygame.event.get():
        if event.type== pygame.QUIT:
            frame_active = False
    screen.fill((255,255,255))
    for rod in rods:
        rod.draw()
    pygame.display.flip()
    clock.tick(20)




class Rod:
    def __init__(self, x, y, phi, length, width):
        self.x = x
        self.y = y
        self.phi = phi
        self.length = length
        self.width = width
        self.drawx = 50+x/1.0*(pix_w - 100) #replace 1.0 with size of domain,
        self.drawy = 50+y/1.0*(pix_h - 100)
        self.drawlength = length/1.0*(pix_w - 100)
        self.drawwidth = width/1.0*(pix_w - 100)
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
    for i in range(int(10000/20)):
        if frame_active == False:
            break
        plot_file("../cmake-build-debug/PlotFiles/"+str(i)+".txt")