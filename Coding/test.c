#include <pololu/3pi.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

/* Grid is numbered like:
 
 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
 16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
 32  33  34 ...
 48 ...
 64 ...
 ...
 240
 */

#define Cell       unsigned char
#define Direction   unsigned char
#define NORTH_BIT   6      // NORTH_BIT and EAST_BIT must be two MSBs
#define EAST_BIT   7      // stored in grid array
#define NORTH_BV   (1 << NORTH_BIT)
#define EAST_BV      (1 << EAST_BIT)
#define NORTH      0
#define EAST      1
#define SOUTH      2
#define WEST      3

#define STRAIGHT   0
#define   RIGHT      1
#define BACK      2
#define LEFT      3  // same as -1 % 4

#define MAX_PATH_LENGTH      32

unsigned char path[MAX_PATH_LENGTH];
unsigned char pathLength;

unsigned char visited[32];
unsigned char flood[256];


Direction curDir;
Cell curCell;
Cell endCell = 0;
unsigned char done = 0;
unsigned char fast = 0;


void serialDumpMaze();


void checkForSerial()
{
    char c = 0;
    serial_receive_blocking(&c, 1, 100);
    if (c == '*')
        serialDumpMaze();
}

void error(const char* str)
{
    set_motors(0, 0);
    clear();
    print(str);
    while(1)
    {
        checkForSerial();
    }
}


void setVisited(Cell cell)
{
    visited[cell / 8] |= 1 << (cell & 0x7);
}

unsigned char getVisited(Cell cell)
{
    return (visited[cell / 8] >> (cell & 0x7)) & 1;
}


Cell getCell(Direction dir, Cell cell)
{
    dir = dir & 0x3;
    if (dir == NORTH)
        return cell - 16;
    else if (dir == SOUTH)
        return cell + 16;
    else if (dir == EAST)
        return (cell & 0xF0) + ((cell + 1) & 15);
    else  // dir == WEST
        return (cell & 0xF0) + ((cell - 1) & 15);
}


static inline unsigned char grid(Cell cell)
{
    while (!eeprom_is_ready());
    return ~eeprom_read_byte((unsigned char*)(unsigned int)cell);
}

static inline void setGridBits(Cell cell, unsigned char bits)
{
    unsigned char value = grid(cell);
    value = ~value & ~bits;
    eeprom_write_byte((unsigned char*)(unsigned int)cell, value);
}


unsigned char canGo(Direction dir, Cell cell)
{
    dir = dir & 0x3;
    if (dir == NORTH)
        return (grid(cell) & NORTH_BV) >> NORTH_BIT;
    else if (dir == EAST)
        return (grid(cell) & EAST_BV) >> EAST_BIT;
    else if (dir == SOUTH)
        return (grid(getCell(dir, cell)) & NORTH_BV) >> NORTH_BIT;
    else  // dir == WEST
        return (grid(getCell(dir, cell)) & EAST_BV) >> EAST_BIT;
}


// this cell is not an intersection if it consists of a single straight line (nothing interesting for the sensors to detect)
static inline unsigned char isIntersection(Cell cell)
{
    return !((canGo(NORTH, cell) == canGo(SOUTH, cell)) &&
             (canGo(EAST, cell) == canGo(WEST, cell)) &&
             (canGo(NORTH, cell) != canGo(EAST, cell)));
}


void addPath(Direction dir, Cell cell)
{
    dir = dir & 0x3;
    if (dir == NORTH)
        setGridBits(cell, NORTH_BV);
    else if (dir == EAST)
        setGridBits(cell, EAST_BV);
    else if (dir == SOUTH)
        setGridBits(getCell(dir, cell), NORTH_BV);
    else  // dir == WEST
        setGridBits(getCell(dir, cell), EAST_BV);
}


unsigned char getFloodVal(Cell cell)
{
    return flood[cell];
}


void setFloodVal(Cell cell, unsigned char floodVal)
{
    flood[cell] = floodVal;
}


