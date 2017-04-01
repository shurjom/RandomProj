''' CS 108
Created Fall 2014
camera
@author: Kristofer Brink (kpb23)
'''



import pygame, sys
import math
from pygame.locals import *
import player


class Camera:
    '''
    Models a camera
    '''
    def __init__(self, player_rect):
        '''
        (pygame.rect)-> Camera
        '''
        self.screen_width = 1400
        self.screen_height = 900
        self.xVel = 0
        self.yVel = 0
        self.cam_game = True
        self.cam_game_move(player_rect)
        self.cam_game = False
        self.screen_player_height_division = 1.7
    
    def camera_xy(self, pos, parallax = 1):
        '''
        Uses position and parallax to find where something is on a screen
        '''
        new_x = pos[0] - self.x * parallax + self.screen_width//2
        new_y = pos[1] - self.y * parallax + self.screen_height//self.screen_player_height_division
        return (new_x, new_y)
    
    def input_xy(self, pos, parallax = 1):
        '''
        Uses position and parallax to find where a point on the screen is relative to the map
        '''
        new_x = pos[0] + self.x * parallax - self.screen_width//2
        new_y = pos[1] + self.y * parallax - self.screen_height//self.screen_player_height_division
        return (new_x, new_y)
    
    def parallax_pos(self, pos, parallax = 1):
        '''
        Position relative to parallax
        '''
        new_x = pos[0] - self.x * parallax + self.x
        new_y = pos[1] - self.y * parallax + self.y
        return (int(new_x), int(new_y)) 
    
    
    def cam_take_screen_size(self, screen):
        '''
        Takes the screen size
        '''
        self.screen_width, self.screen_height = screen.get_size()
    
    def cam_game_move(self, player_rect):
        '''
        Centers the camera on the player when the game is running
        '''
        if self.cam_game:
            self.x = player_rect.centerx # self.screen_width//2
            self.y = player_rect.centery # self.screen_height//2
    
    def cam_editor_move(self, up, down, left, right, shift):
        '''
        When the editor is running it lets the user control the camera with wasd
        '''
        if up or down or left or right or shift:
            self.cam_game = False
        if shift:
            self.camera_speed = 25
        else:
            self.camera_speed = 12
        if up:
            self.y -= self.camera_speed
        if down:
            self.y += self.camera_speed
        if left:
            self.x -= self.camera_speed
        if right:
            self.x += self.camera_speed
        

if __name__ == "__main__":
    camera = Camera(pygame.Rect(0,0,10,10))
    assert camera.x == 5
    assert camera.y == 5
    camera.cam_game = True
    camera.cam_game_move(pygame.Rect(10,0,100,0))
    assert camera.x == 60
    assert camera.y == 0
    assert (camera.parallax_pos((5,5),2)) == (-55, 5)
    assert camera.input_xy((3,4), 4) == (-457, -525.0)
    camera.cam_editor_move(True, False, True, False, True)
    assert (camera.x, camera.y) == (35, -25)
    assert camera.camera_xy((0,2), 1.4) == (651.0, 566.0)
    print('All tests pass')
