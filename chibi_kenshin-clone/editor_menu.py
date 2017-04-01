''' CS 108
Created Fall 2014
editor menu
@author: Kristofer Brink (kpb23)
'''



import pygame, sys
import math
from pygame.locals import *
import player
import util
from environment_editor import Environment_editor
from block_variable import Block_variable
from block import Collidable_block, Scenery_block
from input_mode import InputMode
import json

class Editor_menu:
    '''
    Models editor menu
    
    '''
    def __init__(self, game):
        ''' 
        Constructor
        (game) -> Editor_menu
        '''
        self.menu_background_surf = util.load('editor/menu_button.png')
        self.make_options_surf = util.load('editor/make_options.png')
        self.selection_surf = util.load('editor/selection.png')
        self.editor_menu_option = util.load('editor/editor_menu_options.png')
        self.world_name_dialog = util.load('editor/world_name_dialog.png')
        # ----------- Select rect -- credits - http://stackoverflow.com/questions/6339057/draw-a-transparent-rectangle-in-pygame
        self.select_rect_surf = pygame.Surface((220,64))
        self.select_rect_surf.set_alpha(150)
        self.select_rect_surf.fill((50, 255, 50))
        
        self.make_option = 0 # 4 = collidable
        self.make_option_x = -991
        self.option_specific = 1 # Starts with plain block making
        # Make the white collumn
        self.white_col = pygame.Surface((360, 150))
        self.white_col.set_alpha(128)
        self.white_col.fill((255,255,255))
        # idea self.game.block.append(Block(0, 0, pygame.Color(0,0,0)))
        # self.block_ctor(x, y, surface, **{k: v.current for k, v in self.variables.items()}) **{'jump': 2.5}  # http://stackoverflow.com/questions/334655/passing-a-dictionary-to-a-function-in-python-as-keyword-parameters
        self.collidable = Environment_editor(
            [game.block],
            'Collidable',
            game.collidable_images,
            Collidable_block,
            [('jump', Block_variable(1, .1, 4, .1, 'Jump')), 
            ('ground_friction', Block_variable(1, .01, 2, .1, 'Ground Friction')),
            ('wall_friction', Block_variable(1, .01, 2, .1, 'Wall Friction')),
            ('wall_jump_up', Block_variable(1, .01, 2, .1, 'Wall Jump Up')),
            ('wall_jump_sideways', Block_variable(1,.01, 2, .1, 'Wall Jump Sideways'))])

        self.scenery = Environment_editor([game.scenery_background, game.scenery_foreground], 
                                          'scenery/still', game.scenery_images, Scenery_block, 
                                          [('rotation', Block_variable(0, 0, 359, -1, 'Rotation', True))])
        
        self.brush_tuple = (self.collidable, self.scenery) # Tuple with different brushes
        
        try:
            self.myfont = pygame.font.SysFont("segoeuihistoric", 25)
        except:
            self.myfont = pygame.font.SysFont(None,25)
        
        # For saving and quitting
        self.menu_open = False
        
    def save(self, name, game):
        ''' Saves the world to a txt file with name specified by the user'''
        with open('worlds/' + name +'.txt', 'w') as file:
            # https://docs.python.org/2/library/json.html
            file.write(json.dumps(game.to_dictionary(), indent = 2))

        
    def input(self, key_escape, key_q, key_e, key_r, key_f, space, ctrl, key_w, key_s, key_a, key_d, mouse_x, mouse_y, click, scroll, game):
        '''Takes input to switch between making options'''
        if not space:
            if key_q:
                self.make_option -= 1
                self.collidable.making_something = self.scenery.making_something = False
            if key_e:
                self.make_option += 1
                self.collidable.making_something = self.scenery.making_something = False
                
        self.make_option %= len(self.brush_tuple)
        self.make_option_x = -991 + 210 * self.make_option
        
        width, height = self.menu_background_surf.get_size()
        if (mouse_x < width and mouse_y < height and click) or key_escape:
            self.menu_open = True
            return game
        
        if self.menu_open:
            game.camera.cam_game = False
            screen_width, screen_height = pygame.display.get_surface().get_size()
            width, height = self.editor_menu_option.get_size()
            
            save_level_rect = pygame.Rect(screen_width//2 - width//2, screen_height//2 - height//2, width, height/2)
            quit_rect = pygame.Rect(screen_width//2 - width//2, screen_height//2, width, height/2)
            # Figure out if click on save level
            if click:
                if save_level_rect.collidepoint(mouse_x, mouse_y):
                    def store_text(text): # http://stackoverflow.com/questions/17081243/how-to-use-a-callback-function-in-python
                        self.save(text, game)
                        
                    dialog = InputMode(game, game.draw, store_text, self.world_name_dialog,restrict_to_letters=True, orig_string= '' if game.file_name is None else game.file_name[7:-4])
                    
                    self.menu_open = False
                    
                    return dialog
                
            # Figure out if click on quit
                elif quit_rect.collidepoint(mouse_x, mouse_y):
                    return game.menuGameMode
                else:
                    self.menu_open = False
        else:
            self.brush_tuple[self.make_option].input(mouse_x, mouse_y, click, scroll, key_r, key_f, space, ctrl, key_w, key_s, key_a, key_d, key_e, game)
            
    def logic(self, mouse_x, mouse_y, click, scroll, ctrl, game):
        '''Runs the brushes logic'''
        self.brush_tuple[self.make_option].logic(mouse_x, mouse_y, click, scroll, ctrl, game)
        

    def draw(self, draw, screen, game):
        ''' 
        Draws things for the editor
        '''
        screen_width, screen_height = screen.get_size()
        
        # Draw the green column on the right side  - credits to - http://stackoverflow.com/questions/6339057/draw-a-transparent-rectangle-in-pygame
        white_col = pygame.Surface((130, screen_height))
        white_col.set_alpha(128)
        white_col.fill((215,255,215))
        screen.blit(white_col, (screen_width - 130, 0))
        
        # Draw see through  green rectangle on the bottom
        green_bot = pygame.Surface((300, 200))
        green_bot.set_alpha(128)
        green_bot.fill((215,255,215))
        screen.blit(green_bot, (screen_width/2 - 150, screen_height - 200))
        
        # Draw Selection surface that moves with q and e
        screen.blit(self.selection_surf, (screen_width - 130, screen_height/2 - 120,))
        
        # Draw specifically for the brush tuple
        self.brush_tuple[self.make_option].draw(draw, screen, game)
        
        make_option_width = self.make_options_surf.get_size()[0]
        
        # Draw the background or should I say the thing that displays the text of the different brushes
        screen.blit(self.menu_background_surf, (0,0))
        
        # Draw the green selection thing that is at the middle
        screen.blit(self.select_rect_surf, (self.make_option_x + screen_width, 0))
        # Draw the make_options or should I say the thing that displays the text of the different brushes
        screen.blit(self.make_options_surf, (screen_width - make_option_width, 0))
        
        # Draw the X and Y of the player also the screen
        # Draw the white back to make the things visible
        screen.blit(self.white_col, (0, screen_height - 150))
        
        
        player_pos = self.myfont.render("Player:(%d, %d)(%.2f, %.2f)" % 
                                        (game.myUser.rect.rect.x, game.myUser.rect.rect.y,
                                         game.myUser.rect.dx, game.myUser.rect.dy), 1, (0,0,0)) # Player position
        screen.blit(player_pos, (10, screen_height - 150))
        
        screen_pos = self.myfont.render("Screen: (" + str(game.camera.x) + ', ' + str(game.camera.y) + ')', 1, (0,0,0)) # camera position
        screen.blit(screen_pos, (10, screen_height - 125))
        
        block_number = self.myfont.render("Collidable: " + str(len(game.block)), 1, (0,0,0)) # Number of things made in the collidable
        screen.blit(block_number, (10, screen_height - 100))
        
        scenery_number = self.myfont.render("Scenery: " + str(len(game.scenery_foreground) + len(game.scenery_background)), 1, (0,0,0))
        screen.blit(scenery_number, (10, screen_height - 75))
        
        
        if self.menu_open:
            width, height = self.editor_menu_option.get_size()
            util.draw_center(self.editor_menu_option, screen, draw, width, height, 0, 0)
        
        
            
            