void floodFrom(Cell cell)
{
    Cell i = 0;
    Cell j;
    unsigned char curFloodVal = 0;
    unsigned char nextFloodVal, tempNextFloodVal;
    
    do
    {
        setFloodVal(i, 255);
    }
    while (++i);
    
    setFloodVal(cell, 0);   // clear flood value of starting cell
    
    for (curFloodVal = 0; curFloodVal < 253; curFloodVal++)   // use of < 253 is intentional (since nextFlood = curFlood + 2 and we want to keep track of cells not reached by the flood)
    {
        nextFloodVal = curFloodVal + 2;
        do
        {
            if (getFloodVal(i) == curFloodVal)   // if cell is at current flood level
            {
                unsigned char dir;
                for (dir = 0; dir < 4; dir++)
                {
                    j = i;
                    tempNextFloodVal = nextFloodVal;
                    while (canGo(dir, j) && getFloodVal(getCell(dir, j)) > tempNextFloodVal)
                    {
                        setFloodVal(j = getCell(dir, j), tempNextFloodVal++);
                        if (tempNextFloodVal > 254)
                        {
                            error("flood1");// TO DO: flag error
                            break;
                        }
                    }
                }
            }
        }
        while (++i);
    }
    
    if (curFloodVal == 252)
        error("flood2");   // TO DO: flag some sort of error
}


Cell findCellToExplore()
{
    Cell i = 0;
    Cell foundCell = 0;
    unsigned char minUnvisitedFloodVal = 255;
    
    floodFrom(curCell);
    
    do
    {
        if (!getVisited(i) && getFloodVal(i) < minUnvisitedFloodVal)
        {
            minUnvisitedFloodVal = getFloodVal(i);
            foundCell = i;
        }
    }
    while (++i);
    
    if (minUnvisitedFloodVal == 255)
        done = 1; // TO DO: we're done; the maze is fully explored
    
    return foundCell;
}


void getPathToCell(Cell destCell)
{
    Direction dir, nextDir = curDir, prevDir = curDir;
    Cell cell = curCell;
    floodFrom(destCell);
    pathLength = 0;
    unsigned char minFlood;
    unsigned char cellsSinceLastIntersection = 0;
    
    while (cell != destCell)
    {
        cellsSinceLastIntersection++;
        minFlood = getFloodVal(cell);
        if (canGo(nextDir, cell))   // if we can go in the direction we've been going, we'd like to pick that way if it has the same flood value as another direction
            minFlood = getFloodVal(getCell(nextDir, cell));
        for (dir = 0; dir < 4; dir++)
        {
            if (canGo(dir, cell) && getFloodVal(getCell(dir, cell)) < minFlood)
            {
                minFlood = getFloodVal(getCell(dir, cell));
                nextDir = dir;
            }
        }
        
        if (isIntersection(cell))
        {
            path[pathLength++] = ((nextDir - prevDir) & 0x3) + (cellsSinceLastIntersection << 4);
            if (pathLength > MAX_PATH_LENGTH)
            {
                pathLength = MAX_PATH_LENGTH;
                error("path1");// TO DO: flag an error
            }
            cellsSinceLastIntersection = 0;
        }
        else if (nextDir != prevDir)
            error("path2");   // TO DO: flag an error (cannot have a change of direction if not at an intersection)
        
        cell = getCell(nextDir, cell);
        prevDir = nextDir;
    }
}


unsigned char updateCell(unsigned char left, unsigned char straight, unsigned char right)
{
    if (!getVisited(curCell))
    {
        setVisited(curCell);
        if (left)
            addPath(curDir - 1, curCell);
        if (straight)
            addPath(curDir, curCell);
        if (right)
            addPath(curDir + 1, curCell);
    }
    
    // verify that what we're seeing matches our current characterization of the intersection (a return value of 0 means there was a problem)
    return ((left == canGo(curDir - 1, curCell)) &&
            (straight == canGo(curDir, curCell)) &&
            (right == canGo(curDir + 1, curCell)) &&
            canGo(curDir + 2, curCell));      // the path we used to enter the cell should always be there!
}





