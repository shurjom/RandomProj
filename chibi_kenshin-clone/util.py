''' CS 108
Created Fall 2014
util
@author: Kristofer Brink (kpb23)
'''


import pygame, sys
import math
from pygame.locals import *

 
def load(image_location, alpha=True):
    '''
    Loads an image from a location and auto does alpha unless if specified otherwise
    (string, bool) -> pygame surface
    '''
    try:
        surface = pygame.image.load('images/' + image_location)
        if alpha: # http://www.pygame.org/docs/ref/surface.html#pygame.Surface.convert
            surface = surface.convert_alpha()
        else:
            surface = surface.convert()
        return surface
    except:
        # If image cannot load instead make it a surf that shows the user what image was not able to load
        print('Cannot Load image from path:', image_location)
        
        myfont = pygame.font.SysFont("monospace", 16)
        return myfont.render("Couldn't load " + image_location, 1, (0,0,0))         


def center(screen_length, image_length):
    return screen_length//2 - image_length//2

def draw_center(surface, screen, draw, width, height, x=0, y=0):
    screen_width, screen_height = screen.get_size()
    x = center(screen_width, width) + x 
    y = center(screen_height, height) + y
    screen.blit(surface, (x,y))

def triangle_collide(block_obj, delete_rect):
    '''
    For deleting a triangle
    (Block, pygame.rect)
    '''
    half_size = delete_rect[2]/2
    x, y = (delete_rect[0] + half_size, delete_rect[1] + half_size)
    block = block_obj.rect
    if (block_obj.x < block_obj.x2 and block_obj.y > block_obj.y2) or (block_obj.x > block_obj.x2 and block_obj.y < block_obj.y2): # for / and the opposite of what would fill in a rectangle
        x_difference = x - block.left # need it to calculate y 
        try:
            slope = (block.top - block.bottom) / ( block.left - block.right)
        except:
            return True
        y_place = -(x_difference * slope) + block.bottom
    
    else: # for \ both triangle that when combined would make a rectangle
        x_difference = x - block.right
        try:
            slope = (block.top - block.bottom) / (block.right - block.left)
        except:
            return True
        y_place = -(x_difference * slope) + block.bottom
        
    if y > y_place and (block_obj.y > block_obj.y2):
        return True
    elif y < y_place and (block_obj.y < block_obj.y2):
        return True
    return False