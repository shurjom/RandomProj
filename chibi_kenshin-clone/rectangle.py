''' CS 108
Created Fall 2014
Rectangle for game moving objects
@author: Kristofer Brink (kpb23)
'''

import pygame, sys
import math
from pygame.locals import *


class Rectangle:
    '''
    Models a Rectangle
    Invariants:
        width is int
        height is int
        stair_climb is int
    '''
    def __init__(self, x, y, width, height, stair_climb = 20):
        '''
        (int, int, int, int, int) -> Rectangle
        Constructor
        '''
        self.rect = pygame.Rect(x, y, width, height)  # Make rectangle using pygame's built in function
        self.dx = 0
        self.dy = 0
        self.store_past_position = True # Used to not store the past x and y as a place in a block
        
        # Variables to determine movement
        self.side_touch = ''
        self.side_touch_past = ''
        self.above_bottom = False
        self.slope_ratio = 1
        self.stair_climb = stair_climb
        
        # Block properties
        self.wall_friction = 1
        self.wall_jump_up = 1
        self.wall_jump_sideways = 1
        
        self.ground_friction = 1
        self.jump = 1

    def _modify_wall_properties(self, block_object):
        '''Modifys the rectangles properties relative to a block_object when on a wall'''
        self.wall_friction = block_object.wall_friction
        self.wall_jump_up = block_object.wall_jump_up
        self.wall_jump_sideways = block_object.wall_jump_sideways
        
    def _modify_ground_properties(self, block_object):
        '''Modifys the rectangles properties relative to block_object on the ground'''
        self.ground_friction = block_object.ground_friction
        self.jump = block_object.jump
       
    def zero_out_dx(self):
        '''Zero out the speed when it is close enough to 0'''
        zero_out = .001
        if -zero_out < self.dx < zero_out:
            self.dx = 0
        
    def move(self):
        '''Moves the block and then checks for collision after movement'''
        # zero out dx if close enough to 0
        self.zero_out_dx()
        # Store past x and y for collision response
        self.past_x = self.rect.x
        self.past_y = self.rect.y
        
        # Variables to determine movement reseting area
        self.side_touch_past = self.side_touch
        self.side_touch = ''
        
        self.above_bottom = False
        
    
        self.rect.x += self.dx    
        self.rect.y += self.dy
        
        
    def collision(self, block):
        ''' 
        Checks for collision with a block
        (pygame.rect) -> bool
        '''
        return self.rect.colliderect(block)

    
            
    def _top_coll(self, past_rect, block, block_object, force_coll=False):
        '''
        Top collision for rectangle colliding with block
        (pygame.rect, pygame.rect, Block, bool) -> Collision system
        '''
        if past_rect.bottom <= block.top or force_coll: # Hit the top side of the block
            self.rect.y += (block.top - self.rect.bottom) // block_object.parallax
            self.dy = 0 if block_object.parallax == 1 else 1
            self.side_touch = 'Top'
            self._modify_ground_properties(block_object)
            return True
        return False
            
    def _bot_coll(self, past_rect, block, force_coll=False):
        '''
        Bottom collision for rectangle colliding with block
        (pygame.rect, pygame.rect, Block, bool) -> Collision system
        '''
        if past_rect.top  >= block.bottom: #  Hit the bottom side of the block
            self.rect.y = block.y + block.height
            self.dy = 0
            self.side_touch = 'Bottom'
            return True
        return False
    
    def _up_stairs(self, past_rect, block, block_object):
        '''
        Makes rectangle able to go upstairs that are the height of the rectangle's stair climb
        (pygame.rect, pygame.rect, Block) -> bool
        '''
        if self.rect.bottom - self.stair_climb < block.top and (self.side_touch.startswith('Top') or self.side_touch_past.startswith('Top')):
            
            step_height = self.rect.bottom - block.top
            if step_height > 5:
                self.dx /= 1 + step_height/5
            self._top_coll(past_rect, block, block_object, force_coll=True)
            return True
        return False
            
    def _right_coll(self, past_rect, block, block_object, force_coll=False):
        '''
        Right collision for rectangle relative to block
        (pygame.rect, pygame.rect, Block, bool) -> bool
        '''
        if past_rect.right <= block.left: # Hit the left side of block
            if self._up_stairs(past_rect, block, block_object):
                return
            
            self.rect.x += (block.left - self.rect.right)//block_object.parallax
            self.side_touch = 'Left' if (self.side_touch != 'Top') else 'Top'
            
            self.dx = 0
            
            self._modify_wall_properties(block_object)
            return True
        return False
            
    def _left_coll(self, past_rect, block, block_object, force_coll=False):
        '''
        Left collision for rectangle relative to block
        (pygame.rect, pygame.rect, Block, bool) -> bool
        '''
        
        if past_rect.left >= block.right: # Hit the right side of the block
            if self._up_stairs(past_rect, block, block_object):
                return
        
            self.rect.x += (block.right - self.rect.left)//block_object.parallax
            self.side_touch = 'Right' if (self.side_touch != 'Top') else 'Top'
            
            self.dx = 0
            
            self._modify_wall_properties(block_object)
            return True
        return False
            
    def _collision_with_square(self, block, block_object, past_rect):
        '''
        Rectangle collision response to a block that starts with type Block
        (pygame.rect, pygame.rect, Block, bool) -> bool
        '''
        # seems really complicated but just doing this because of parallax
        if (block_object.type == 'Block') or (block_object.type == 'Block Invisible'):
            if not (self._bot_coll(past_rect, block) 
                    or self._right_coll(past_rect, block, block_object) 
                    or self._left_coll(past_rect, block, block_object)
                    or self._top_coll(past_rect, block, block_object)):
                if self.side_touch_past == 'Top':
                    self._top_coll(past_rect, block, block_object, force_coll=True)
                elif self.side_touch_past == 'Left':
                    self._left_coll(past_rect, block, block_object, force_coll=True)
                elif self.side_touch_past == ' Right': 
                    self._right_coll(past_rect, block, block_object, force_coll=True)
                elif self.side_touch_past == 'Bottom':
                    self._right_coll(past_rect, block, block_object, force_coll=True)
                else:
                    self._top_coll(past_rect, block, block_object, force_coll=True)
            return
        
        if block_object.type == 'Block Top':
            self._top_coll(past_rect, block, block_object)
        
        if block_object.type == 'Block Bottom':
            self._bot_coll(past_rect, block)
            
        if block_object.type == 'Block Right':
            self._right_coll(past_rect, block, block_object)
            
        if block_object.type == 'Block Left':
            self._left_coll(past_rect, block, block_object)
                
            
            
  
    def _collision_with_triangle(self, block, block_object, past_rect):
        '''
        Collision with triangle
        (pygame.rect, Block, pygame.rect)
        '''
        
        # This handles the part of triangles that are like a rectangle
        if block_object.x < block_object.x2:
            if self._left_coll(past_rect, block, block_object):
                return
        else:
            if self._right_coll(past_rect, block, block_object):
                return
        if block_object.y < block_object.y2:
            self._top_coll(past_rect, block, block_object)
        else:
            self._bot_coll(past_rect, block)
            
            
        # This handles the part of triangles with slopes
        
        if self.collision(block): 
            '''
            Failed
            first_corner = pygame.Rect(block_object.x, block_object.y+1, 1, 0)
            second_corner = pygame.Rect(block_object.x2, block_object.y2+1, 1, 0)
            if self.collision(first_corner):
                self._collision_with_square(first_corner, block_object, past_rect)
            if self.collision(second_corner):
                self._collision_with_square(second_corner, block_object, past_rect)
            '''
            
            
            '''Slope of triangle'''
            if block_object.x < block_object.x2: # Triangle slope //// and upside down
                
                corner = pygame.Rect(block_object.x, block_object.y, 0, 0)
                
                if self.collision(corner) and self._right_coll(past_rect, corner, block_object):
                    
                    self.side_touch = 'Right'
                    self.dx = 0
                    return
                
                if block_object.y > block_object.y2: # specifically for /// upside right slope  part
                    x, y = self.rect.bottomright
                    
                    if block.left <= x <= block.right:
                        x_difference = x - block.left # need it to calculate y 
                        slope = (block.top - block.bottom) / ( block.left - block.right)
                        y_place = -(x_difference * slope) + block.bottom
                        
                        if y > y_place:
                            self._modify_ground_properties(block_object)
                            self.rect.bottom = y_place
                            self.side_touch = 'Top Slope Left'
                            self.dy = 0 if self.dy > 0 else self.dy
                            self.slope_ratio = slope + 1 # ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ slope
                            return
                            
                    elif block.right < x:
                        self.dy = 0 if self.dy > 0 else self.dy 
                        self.rect.bottom = block.top
                        self.side_touch = 'Top'
                        return
                    
                else: # specifically for /// upside down triangle slope part
                    x, y = self.rect.topright
                    
                    if block.left <= x <= block.right:
                        x_difference = x - block.left
                        slope = (block.bottom - block.top) / (block.left - block.right)
                        y_place = -(x_difference * slope) + block.top
                        
                        if y < y_place and self.side_touch != 'Top':
                            self.rect.top = y_place
                            self.side_touch = 'Bottom'
                            self.dy = 2
                            self.dx /= 2
                            return
                        
                    elif block.right < x:
                        self.dy = 0
                        self.rect.top = block.bottom
                        self.side_touch = 'Bottom'
                        return
                    
                    
            if block_object.x > block_object.x2: # Triangle slope |\\\\

                corner = pygame.Rect(block_object.x, block_object.y, 0, 0)
                
                if self.collision(corner) and self._left_coll(past_rect, corner, block_object): # corner
                    
                    self.side_touch = 'Left'
                    self.dx = 0
                    return
                    
                if block_object.y > block_object.y2: # upside right \\\\\ slope
                    x, y = self.rect.bottomleft
                     
                    if block.left <= x <= block.right:
                        x_difference = x - block.right
                        slope = (block.top - block.bottom) / (block.right - block.left)
                        y_place = -(x_difference * slope) + block.bottom
                        
                        if y > y_place:
                            self._modify_ground_properties(block_object)
                            self.rect.bottom = y_place
                            self.side_touch = 'Top Slope Right'
                            self.dy = 0 if self.dy > 0 else self.dy 
                            self.slope_ratio = math.fabs(slope - 1) # ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ slope
                            return
                            
                    elif block.left > x:
                        self.dy = 0 if self.dy > 0 else self.dy 
                        self.rect.bottom = block.top
                        self.side_touch = 'Top'
                        return
                else: # upside down slope \\\\\\\\\
                    x, y = self.rect.topleft
                    
                    if block.left <= x <= block.right:
                        x_difference = x - block.right
                        slope = (block.bottom - block.top) / (block.right - block.left)
                        y_place = -(x_difference * slope) + block.top
                        
                        if y < y_place and self.side_touch != 'Top':
                            self.rect.top = y_place
                            self.side_touch = 'Bottom'
                            self.dy = 2
                            self.dx /= 2
                            return
                    
                    elif block.left > x:
                        self.dy = 0
                        self.rect.top = block.bottom
                        self.side_touch = 'Bottom'
                        return
        
                
    
    def block_collision(self, block_objects, game): # collision detection and response. Made by Kristofer who had no idea what he was doing
        '''
        Block collision handling
        (list[Block], game)
        '''
        for block_object in block_objects:
            # Take into account parallax
            
            position = game.camera.parallax_pos((block_object.rect.x, block_object.rect.y), block_object.parallax)
            
            block = pygame.Rect(position[0], position[1], block_object.rect.width, block_object.rect.height)
            
            if self.rect.colliderect(block): # If collide then resolve
                past_rect = pygame.Rect(self.past_x, self.past_y, self.rect.width, self.rect.height)
                
                
                if block_object.type.startswith('Block'):
                    self._collision_with_square(block, block_object, past_rect)
                    
                if (block_object.type == 'Triangle') or (block_object.type == 'Line'):
                    self._collision_with_triangle(block, block_object, past_rect)
                
                if self.rect.bottom < block.bottom: # wall jumping was broken this fixes it. Hard to explain without a picture
                    self.above_bottom = True
                    
                
                        

        
        
        
        
        
        
