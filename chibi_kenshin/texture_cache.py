''' CS 108
Created Fall 2014
Texture Cache
@author: Kristofer Brink (kpb23)
'''


import util

class Texture_cache:
    '''
    Models a texture cache 
    Texture cache makes the game only load images that have not already or need to be loaded
    '''
    def __init__(self):
        '''
        ()-> Texture_cache
        '''
        self.image_dict = {}
        
    def load(self, image_string):
        ''' Loads an image
        (str) -> pygame surface
        '''
        if not image_string in self.image_dict:
            self.image_dict[image_string] = util.load(image_string)
        return self.image_dict[image_string]
        