/****************************************************************************************************************************************************/


int idx = 0;
unsigned char useTurnsArray = 0;
unsigned char runSpeed;
unsigned long intersectionTime;


const char rhapsody[] PROGMEM = "O6 T40 L16 d#<b<f#<d#<f#<bd#f#"
"T80 c#<b-<f#<c#<f#<b-c#8"
"T180 d#b<f#d#f#>bd#f#c#b-<f#c#f#>b-c#8 c>c#<c#>c#<b>c#<c#>c#c>c#<c#>c#<b>c#<c#>c#"
"c>c#<c#>c#<b->c#<c#>c#c>c#<c#>c#<b->c#<c#>c#"
"c>c#<c#>c#f>c#<c#>c#c>c#<c#>c#f>c#<c#>c#"
"c>c#<c#>c#f#>c#<c#>c#c>c#<c#>c#f#>c#<c#>c#d#bb-bd#bf#d#c#b-ab-c#b-f#d#";

const char LcdCustomChar[] PROGMEM =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, // 1. 1/8 full progress block
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, // 2. 2/8 full progress block
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, // 3. 3/8 full progress block
    0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, // 4. 4/8 full progress block
    0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, // 5. 5/8 full progress block
    0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, // 6. 6/8 full progress block
    0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, // 7. 7/8 full progress block
};

void display_readings(const unsigned int *calibrated_values)
{
    unsigned char i;
    
    for(i=0;i<5;i++) {
        unsigned char c = calibrated_values[i]/112;
        if (c == 0)
            print_character(' ');
        else if (c == 8)
            print_character((char)255);
        else
            print_character(c);
    }
}



void celebrate()
{
    play_from_program_space(rhapsody);
    wait_for_button(ALL_BUTTONS);
    stop_playing();
}


void turnLeft()
{
    int i = 0;
    set_motors(0, 0);
    delay(10);
    for (i = 0; i < 170; i += 5)
    {
        set_motors(-i, i);
        delay(2);
    }
    set_motors(-170, 170);
    delay(52);   // 60 ms is about perfect in stationary tests (slightly overshoots when maze-running)
    
    set_motors(0, 0);
    delay(15);
    curDir--;
}



void turnRight()
{
    int i = 0;
    set_motors(0, 0);
    delay(10);
    for (i = 0; i < 170; i += 5)
    {
        set_motors(i, -i);
        delay(2);
    }
    set_motors(170, -170);
    delay(52);   // 60 ms is about perfect in stationary tests (slightly overshoots when maze-running)
    
    set_motors(0, 0);
    delay(15);
    curDir++;
}


void turnAround()
{
    int i = 0;
    set_motors(0, 0);
    delay(10);
    for (i = 0; i < 170; i += 5)
    {
        set_motors(-i, i);
        delay(2);
    }
    set_motors(-170, 170);
    delay(145);
    
    set_motors(0, 0);
    delay(15);
    curDir += 2;
}


// spin 360 and look at all paths into and out of the current cell
// (used to figure out the state of the starting cell)
void characterizeCell()
{
    unsigned int sensors[5];
    unsigned long time = millis();
    
    set_motors(-100, 100);
    while (millis() - time < 620)
    {
        read_line(sensors, IR_EMITTERS_ON);
        if (sensors[2] > 300)
            addPath(curDir + (millis() - time + 75) / 155, curCell);
        
    }
    set_motors(0, 0);
    
    /*  // this code turns the 3pi to the closest direction it can go
     // it is commented out since it should always be able to go "south"
     // (why would you ever place it facing a direction it can't go?)
     if (!canGo(curDir, curCell))
     {
     if (canGo(curDir + 1, curCell))
     turnRight();
     else if (canGo(curDir - 1, curCell))
     turnLeft();
     else if (canGo(curDir + 2, curCell))
     turnAround();
     else
     while (1);
     }
     */
}


