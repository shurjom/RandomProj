/*
 * This file contains the maze-solving strategy.
 * Modified By Moat Amer 04Jan2013
 * moatasa@gmail.com
 */

#include <pololu/3pi.h>
#include "follow-segment.h"
#include "turn.h"

// The path variable will store the path that the robot has taken.  It
// is stored as an array of characters, each of which represents the
// turn that should be made at one intersection in the sequence:
//  'L' for left
//  'R' for right
//  'S' for straight (going straight through an intersection)
//  'B' for back (U-turn)
//
// Whenever the robot makes a U-turn, the path can be simplified by
// removing the dead end.  The follow_next_turn() function checks for
// this case every time it makes a turn, and it simplifies the path
// appropriately.
char path[100] = "";
int intersection[100];
unsigned char path_length = 0; // the length of the path
char paths[10] = "";
int intersections[10];
unsigned char paths_length = 0; // the length of the path sample
int m=5;
char pathsol[100] = "";
unsigned char pathsol_length = 0; // the length of the pathsol

// This function decides which way to turn during the learning phase of
// maze solving.  It uses the variables found_left, found_straight, and
// found_right, which indicate whether there is an exit in each of the
// three directions, applying the "left hand on the wall" strategy.
char select_turn(unsigned char found_left, unsigned char found_straight, unsigned char found_right)
{
    // Make a decision about how to turn.  The following code
    // implements a left-hand-on-the-wall strategy, where we always
    // turn as far to the left as possible.
    if(found_left) // L->1
        return 'L';
    else if(found_straight) // S->2
        return 'S';
    else if(found_right) // R->3
        return 'R';
    else // back->0
        return 'B';
}


// code for intersection recording
int record_intersec(unsigned char found_left, unsigned char found_straight, unsigned char found_right)
{
    // Make a decision about how to turn.  The following code
    // implements a left-hand-on-the-wall strategy, where we always
    // turn as far to the left as possible.
    if(found_left && found_right && found_straight) // LRS->4
        return '4';
    else if(found_left && found_right) // LR->3
        return '3';
    else if(found_right && found_straight) // RS->2
        return '2';
    else if(found_left && found_straight) // LS->1
        return '1';
    else // No intersection->0
        return '0';
}

// Path simplification.  The strategy is that whenever we encounter a
// sequence xBx, we can simplify it by cutting out the dead end.  For
// example, LBL -> S, because a single S bypasses the dead end
// represented by LBL.

void simplify_path()
{
    // only simplify the path if the second-to-last turn was a 'B'
    if(pathsol_length < 3 || pathsol[pathsol_length-2] != 'B')
        return;
    
    int total_angle = 0;
    int i;
    for(i=1;i<=3;i++)
    {
        switch(pathsol[pathsol_length-i])
        {
            case 'R':
                total_angle += 90;
                break;
            case 'L':
                total_angle += 270;
                break;
            case 'B':
                total_angle += 180;
                break;
        }
    }
    
    // Get the angle as a number between 0 and 360 degrees.
    total_angle = total_angle % 360;
    
    // Replace all of those turns with a single one.
    switch(total_angle)
    {
        case 0:
            pathsol[pathsol_length - 3] = 'S';
            break;
        case 90:
            pathsol[pathsol_length - 3] = 'R';
            break;
        case 180:
            pathsol[pathsol_length - 3] = 'B';
            break;
        case 270:
            pathsol[pathsol_length - 3] = 'L';
            break;
    }
    
    // The path is now two steps shorter.
    pathsol_length -= 2;
}

