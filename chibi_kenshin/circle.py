''' CS 108
Created Fall 2014
circle
@author: Kristofer Brink (kpb23)
'''



''' This file is not yet used'''

import pygame, sys
import math
from pygame.locals import *

class Circle:
    '''
    Models a circle
    '''
    def __init__(self, x, y, radius, color):
        '''
        (int, int, int, pygame.color)-> circle
        '''
        self.x = x
        self.y = y
        self.radius = radius
        self.color = color
        self.xVel = 0
        self.yVel = 0
    
    def draw(self, draw, surface):
        '''
        draws the circle
        '''
        draw.circle(surface, self.color, (int(self.x), int(self.y)), self.radius)
        
    def move(self):
        '''
        moves the circle
        '''
        self.x += self.xVel
        self.y += self.yVel
        # print (self.yVel)
        # print (self.xVel) Just testing for errors
        
    def collidesWith(self, otherCircle):
        ''' 
        circle collision
        '''
        #collision detection using radius and hypotenuse of imaginary triangle of the mid points from the circles
        return(self.radius+otherCircle.radius >= math.sqrt((self.x-otherCircle.x) ** 2 +(self.y-otherCircle.y) ** 2))
    