// compute the number of maze cells traversed based on the time since we left the last intersection
unsigned char getCellsFromTime(unsigned long intersectionTime)
{
    return (unsigned char)(((unsigned int)(millis() - intersectionTime) + 150) / 400);
}


void exploreMaze()
{
    red_led(0);
    green_led(0);
    
    if (!fast)
    {
        characterizeCell();
        delay(25);
    }
    
    unsigned int sensors[5];
    unsigned int last_position = read_line(sensors, IR_EMITTERS_ON);
    unsigned int left, right, straight;
    unsigned long time = millis();
    intersectionTime = millis();
    
    unsigned char pathIdx = 0;
    unsigned char count = 0;
    
    
    while(1)
    {
        if (button_is_pressed(ALL_BUTTONS))
        {
            wait_for_button_release(ALL_BUTTONS);
            return;
        }
        left = 0;
        right = 0;
        straight = 0;
        int position = (int)read_line(sensors, IR_EMITTERS_ON) - 2000;
        
        // if we see something on the left, something on the right, or nothing on any of our sensors (i.e. if we've hit an intersection)
        if ((sensors[0] >= 200 || sensors[4] >= 200 ||
             (sensors[0] < 300 && sensors[1] < 300 && sensors[2] < 300 && sensors[3] < 300 && sensors[4] < 300)) &&
            millis() - time > 200)      // don't check for next intersection until at least 200 ms have elapsed
        {
            //set_motors(100, 100);   // drive straight at a slow speed through the intersection (stop following the line temporarily)
            time = millis();            // store the time
            // watch sensors for the next 70 ms to characterize the intersection
            while (millis() - time < 50)  // was 65 when it worked best (at last contest that wasn't in december 2008)
            {
                last_position = read_line(sensors, IR_EMITTERS_ON);
                //if ((sensors[0] < 100 && sensors[1] < 100 && sensors[2] < 100 && sensors[3] < 100 && sensors[4] < 100) && (millis() - time > 50))
                //   break;
                
                if (sensors[0] > 500 && left < 255)
                    left++;
                if (sensors[4] > 500 && right < 255)
                    right++;
            }
            // if see all black after 100 ms, we need to flag the finish cell
            if (sensors[1] > 500 && sensors[2] > 500 && sensors[3] > 500)
            {
                if (!fast)
                    play("O6 T40 L16 d#<b<f#");
                left = 0;
                right = 0;
            }
            else if (sensors[1] + sensors[2] + sensors[3] > 300)
                straight = 1;
            
            if (getCellsFromTime(intersectionTime) == count + 1)
            {
                play_note(A(4), 80, 10);
            }
            
            if (!fast)
            {
                delay_ms(10);
                set_motors(0, 0);      // TO DO: is this necessary?
                
                clear();
                if (left >= 3) print("L");
                if (straight)
                {
                    lcd_goto_xy(3, 0);
                    print("S");
                }
                if (right >= 3)
                {
                    lcd_goto_xy(7, 0);
                    print("R");
                }
                if (left < 3 && !straight && right < 3)
                {
                    lcd_goto_xy(4, 0);
                    print("B");
                }
                
                unsigned char numCells = getCellsFromTime(intersectionTime);
                if (numCells == 0)
                    numCells = 1;
                unsigned char i = numCells;
                
                while (1)
                {
                    curCell = getCell(curDir, curCell);
                    if (--i == 0)
                        break;
                    if (!updateCell(0, 1, 0))         // update the intersectionless grid cells
                        error("valid1");   // TO DO: flag an error;
                }
                
                lcd_goto_xy(0, 1);
                print("(");
                print_hex_byte(curCell & 0xF);
                print(", ");
                print_hex_byte(curCell >> 4);
                print(")");
                
                //delay(100);
                
                
                if (!updateCell(left >= 3, straight, right >= 3))   // marks cell as visited, checks against previous learning if this is a re-visit
                    error("valid2");   // TO DO: flag an error
                
                // if see all black after 100 ms, we need to flag the finish cell
                if (sensors[1] > 500 && sensors[2] > 500 && sensors[3] > 500)
                {
                    endCell = curCell;
                    eeprom_write_byte((unsigned char *)256, endCell);
                }
                
            }
            
            if (pathIdx >= pathLength)
            {
                pathIdx = 0;
                
                if (done)
                {
                    set_motors(0, 0);
                    if (fast)
                    {
                        play_from_program_space(rhapsody);
                        //return;
                    }
                    else
                    {
                        clear();
                        print("(");
                        print_hex_byte(curCell & 0xF);
                        print(", ");
                        print_hex_byte(curCell >> 4);
                        print(")");
                        lcd_goto_xy(0, 1);
                        print("(");
                        print_hex_byte(endCell & 0xF);
                        print(", ");
                        print_hex_byte(endCell >> 4);
                        print(")");
                        
                        if ((curDir & 3) == WEST)
                            turnLeft();
                        else if ((curDir & 3) == EAST)
                            turnRight();
                        else if ((curDir & 3) == NORTH)
                            turnAround();
                        play("!v12 L64 o4 cdefgab");
                        while (is_playing());
                        
                        //return;
                        //getPathToCell(endCell);
                        fast = 1;
                    }
                    curCell = 0;
                    curDir = SOUTH;
                    do
                    {
                        checkForSerial();
                    }
                    while (!button_is_pressed(ALL_BUTTONS));
                    stop_playing();
                    delay_ms(10);
                    do
                    {
                        checkForSerial();
                    }
                    while (button_is_pressed(ALL_BUTTONS));
                    delay_ms(10);
                    
                    getPathToCell(endCell);
                    
                    clear();
                    
                    //buzzer.play("!L16 cdegreg4");
                    
                    play_note(A(4), 200, 15);
                    delay(500);
                    play_note(A(4), 200, 15);
                    delay(500);
                    play_note(A(5), 300, 15);
                    delay(300);
                    
                }
                else
                    getPathToCell(findCellToExplore());
            }
            
            if ((path[pathIdx] & 3) == BACK)
            {
                turnAround();
            }
            if ((path[pathIdx] & 3) == RIGHT)
            {
                turnRight();
            }
            if ((path[pathIdx] & 3) == LEFT)
            {
                turnLeft();
            }
            intersectionTime = millis();
            if ((path[pathIdx] & 3) == STRAIGHT)
                intersectionTime -= 50;
            
            
            pathIdx++;
            
            time = millis();
            last_position = read_line(sensors, IR_EMITTERS_ON);
            count = 0;
            continue;
        }
        
        int spd, M1, M2;
        
        spd = position / 10 + 2 * (position - last_position); // originally pos / 8 + 3 * diff
        if (millis() - intersectionTime > 250)
            spd >>= 2;   // after 250 ms, decrease our PID response by a factor of four
        
        M1 = 100;
        M2 = 100;
        
        int m1 = M1 + spd;
        int m2 = M2 - spd;
        if (m1 > M1)
            m1 = M1;
        if (m1 < 0)
            m1 = 0;
        if (m2 > M2)
            m2 = M2;
        if (m2 < 0)
            m2 = 0;
        set_motors(m1, m2);
        last_position = position;
        
        if (getCellsFromTime(intersectionTime) == count + 1)
        {
            play_note(A(4), 80, 10);
            count++;
        }
    }
}


