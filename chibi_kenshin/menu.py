''' CS 108
Created Fall 2014
menu
@author: Kristofer Brink (kpb23)
'''


import pygame, sys
import math
from pygame.locals import *
from game import GameGameMode
from editor import EditorGameMode
import util
from random import randint
import os

        
X_ORIGINAL = 1400
Y_ORIGINAL = 900


class Select_Bar:
    '''
    Models the Select Bar
    Invariants:
        None
    '''
    def __init__(self):
        '''
        ()-> Select_Bar
        '''
        self.menu_place = 0
        self.menu_place_screen = 368
        self.menu_place_actual = 0
        
    def input(self, up, down):
        '''Takes input and move the select bar'''
        if up and self.menu_place > 0:
            self.menu_place -= 1
        if down and self.menu_place < 2:
            self.menu_place += 1
        self.logic()
            
    def getplace(self):
        '''Returns the position of the select bar'''
        if self.menu_place == 0:
            return 'Choose World Game'
        elif self.menu_place == 1:
            return 'Choose World Editor'
        else:
            return 'Menu Quit'
           
    def logic(self):
        '''Changes the place on the screen'''
        self.menu_place_screen = 368 + self.menu_place * 145
           
    def draw(self, screen, draw, surf):
        util.draw_center(surf, screen, draw, X_ORIGINAL, Y_ORIGINAL, 227, self.menu_place_screen)



class Particle:
    '''
    Models a particle that flies in the background
    '''
    def __init__(self, y):
        '''
        (int)-> Particle
        '''
        self.x = randint(400, 900)
        self.y = y
        self.x_vel = randint(-10,10)
        self.y_vel = randint(-10, 10)
        self.size = randint(50,100)
        
    def logic(self):
        ''' Runs the logic of the particle'''
        self.x += self.x_vel
        self.y += self.y_vel
        self.size -= 1
        return self.size <= 0
    
    def reverse(self):
        ''' Reverses direction of the particle'''
        self.x_vel *= -1
        self.y_vel *= -1
        
    def perpendicular_movement(self, direction):
        '''Makes the particle move perpendicular to it's past movement'''
        x_vel_temp = self.x_vel 
        self.x_vel = self.y_vel
        self.y_vel = (-x_vel_temp)
           
    def draw(self, draw, screen, surf):
        ''' Draws the particle'''
        util.draw_center(pygame.transform.scale(surf, (self.size, self.size)), screen, draw, X_ORIGINAL, Y_ORIGINAL, self.x, self.y)



