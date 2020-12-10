import pygame
import numpy as np

pygame.init()

pix_w = 1000# (pix_w - 100)/(pix_h - 100) must be equal to w/h from real domain
pix_h = 1000
screen = pygame.display.set_mode((pix_w,pix_h))
clock = pygame.time.Clock()
pygame.display.set_caption("Data Visualisation")

frame_active = True

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
        print(self.drawx,self.drawy,self.drawlength,self.drawwidth, self.phi)
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

rods = []
l = 0.3
w = 0.05
#for i in range(30):
#    rods.append(Rod(0.03*i+0*np.random.rand(),np.random.rand(),0.1*i+0*np.random.rand()*np.pi,l,w)) # should instead be read from file
rods.append(Rod(0.5, 0.5, 1.4, l, w))
#rods.append(Rod(0.4,0.4,0.4,l,w))
rods.append(Rod(0.6, 0.75, 1.6, l, w))
rods.append(Rod(0.5, 0.75, 1.6, l, w))


while frame_active:
    for event in pygame.event.get():
        if event.type== pygame.QUIT:
            frame_active = False
    screen.fill((255,255,255))
    for rod in rods:
        rod.draw()
    pygame.display.flip()
    clock.tick(30)