unsigned char cellValue;

void serialDumpMaze()
{
    int i;
    floodFrom(endCell);
    for (i = 0; i < 256; i++)
    {
        cellValue = grid((Cell)i);
        serial_send_blocking((char *)&cellValue, 1);
    }
    serial_send_blocking((char *)flood, 128);
    serial_send_blocking((char *)flood + 128, 128);
    serial_send_blocking((char *)&endCell, 1);
}


void displayBatteryVoltage()
{
    int bat = read_battery_millivolts_3pi();
    lcd_goto_xy(0, 0);
    print_long(bat);
    print(" mV");
}


void setup()
{
    serial_set_mode(SERIAL_CHECK);
    serial_set_baud_rate(38400);
    lcd_load_custom_character(LcdCustomChar, 1);
    lcd_load_custom_character(LcdCustomChar + 8, 2);
    lcd_load_custom_character(LcdCustomChar + 16, 3);
    lcd_load_custom_character(LcdCustomChar + 24, 4);
    lcd_load_custom_character(LcdCustomChar + 32, 5);
    lcd_load_custom_character(LcdCustomChar + 40, 6);
    lcd_load_custom_character(LcdCustomChar + 48, 7);
    clear();
    pololu_3pi_init(2000);
    red_led(1);
    green_led(1);
    
    setVisited(curCell);
    //   addPath(SOUTH, curCell);
}