# Fail attempts at collision for memory of failures may they rest in piece
        
''' This worked before but I had to change it to accomodate the triangles that I did not know I would add
  def block_collision(self, block_objects, game): # collision detection and response. Made by Kristofer who had no idea what he was doing
        for block_object in block_objects:
            # Take into account parallax
            position = game.camera.input_xy((block_object.rect.x, block_object.rect.y), block_object.parallax)
            
            block = pygame.Rect(position[0], position[1], block_object.width, block_object.height)
            if self.rect.colliderect(block): # If collide then resolve
                if block_object.triangle == False:
                    self.store_past_position = False
                    past_rect = pygame.Rect(self.past_x, self.past_y, self.rect.width, self.rect.height)
                    
                    if past_rect.right <= block.left: # Hit the left side of block
                        self.rect.x = block.left - self.rect.width
                        self.side_touch = 'Left' if (self.side_touch != 'Top') else 'Top'
                    if past_rect.left >= block.right: # Hit the right side of the block
                        self.rect.x = block.x + block.width
                        self.side_touch = 'Right' if (self.side_touch != 'Top') else 'Top'
                    if past_rect.bottom <= block.top: # Hit the top side of the block
                        self.rect.y = block.top - self.rect.height
                        self.dy = 0
                        self.side_touch = 'Top'
                    if past_rect.top  >= block.bottom: #  Hit the bottom side of the block
                        self.rect.y = block.y + block.height
                        self.dy = 0
                        self.side_touch = 'Bottom'
                    if self.rect.bottom < block.bottom:
                        self.above_bottom = True



self.max = max([self.past_dx, self.past_dy])

while self.rect.colliderect(block): This was just dumb to begin with
    if(self.max != 0):
        self.rect.x -= self.past_dx/10
        self.rect.y -=self.past_dy/10

'''
'''
if self.rect.colliderect(block): wow so fail
    self.past_x = self.rect.x - self.past_dx
    self.past_y = self.rect.y - self.past_dy
    self.past_dx_direction = -1 if (self.past_dx > 0) else 1 if (self.past_dx != 0) else 0
    self.past_dy_direction = 1 if (self.past_dy > 0) else -1 if (self.past_dy != 0) else 0
    # if self.past_x
    for i in range(50):
        if self.rect.colliderect(block):
            self.rect.x -= self.past_dx_direction
            self.rect.y -= self.past_dy_direction
        else:
            break
'''