// This function is called once, from main.c.
void maze_solve()
{
    // Loop until we have record the maze.
    while(1)
    {
        // FIRST MAIN LOOP BODY
        follow_segment();
        
        // Drive straight a bit.  This helps us in case we entered the
        // intersection at an angle.
        // Note that we are slowing down - this prevents the robot
        // from tipping forward too much.
        set_motors(50,50);
        delay_ms(50);
        
        // These variables record whether the robot has seen a line to the
        // left, straight ahead, and right, while examining the current
        // intersection.
        unsigned char found_left=0;
        unsigned char found_straight=0;
        unsigned char found_right=0;
        
        // Now read the sensors and check the intersection type.
        unsigned int sensors[5];
        read_line(sensors,IR_EMITTERS_ON);
        
        // Check for left and right exits.
        if(sensors[0] > 100)
            found_left = 1;
        if(sensors[4] > 100)
            found_right = 1;
        
        // Drive straight a bit more - this is enough to line up our
        // wheels with the intersection.
        set_motors(40,40);
        delay_ms(200);
        
        // Check for a straight exit.
        read_line(sensors,IR_EMITTERS_ON);
        if(sensors[1] > 200 || sensors[2] > 200 || sensors[3] > 200)
            found_straight = 1;
        
        // Check for the ending spot.
        // If all three middle sensors are on dark black, we have
        // solved the maze.
        if(sensors[1] > 600 && sensors[2] > 600 && sensors[3] > 600)
            break;
        
        // Intersection identification is complete.
        // If the maze has been solved, we can follow the existing
        // path.  Otherwise, we need to learn the solution.
        unsigned char dir = select_turn(found_left, found_straight, found_right);
        
        int recint = record_intersec(found_left, found_straight, found_right);
        // Make the turn indicated by the path.
        turn(dir);
        
        // Store the intersection in the path variable.
        path[path_length] = dir;
        intersection[path_length] = recint;
        path_length ++;
        
        // You should check to make sure that the path_length does not
        // exceed the bounds of the array.  We'll ignore that in this
        // example.
        
    }
    // Maze Recorded!
    while(1)
    {
        // Beep to show that we finished the maze.
        paths_length=0;
        set_motors(0,0);
        play(">>a32");
        
        // Wait for the user to press a button.
        while(!button_is_pressed(BUTTON_B))
        {
            clear();
            print("Recorded!");
            lcd_goto_xy(0,1);
            print("Press B");
            
            delay_ms(30);
        }
        while(button_is_pressed(BUTTON_B));
        
        clear();
        print("Here we");
        lcd_goto_xy(0,1);
        print("Go!");
        delay_ms(1000);
        
        int i=0;
        while (i<4)
        {
            follow_segment();
            // Drive straight a bit.  This helps us in case we entered the
            // intersection at an angle.
            // Note that we are slowing down - this prevents the robot
            // from tipping forward too much.
            set_motors(50,50);
            delay_ms(50);
            
            // These variables record whether the robot has seen a line to the
            // left, straight ahead, and right, while examining the current
            // intersection.
            unsigned char found_left=0;
            unsigned char found_straight=0;
            unsigned char found_right=0;
            
            // Now read the sensors and check the intersection type.
            unsigned int sensors[5];
            read_line(sensors,IR_EMITTERS_ON);
            display_readings(sensors);
            // Check for left and right exits.
            if(sensors[0] > 100)
                found_left = 1;
            if(sensors[4] > 100)
                found_right = 1;
            
            // Drive straight a bit more - this is enough to line up our
            // wheels with the intersection.
            set_motors(40,40);
            delay_ms(200);
            
            // Check for a straight exit.
            read_line(sensors,IR_EMITTERS_ON);
            if(sensors[1] > 200 || sensors[2] > 200 || sensors[3] > 200)
                found_straight = 1;
            
            unsigned char dir = select_turn(found_left, found_straight, found_right);
            
            int recint = record_intersec(found_left, found_straight, found_right);
            // Make the turn indicated by the path.
            turn(dir);
            
            // Store the intersection in the path variable.
            paths[paths_length] = dir;
            intersections[paths_length] = recint;
            if (paths[paths_length] != 'B')
            {
                i++;
                paths_length ++;
            }
            else
            {
                paths_length ++;
            }
            
        }
        // Find the robot location with respect to the end.
        // The Robot location code can be simplified more.
        // Its Possible to modify this code, so we start by running the robot two steps,
        // then we check how many values of m we have, if we have more than one value we add another step and so on until we have one m value.
        int i2;
        int c2;
        
        for (i=0;i<100;i++)
        {
            if (i<(100-paths_length))
            {
                i2=0;
                while (i2<paths_length)
                {
                    c2=0;
                    if (paths[i2]==path[i+i2] && intersections[i2]==intersection[i+i2])
                    {
                        c2=1;
                        i2++;
                    }
                    if (c2!=1)
                    {
                        i2=100;
                    }
                    
                    if (i2 == paths_length )
                    {
                        m=i+paths_length;
                    }
                }
            }
        }
        
        // We found the Robot location, now find the shortest way to the end.
        for(i=0;i<(100-m);i++)
        {
            pathsol[pathsol_length]=path[i+m];
            pathsol_length ++;
            simplify_path();
        }
        
        // Go to the end.
        i=0;
        while(i<pathsol_length)
        {
            // Re-run the maze.  It's not necessary to identify the
            // intersections, so this loop is really simple.
            follow_segment();
            
            // Drive straight while slowing down, as before.
            set_motors(50,50);
            delay_ms(50);
            set_motors(40,40);
            delay_ms(200);
            
            // Make a turn according to the instruction stored in
            // pathsol[i].
            turn(pathsol[i]);
            i+=1;
            unsigned int sensors[5];
            read_line(sensors,IR_EMITTERS_ON);
            // The end is reached.
            if(sensors[1] > 600 && sensors[2] > 600 && sensors[3] > 600)
                pathsol_length=0;
        }
        // Now we should be at the finish!  Restart the loop.
    }
}

// Local Variables: **
// mode: C **
// c-basic-offset: 4 **
// tab-width: 4 **
// indent-tabs-mode: t **
// end: **