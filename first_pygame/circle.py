import pygame, sys
import math
from pygame.locals import *

class circle:
    
    def __init__(self, x, y, radius, color):
        self.x = x
        self.y = y
        self.radius = radius
        self.color = color
        self.xVel = 0
        self.yVel = 0
    
    def draw(self, draw, surface):
        draw.circle(surface, self.color, (int(self.x), int(self.y)), self.radius)
        draw.circle(surface, pygame.Color(255, 255, 255), (int(self.x), int(self.y)), 5)
        
    def move(self):
        self.x += self.xVel
        self.y += self.yVel
        
    def collidesWith(self, otherCircle):
        #collision detection using radius and hypotenuse of imaginary triangle of the mid points from the circles
        return(self.radius+otherCircle.radius >= math.sqrt((self.x-otherCircle.x) ** 2 +(self.y-otherCircle.y) ** 2))
    