class MenuGameMode:
    '''
    Models the Game Mode that is in the menu
    '''
    def __init__(self):
        '''
        ()-> MenuGameMode:
        '''
        # Directional keys
        self.up = self.down = self.left = self.right = False
        # Choosing keys
        self.key_z = self.enter = self.key_x = self.backspace = False
        
        # Darkness of the screen
        self.darkness = 0
        self.current_darkness = 0
        
        # For in main menu part
        # Loading images
        self.menu_background = util.load('menu/menu_image.png', alpha=False)
        self.instructions = util.load('menu/instructions.png')
        self.menu_options = util.load('menu/options.png')
        self.particle_surf = util.load('menu/particle.png')
        self.select_bar_surf = util.load('menu/select_bar.png')
        self.EDITOR = util.load('menu/EDITOR.png')
        self.PLAY = util.load('menu/play!.png')
        
        
        # Select_bar
        self.select_bar = Select_Bar() # Make a select bar object
        self.key_press_timer = 0 # Have a key press timer to see if instructions should be displayed
        self.TIME_TIL_SHOW = 120

        # Particles
        self.particle = [Particle(self.select_bar.menu_place_screen)]
        
        # For in quit
        # Load images for quit
        self.quit_surf = util.load('menu/quit.png', alpha=False)
        
        # Quit yes or no?
        self.quit_option = False
        
        self.in_current = 'Menu'
        
        # set up my font
        try:
            self.myfont = pygame.font.SysFont("segoeuihistoric", 50)
        except:
            self.myfont = pygame.font.SysFont(None,50)
        
        self.world_choose = 0


    def handleEvent(self, event):
        '''
        Handle events for the menu
        Controls are arrow keys, enter, z, backspace, escape, x
        '''
        # Movement
        self.up = self.down = self.left = self.right = False
        # Choosing
        self.key_z = self.enter = self.key_x = self.backspace = self.escape =  False
        # Deleting levels
        self.delete = False
        
        if event.type == KEYDOWN:
            
            if event.key == K_UP:
                self.up = True
        
            if event.key == K_DOWN:
                self.down = True
                
            if event.key == K_LEFT:
                self.left = True
                
            if event.key == K_RIGHT:
                self.right = True
                
            if event.key == K_z:
                self.key_z = True
            
            if event.key == K_x:
                self.key_x = True
                
            if event.key == K_RETURN:
                self.enter = True
            
            if event.key == K_BACKSPACE:
                self.backspace = True
                    
            if event.key == K_ESCAPE:
                self.escape = True
                
            if event.key == K_DELETE:
                self.delete = True
                
            if event.key == K_g:
                return GameGameMode(self, file_name= 'worlds/asdf.txt')
            
            if event.key == K_h:
                return EditorGameMode(self, file_name = 'worlds/asdf.txt')

    
        if self.in_current == 'Menu':
            # input of main menu
            if self.up:
                self.reverse_particles()
                
            if self.down:
                self.reverse_particles()
            
            if self.left:
                for i in self.particle:
                    i.perpendicular_movement(1)
                    
            if self.right:
                for i in self.particle:
                    i.perpendicular_movement((-1))
                # reverse movement of particles
                for i in range(len(self.particle)):
                    self.particle[i].reverse()
            
            self.select_bar.input(self.up, self.down)
            
            if self.key_z or self.enter:
                self.in_current = self.select_bar.getplace()
                if self.in_current == 'Menu Quit':
                    self.darkness = 200
                    return
                if self.in_current.startswith('Choose World'):
                    self.particle = []
                    self.darkness = 300
                    if not os.path.exists('worlds'): # http://stackoverflow.com/questions/273192/check-if-a-directory-exists-and-create-it-if-necessary
                        os.makedirs('worlds')
                    self.worlds = sorted(os.listdir('worlds/'))
                    if self.in_current == 'Choose World Editor':
                        self.worlds.insert(0, None)
                    self.world_choose = 0
                    return
            if self.escape:
                self.in_current = 'Menu Quit'
                self.darkness = 200
                return
            
        if self.in_current == 'Menu Quit':
            
            if self.right:
                self.quit_option = False
                return
            if self.left:
                self.quit_option = True
                return
            if self.key_x or self.backspace or self.escape:
                self.in_current = 'Menu'
                self.current_darkness = 0
                self.quit_option = False
                return
            if self.key_z or self.enter:
                if self.quit_option == False:
                    self.in_current = 'Menu'
                    self.current_darkness = 0
                if self.quit_option == True:
                    return 'Quit'
                
        if self.in_current.startswith('Choose World'):
            
            if self.key_x or self.backspace or self.escape:
                self.in_current = 'Menu'
                self.current_darkness = 0
                return
                
            if self.up:
                if self.world_choose > 0:
                    self.world_choose -= 1
                return
            if self.down:
                if self.world_choose < len(self.worlds) - 1:
                    self.world_choose += 1
                return
                    
            if self.key_z or self.enter:
                chosen_world = self.worlds[self.world_choose]
                if self.in_current == 'Choose World Game':
                    return GameGameMode(self, file_name = 'worlds/' + chosen_world)
                else:
                    return EditorGameMode(self, file_name = ('worlds/' + chosen_world) if chosen_world is not None else chosen_world)
                
            if self.delete and self.in_current == 'Choose World Editor':
                
                if self.worlds[self.world_choose] is not None:
                    os.remove('worlds/'+ self.worlds[self.world_choose])
                    self.worlds.pop(self.world_choose)
                    self.world_choose -= 1
    
        
    def reverse_particles(self):
        ''' Reverses particles and changes the key press timer'''
        self.key_press_timer = 0
        # reverse movement of particles
        for i in self.particle:
            i.reverse()
        
        
    def logic(self):
        ''' Runs all logic for the Menu'''
        if self.in_current.startswith('Menu'):
            # Select Bar Logic
            
            # Instructions logic
            if self.key_press_timer <= self.TIME_TIL_SHOW:
                self.key_press_timer += 1
    
            # Particle logic
            for i in reversed(range(len(self.particle))): # http://stackoverflow.com/questions/1207406/remove-items-from-a-list-while-iterating-in-python/1207485#1207485
                if self.particle[i].logic():
                    self.particle.pop(i)
            # Random number of particles made
            particle_make = randint(0,10)
            # Loop through and make a certain amount of particles
            for i in range(0, particle_make):
                self.particle.append(Particle(self.select_bar.menu_place_screen + 20))
        
