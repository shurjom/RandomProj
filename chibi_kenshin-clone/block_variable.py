''' CS 108
Created Fall 2014
bock variable
@author: Kristofer Brink (kpb23)
'''


import pygame, sys
from pygame.locals import *
import pygame

class Block_variable:
    '''
    Models a variable for blocks
    Invariants:
        minimum float
        maximum float must be more than minimum
        step float
        wrap around bool
    '''
    def __init__(self, default, minimum, maximum, step, text, wrap_around = False):
        '''
        Constructor
        (float, float, float, float, float, float , bool)-> Block_variable
        '''
        self.current = self.original = float(default)
        self.minimum = float(minimum)
        self.maximum = float(maximum)
        self.step = float(step)
        self.text = text
        if __name__ != '__main__':
            try:
                self.myfont = pygame.font.SysFont("segoeuihistoric", 26)
            except:
                self.myfont = pygame.font.SysFont(None,10)
            
        self.wrap_around = wrap_around
        
    def change(self, up):
        '''
        Change the current value
        (bool)
        '''
        if self.wrap_around:
            self.current = self.current + self.step * (1 if up else -1)
            self.current = self.minimum if self.current > self.maximum else self.current
            self.current = self.maximum if self.current < self.minimum else self.current
        else:
            self.current = min(max(self.current + self.step * (1 if up else -1), self.minimum), self.maximum)
        
    def reset(self):
        '''
        resets current value to default value
        '''
        self.current = self.original
        
    def draw(self, screen, position, color):
        ''' 
        Draws the block variable's current value
        '''
        variable_display = self.myfont.render(self.text + ': %.2f' % (self.current), 1, color)
        screen.blit(variable_display, position)

if __name__ == "__main__":
    # Test block variable
    block_var = Block_variable(1,1,2,.1,'Test', True)
    assert block_var.current == 1
    block_var.change(True)
    assert block_var.current == 1.1
    block_var.change(False)
    assert block_var.current == 1
    block_var.current = 21
    block_var.reset()
    assert block_var.current == block_var.original
    assert block_var.current == 1
    block_var.change(False)
    assert block_var.current == 2
    block_var.change(True)
    assert block_var.current == 1
    print('All tests pass')
    
    
    
    