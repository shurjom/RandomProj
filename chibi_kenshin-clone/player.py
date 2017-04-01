''' CS 108
Created Fall 2014
Player
@author: Kristofer Brink (kpb23)
'''


import pygame, sys
import math
from pygame.locals import *
import rectangle

# cap function
def cap(capped, speed):
    '''Caps speed
    (int, int) -> int
    '''
    if speed > capped:
        speed = capped
    if speed < -capped:
        speed = - capped
    return speed


class Player:
    ''' 
    Player models a player in the game, this can be used for making enemies
    '''
    def __init__(self, x, y, width, height, # rectangle
                 ground_accelerate, ground_decelerate, # Ground movement
                 air_acclerate, air_decelerate, air_antigravity, air_downards, # Air movement
                 wall_decelerate, wall_jump_dy, wall_jump_dx, # Wall movement
                 jump_speed, # Jump speed
                 capped_dx_ground, capped_dx_air, # Capped dx for ground and air
                 capped_dx, capped_dy, # Capped dx
                 gravity # Gravity on the player
                 ):
        '''
        (int, int , int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) -> Player
        '''
        
        self.origin = (x,y)
        
        # Make the rectangle for collision
        self.modify_rect(x, y, width, height)
        
        # Set up the movement variables
        self.modify_movement(ground_accelerate, ground_decelerate, # Ground movement
                air_acclerate, air_decelerate, air_antigravity, air_downards, # Air movement
                 wall_decelerate, wall_jump_dy, wall_jump_dx, # Wall movement
                 jump_speed, # Jump speed
                 capped_dx_ground, capped_dx_air, # Capped dx for ground and air
                 capped_dx, capped_dy, # Capped dx
                 gravity # Gravity on the player
                 )
        
        self.past_up = self.past_down = self.past_left = self.past_right = True # Past key presses
        
        
        self.action = 'None'
    
    
    def modify_rect(self, x, y, width, height):
        '''Modifies the rectangle properties
        (int, int, int, int)-> modify rectangle
        '''
        self.rect = rectangle.Rectangle(x, y, width, height)
        
    def modify_movement(self, ground_accelerate, ground_decelerate, # Ground movement
                air_acclerate, air_decelerate, air_antigravity, air_downards, # Air movement
                 wall_decelerate, wall_jump_dy, wall_jump_dx, # Wall movement
                 jump_speed, # Jump speed
                 capped_dx_ground, capped_dx_air, # Capped dx for ground and air
                 capped_dx, capped_dy, # Capped dx
                 gravity # Gravity on the player
                 ):
        '''
        Modifies player movement
        (int, int , int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) -> Player movement attributes
        '''
        self.ground_accelerate = ground_accelerate
        self.ground_decelerate = ground_decelerate # Ground movement
        self.air_accelerate = air_acclerate
        self.air_decelerate = air_decelerate
        self.air_antigravity = air_antigravity 
        self.air_downwards = air_downards # Air movement
        self.wall_decelerate = wall_decelerate
        self.wall_jump_dy = wall_jump_dy 
        self.wall_jump_dx = wall_jump_dx # Wall movement
        self.jump_speed = jump_speed
        self.capped_dx_ground = capped_dx_ground
        self.capped_dx_air = capped_dx_air # Capped dx for ground and air
        self.capped_dx = capped_dx
        self.capped_dy = capped_dy # Capped dx
        self.gravity = gravity
        
        
    def input(self, up, down, left, right):
        '''
        Takes input
        (bool, bool, bool, bool)
        '''
        side_touch = self.rect.side_touch
        side_touch_past = self.rect.side_touch_past
        slope_ratio = self.rect.slope_ratio
        
        '''Left and right movement'''
        if right and not left: # Right movement
            
            if self.rect.dx <= self.capped_dx_ground and side_touch == 'Top': # Right movement on top of rect
                self.rect.dx += self.ground_accelerate * self.rect.ground_friction
                
            if side_touch == 'Top Slope Left': # Up a triangle ///
                
                new_dx_cap = self.capped_dx_ground/ slope_ratio
                
                if self.rect.dx <= new_dx_cap: # Up slope movement speed change cap
                    self.rect.dx += self.ground_accelerate / slope_ratio
                    
                else: # Speed slow down if greater than max up the slope
                    
                    new_dx = self.rect.dx - self.ground_accelerate * (slope_ratio)
                    self.rect.dx = new_dx if new_dx > new_dx_cap else new_dx_cap # Make it not constantly alternate between different dx
                    
            if side_touch == 'Top Slope Right': # Down a slope \\\\
                new_dx_cap = self.capped_dx_ground * slope_ratio
                if self.rect.dx <= new_dx_cap:
                    self.rect.dx += self.ground_accelerate * slope_ratio
                    
                
            if self.rect.dx <= self.capped_dx_air and side_touch != 'Top': # Right movement air
                self.rect.dx +=  self.air_accelerate  
                
            if side_touch == "Left": # Wall decelerate
                self.rect.dx = 1
                if self.rect.above_bottom: # DY DECEL
                    self.rect.dy *= self.wall_decelerate * self.rect.wall_friction
            
            
            
        elif left and not right: # Left movement
            if self.rect.dx >= -self.capped_dx_ground and side_touch == 'Top': # Left movement on top of rect
                self.rect.dx += -self.ground_accelerate * self.rect.ground_friction
                
            if side_touch == 'Top Slope Right': # Up a triangle \\\
                new_dx_cap = self.capped_dx_ground/ slope_ratio
                
                if self.rect.dx >= -new_dx_cap: # Up slope movement speed change cap
                    self.rect.dx -= self.ground_accelerate / slope_ratio
                    
                else: # Speed slow down if greater than max up the slope
                    new_dx = self.rect.dx + self.ground_accelerate * (slope_ratio)
                    self.rect.dx = new_dx if new_dx < -new_dx_cap else -new_dx_cap # Make it not constantly alternate between different dx but it is still doing that and oh well
                    
            if side_touch == 'Top Slope Left': # Down a slope /////
                new_dx_cap = self.capped_dx_ground * slope_ratio
                if self.rect.dx >= -new_dx_cap:
                    self.rect.dx -= self.ground_accelerate * slope_ratio
                    
            if self.rect.dx >= -self.capped_dx_air and side_touch != 'Top': # Left movement air
                self.rect.dx +=  -self.air_accelerate 
                
            if side_touch == 'Right' : # Wall decelerate
                self.rect.dx = -1
                if self.rect.above_bottom:
                    self.rect.dy *= self.wall_decelerate * self.rect.wall_friction # DY DECEL
                    
        else:
            self.rect.dx *= self.ground_decelerate * self.rect.ground_friction if (side_touch.startswith('Top')) else self.air_decelerate # Sideways deceleration air or ground 
            
        
        if side_touch.startswith('Top Slope'):  # Up slope movement lower to go to movement max
            #++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            if (side_touch == 'Top Slope Left' and self.rect.dx > 0) or (side_touch == 'Top Slope Right' and self.rect.dx < 0): # Up slope ////\\\\\ Y change
                new_dy = -(math.fabs(self.rect.dx) - math.fabs(self.rect.dx) / slope_ratio)
                self.rect.dy = new_dy if new_dy < self.rect.dy else self.rect.dy # Make sure it lets the player jump this is probably not necessary 
            
            else: # down a slope
                self.rect.dy = math.fabs(self.rect.dx) * (slope_ratio - 1)
                
                    
        
            
                
        
        
        # ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        #++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        '''Up and down movement'''
        self.rect.dy += self.gravity # Gravity
        
        if up and not down: # Jump, wall jump left, wall jump right, and hover
            if (side_touch.startswith('Top') or side_touch_past.startswith('Top')) and not self.past_up: # Jump from top of block or triangle
                if self.rect.dy > 0:
                    self.rect.dy = self.rect.dy/10 + self.jump_speed * self.rect.jump
                else:
                    self.rect.dy += self.jump_speed * self.rect.jump
                    
            elif (side_touch == 'Left' or side_touch_past == 'Left') and self.rect.above_bottom and not self.past_up: # Wall jump from left of block
                self.rect.dy = self.wall_jump_dy * self.rect.wall_jump_up + self.rect.dy/4
                self.rect.dx = -self.wall_jump_dx * self.rect.wall_jump_sideways
            elif (side_touch == 'Right' or side_touch_past == 'Right') and self.rect.above_bottom and not self.past_up: # Wall jump from right of block
                self.rect.dy = self.wall_jump_dy * self.rect.wall_jump_up + self.rect.dy/4
                self.rect.dx = self.wall_jump_dx * self.rect.wall_jump_sideways
            else:
                self.rect.dy += -self.air_antigravity
                
        elif down and not up: # Fall faster when user is pressing down
            self.rect.dy += self.air_downwards
            
        self.rect.dx = cap(self.capped_dx, self.rect.dx)
        self.rect.dy = cap(self.capped_dy, self.rect.dy)
    
        # Check up
        if self.past_up:
            self.rect.color = (0,0,0)
        else:
            self.rect.color = (100,100,100)
    
        #Store past key presses
        self.past_up = True if (up) else False
        self.past_down = True if (down) else False
        self.past_left = True if (left) else False
        self.past_right = True if (right) else False
        
    
    def vectorSpeedConstant(self):
        ''' Vector speed constant for flying creatures'''
        if self.rect.dx**2 < 0.1 and self.rect.dy**2 < 0.1:
            self.rect.dx = 0
            self.rect.dy = 0
            return
        ratio = 20.0/math.sqrt(self.rect.dx ** 2 + self.rect.dy ** 2)
        if ratio < 1:
            self.rect.dx *= ratio
            self.rect.dy *= ratio
            
            
    def playerLogic(self, up, down, left, right):
        '''Combines the player logic to one function'''
        self.input(up, down, left, right)
        self.rect.move()
            