void loop()
{
    unsigned int counter;
    unsigned char button;
    
    
    curCell = 0;
    curDir = SOUTH;
    
    
    if (!done)
    {
        clear();
        lcd_goto_xy(0,1);
        print("press B");
        
        endCell = eeprom_read_byte((unsigned char *)256);
        
        // display battery voltage until button B is pressed
        while(!button_is_pressed(BUTTON_B))
        {
            displayBatteryVoltage();
            checkForSerial();
            delay(50);
        }
        
        wait_for_button_release(ALL_BUTTONS);
        delay(500);
        
        int i = 0;
        for (i = 0; i < 256; i++)
            eeprom_write_byte((unsigned char *)i, 0xFF);
        
        // auto-calibration
        for(counter = 0; counter < 80; counter++)
        {
            if(counter < 20 || counter >= 60)
                set_motors(80,-80);
            else
                set_motors(-80,80);
            
            calibrate_line_sensors(IR_EMITTERS_ON);
            
            delay(5);
        }
        set_motors(0,0);
        
        while(!button_is_pressed(ALL_BUTTONS))
        {
            unsigned int sensors[5];
            unsigned int position = read_line(sensors, IR_EMITTERS_ON);
            
            clear();
            print_unsigned_long(position);
            lcd_goto_xy(0,1);
            display_readings(sensors);
            
            delay(50);
        }
        
        wait_for_button_release(ALL_BUTTONS);
    }
    else
    {
        do
        {
            // display the battery voltage for 1 second
            unsigned long time = millis();
            while (millis() - time < 1000)
            {
                displayBatteryVoltage();
                delay(50);
                checkForSerial();
            }
            
            clear();
            print("B: Maze");
            lcd_goto_xy(0, 1);
            print("C: Bat V");
            
            do
            {
                button = button_is_pressed(ALL_BUTTONS);
                checkForSerial();
            }
            while (!button);
            delay_ms(10);
            do
            {
                checkForSerial();
            }
            while (button_is_pressed(button));
            delay_ms(10);
            
            //button = wait_for_button(ALL_BUTTONS);
            if (button == BUTTON_C)
            {
                clear();
                lcd_goto_xy(0, 1);
                print("voltage");
            }
        }
        while (button != BUTTON_B);
        curCell = 0;
        curDir = SOUTH;
        getPathToCell(endCell);
        fast = 1;
    }
    
    // display calibrated values
    
    
    clear();
    
    //buzzer.play("!L16 cdegreg4");
    
    play_note(A(4), 200, 15);
    delay(500);
    play_note(A(4), 200, 15);
    delay(500);
    play_note(A(5), 300, 15);
    delay(300);
    
    //if (!done)
    exploreMaze();
    
    set_motors(0, 0);
    wait_for_button(ALL_BUTTONS);
    stop_playing();
    clear();
}



/****************************************************************************************************************************************************/






int main()
{
    setup();
    
    while (1)
    {
        loop();
    }
    
    return 0;
}