#         
#         if self.in_world_choose:
#             
#          
#
    def screen_darken(self, draw, screen):
        ''' Fade the screen'''
        screen_width, screen_height = screen.get_size()
        darken = pygame.Surface((screen_width, screen_height))
        
        
        if self.current_darkness < self.darkness:
            self.current_darkness += 10
        elif self.current_darkness > self.darkness:
            self.current_darkness -= 10
        
        darken.set_alpha(self.current_darkness)
        darken.fill((0, 0, 0))
        screen.blit(darken, (0, 0))
        
        
        
        
                 
    def draw(self, draw, screen):
        '''Drawing for the menu'''
        
        if self.in_current.startswith('Menu'):
            # draw white before menu background
            screen.fill(pygame.Color(255, 255, 255))
            
            # Draw the menu background and center it
            util.draw_center(self.menu_background, screen, draw, X_ORIGINAL, Y_ORIGINAL)
            
            # Display the keyboard instructions if the user need it
            if self.key_press_timer > self.TIME_TIL_SHOW:
                util.draw_center(self.instructions, screen, draw, X_ORIGINAL, Y_ORIGINAL)
            # Draw particles
            for i in range(len(self.particle)):
                self.particle[i].draw(pygame.draw, screen, self.particle_surf)
                
            # draw the select bar (the sword handles on the sides)
            self.select_bar.draw(screen, draw, self.select_bar_surf)
            
            # Draw menu options
            util.draw_center(self.menu_options, screen, draw, X_ORIGINAL, Y_ORIGINAL)
            
            
            if self.in_current == 'Menu Quit':
                self.screen_darken(draw, screen)
                width, height = self.quit_surf.get_size()
                util.draw_center(self.quit_surf, screen, draw, width, height)
                
                # Draw the selection that is currently being selected
                quit_select = pygame.Surface((width//2, height//2))
                quit_select.set_alpha(128)
                quit_select.fill((100, 150, 100))
                if self.quit_option == False:
                    util.draw_center(quit_select, screen, draw, 0,0)
                else:
                    util.draw_center(quit_select, screen,draw, width, 0)
            
        if self.in_current.startswith('Choose World'):
            screen_width, screen_height = screen.get_size()
            screen.fill(pygame.Color(255, 255, 255))
            self.screen_darken(draw, screen)
            x_spot = screen_width//10
            y_spot = screen_height//4 - self.world_choose * 50
            count = 0
            for world in self.worlds:
                
                world_text = self.myfont.render(world[:-4] if world is not None else 'NEW WORLD', 1, (255,255,255) if count != self.world_choose else (0,255,0))
                screen.blit(world_text, (x_spot, y_spot))
                y_spot += 50
                count += 1
                
            if self.in_current == 'Choose World Game':
                screen.blit(self.PLAY, (screen_width - self.PLAY.get_size()[0], 0))
                
            else:
                screen.blit(self.EDITOR, (screen_width - self.EDITOR.get_size()[0], 0))
    
                