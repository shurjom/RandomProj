import pygame, sys
import math
from pygame.locals import *
import circle

class player:
    
    def __init__(self, x, y, radius, color):
        self.circle = circle.circle(x,y,radius, color)
        
    def input(self, up, down, left, right):
        #Up and down movement
        playerSpeed = .6
        playerDecelerate = 1.25
        if up and not down:
            self.circle.yVel -= playerSpeed
        elif down and not up:
            self.circle.yVel += playerSpeed
        else:
            self.circle.yVel /= playerDecelerate
        #Left and right movement
        if right and not left:
            self.circle.xVel += playerSpeed
        elif left and not right:
            self.circle.xVel -= playerSpeed
        else:
            self.circle.xVel /= playerDecelerate
            
    def vectorSpeedConstant(self):
        if self.circle.xVel**2 < 0.1 and self.circle.yVel**2 < 0.1:
            self.circle.xVel = 0
            self.circle.yVel = 0
            return
        ratio = 10.0/math.sqrt(self.circle.xVel ** 2 + self.circle.yVel ** 2)
        if ratio < 1:
            self.circle.xVel *= ratio
            self.circle.yVel *= ratio
            
    def playerLogic(self, up, down, left, right):
        self.input(up, down, left, right)
        self.vectorSpeedConstant()
        self.circle.move()
            
