'''
jfkjfbv

'''

import turtle
import sys
#Give the name "window" to the screen where the turtle will appear
window = turtle.Screen()

#Create a turtle and name it bob
bob = turtle.Turtle()
file = input('Input a file with turtle directions. ')
f = open(file)
lines = f.readlines()
counter = 0
for lin in lines:
    l = lin.split()
    if 'penup' in l:
        bob.penup()
    elif 'pendown' in l:
        bob.pendown()
    elif 'color' in l:
        bob.color(l[1].rstrip())
    elif 'goto' in l:
        bob.goto(int(l[1]), int(l[2].rstrip()))
    elif 'point' in l:
        bob.pensize(int(l[1]))
        bob.goto(int(l[2]), int(l[3]))

window.exitonclick()