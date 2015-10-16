# template for "Stopwatch: The Game"
import simplegui
import math

# define global variables
clockRunning = False
clockTicks = 0
wins = 0
stops = 0

# define helper function format that converts time
# in tenths of seconds into formatted string A:BC.D
def format(ticks):
    fractionals = ticks % 10
    seconds = ticks / 10
    secsToPrint = seconds % 60
    minutes = seconds / 60
    
    retVal = str(minutes) + ":"
    
    if (secsToPrint <= 9):
        retVal += "0" + str(secsToPrint)
    else:
        retVal += str(secsToPrint)
        
    retVal += ":" + str(fractionals)
    
    return retVal

# define event handlers for buttons; "Start", "Stop", "Reset"
def startHandler():
    global clockRunning
    clockRunning = True
    
def stopHandler():
    global clockRunning, stops, wins
    
    # Only count the stop if the clock was running
    if (clockRunning):
        stops += 1
        if ( (clockTicks % 10) == 0):
            wins += 1
    
    
    clockRunning = False

    
def resetHandler():
    global clockRunning, clockTicks, wins, stops
    clockRunning = False
    clockTicks = 0
    stops = 0
    wins = 0
    
# define event handler for timer with 0.1 sec interval
def timerHandler():
    global clockTicks
    if (clockRunning):
        clockTicks += 1    

        
# define draw handler
def draw_analog_clock(canvas, coordCenter, radius, divisions, value):
    canvas.draw_circle(coordCenter, radius, 3, 'blue')
    x = radius * math.cos(value * math.pi * 2 / divisions - math.pi / 2)
    y = radius * math.sin(value * math.pi * 2 / divisions - math.pi / 2)
    
    endpoint = (coordCenter[0] + x, coordCenter[1] + y)
    
    canvas.draw_line(coordCenter, endpoint, 5, 'white')

def sw_draw(canvas):
    canvas.draw_text(format(clockTicks), (105,200), 75, 'white')
    
    scoreStr = "Wins: " + str(wins) + \
               "     Games: " + str(stops)
        
    canvas.draw_text(scoreStr, (150,50), 25, 'white')
    
    ticks = clockTicks % 10
    secs = (clockTicks / 10) % 60
    mins = (clockTicks / 600) % 10
    
    #min
    draw_analog_clock(canvas, (75, 300), 50, 10, mins)
    
    #sec
    draw_analog_clock(canvas, (200, 300), 50, 60, secs)
    
    #tenths
    draw_analog_clock(canvas, (325, 300), 50, 10, ticks)

        
# create frame
frame = simplegui.create_frame("Stop Watch", 400, 400, 150)
frame.add_button("Start", startHandler, 125)
frame.add_button("Stop", stopHandler, 125)
frame.add_button("Reset", resetHandler, 125)

timer1 = simplegui.create_timer(100, timerHandler)


# register event handlers
frame.set_draw_handler(sw_draw)

# start frame
frame.start()
timer1.start()


# Please remember to review the grading rubric

