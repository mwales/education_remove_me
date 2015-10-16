# Implementation of classic arcade game Pong

import simplegui
import random
import math

# initialize globals - pos and vel encode vertical info for paddles
WIDTH = 600
HEIGHT = 400       
BALL_RADIUS = 20
PAD_WIDTH = 8
PAD_HEIGHT = 80
HALF_PAD_WIDTH = PAD_WIDTH / 2
HALF_PAD_HEIGHT = PAD_HEIGHT / 2
LEFT = False
RIGHT = True
UPDATE_RATE = 1000 / 60
PADDLE_SPEED = 200
REFLECTION_SPEEDUP = -1.2

# Definitions of globals
ball_pos = [0.0,0.0] # x,y pixel coordinate
ball_vel = [0,0,250] # xComp, yComp, magnitude
paddle1_pos = [HALF_PAD_WIDTH, HEIGHT/2]
paddle2_pos = [WIDTH-HALF_PAD_WIDTH, HEIGHT/2]

# initialize ball_pos and ball_vel for new bal in middle of table
# if direction is RIGHT, the ball's velocity is upper right, else upper left
def spawn_ball(direction):
    global ball_pos, ball_vel # these are vectors stored as lists

    # ball is in center court
    ball_pos[0] = WIDTH  / 2
    ball_pos[1] = HEIGHT / 2
    
    # The angle that the ball should spawn at -15 to -60 (upward)
    spawnAngle = random.random() * -45.0 - 15.0
    #print "Spawn angle =", spawnAngle
    
    # Reset ball speed to start speed
    ball_vel[2] = 200
    
    ball_vel[0] = ball_vel[2] * math.cos(spawnAngle / 360.0 * (math.pi * 2))
    ball_vel[1] = ball_vel[2] * math.sin(spawnAngle / 360.0 * (math.pi * 2))
    
    # If spawn is left, reverse x component
    if direction == LEFT:
        ball_vel[0] *= -1.0
        
    #print "Ball velocity mag=" + str(ball_vel[2]) + \
    #      ", xComp=" + str(ball_vel[0]) + \
    #      ", yComp=" + str(ball_vel[1])

# define event handlers
def new_game():
    global paddle1_pos, paddle2_pos, paddle1_vel, paddle2_vel  # these are numbers
    global score1, score2  # these are ints
    paddle1_vel = 0
    paddle2_vel = 0
    paddle1_pos = [HALF_PAD_WIDTH, HEIGHT/2]
    paddle2_pos = [WIDTH-HALF_PAD_WIDTH, HEIGHT/2]
    score1 = 0
    score2 = 0
    spawn_ball(random.choice([LEFT, RIGHT]))

def draw(canvas):
    global score1, score2, paddle1_pos, paddle2_pos, ball_pos, ball_vel
 
        
    # draw mid line and gutters
    canvas.draw_line([WIDTH / 2, 0],[WIDTH / 2, HEIGHT], 1, "White")
    canvas.draw_line([PAD_WIDTH, 0],[PAD_WIDTH, HEIGHT], 1, "White")
    canvas.draw_line([WIDTH - PAD_WIDTH, 0],[WIDTH - PAD_WIDTH, HEIGHT], 1, "White")
        
    # draw ball
    canvas.draw_circle(ball_pos, BALL_RADIUS, 6, 'Red', 'White')
    
    # draw paddles
    drawPaddle(canvas, paddle1_pos[0], paddle1_pos[1])
    drawPaddle(canvas, paddle2_pos[0], paddle2_pos[1])
    
    # draw scores
    canvas.draw_text(str(score1), (WIDTH/2 - 100, 100), 25, 'White')
    canvas.draw_text(str(score2), (WIDTH/2 + 100, 100), 25, 'White')
    
    
def drawPaddle(canvas, x, y):
    topLeft  = (x - HALF_PAD_WIDTH, y + HALF_PAD_HEIGHT)
    topRight = (x + HALF_PAD_WIDTH, y + HALF_PAD_HEIGHT)
    botLeft  = (x - HALF_PAD_WIDTH, y - HALF_PAD_HEIGHT)
    botRight = (x + HALF_PAD_WIDTH, y - HALF_PAD_HEIGHT)
    canvas.draw_polygon([topLeft, topRight, botRight, botLeft], 2, 'White', 'Orange')
    
        
def keydown(key):
    global paddle1_vel, paddle2_vel
    print "keydown"
    
    if (key == simplegui.KEY_MAP['w']):
        paddle1_vel = -PADDLE_SPEED
    elif (key == simplegui.KEY_MAP['s']):
        paddle1_vel = PADDLE_SPEED
    elif (key == simplegui.KEY_MAP['up']):
        paddle2_vel = -PADDLE_SPEED
    elif (key == simplegui.KEY_MAP['down']):
        paddle2_vel = PADDLE_SPEED
    
    return
   
def keyup(key):
    global paddle1_vel, paddle2_vel
          
    if ( (key == simplegui.KEY_MAP['w']) or \
         (key == simplegui.KEY_MAP['s']) ):
        paddle1_vel = 0
    elif ( (key == simplegui.KEY_MAP['up']) or \
           (key == simplegui.KEY_MAP['down']) ):
        paddle2_vel = 0
    
def updatePosition():
    global ball_pos, ball_vel
    ball_pos[0] = ball_pos[0] + ball_vel[0] * UPDATE_RATE / 1000;
    ball_pos[1] = ball_pos[1] + ball_vel[1] * UPDATE_RATE / 1000;
    paddle1_pos[1] = paddle1_pos[1] + paddle1_vel * UPDATE_RATE / 1000;
    paddle2_pos[1] = paddle2_pos[1] + paddle2_vel * UPDATE_RATE / 1000;
    
    # Did ball leave the game field (top or bottom)?
    if (ball_pos[1] < BALL_RADIUS):
        ball_vel[1] = - ball_vel[1]
        ball_pos[1] = BALL_RADIUS + BALL_RADIUS - ball_pos[1]
    elif (ball_pos[1] > (HEIGHT - BALL_RADIUS)):
        ball_vel[1] = - ball_vel[1]
        ball_pos[1] = (HEIGHT - BALL_RADIUS)
          
    # Are the paddles trying to leave the game field
    if (paddle1_pos[1] < HALF_PAD_HEIGHT):
        paddle1_pos[1] = HALF_PAD_HEIGHT
    elif(paddle1_pos[1] > (HEIGHT - HALF_PAD_HEIGHT) ):
        paddle1_pos[1] = HEIGHT - HALF_PAD_HEIGHT
    
    if (paddle2_pos[1] < HALF_PAD_HEIGHT):
        paddle2_pos[1] = HALF_PAD_HEIGHT
    elif(paddle2_pos[1] > (HEIGHT - HALF_PAD_HEIGHT) ):
        paddle2_pos[1] = HEIGHT - HALF_PAD_HEIGHT
        
    # Did ball hit gutter?
    if (ball_pos[0] < BALL_RADIUS + PAD_WIDTH):
        # Left gutter hit.  Reflect or score point?
        if ( ( ball_pos[1] > paddle1_pos[1] - HALF_PAD_HEIGHT) and \
             ( ball_pos[1] < paddle1_pos[1] + HALF_PAD_HEIGHT) ):
            # Ball hit the paddle, reflect it
            print "Good hit left player!"
            ball_vel[0] = ball_vel[0] * REFLECTION_SPEEDUP
            ball_pos[0] = BALL_RADIUS + PAD_WIDTH
        else:
            # Right player just scored
            print "Right player scores!  GOOAAAAAALLLLLL!!!!"
            updateScore(RIGHT)
    elif (ball_pos[0] > (WIDTH- BALL_RADIUS - PAD_WIDTH) ):
        # Right gutter hit, reflect or score?
        if ( ( ball_pos[1] > paddle2_pos[1] - HALF_PAD_HEIGHT) and \
             ( ball_pos[1] < paddle2_pos[1] + HALF_PAD_HEIGHT) ):
            # Ball hit the paddle, reflect it
            print "Good hit right player!"
            ball_vel[0] = ball_vel[0] * REFLECTION_SPEEDUP
            ball_pos[0] = WIDTH - BALL_RADIUS - PAD_WIDTH
        else:
            # Right player just scored
            print "Left player scores!  GOOAAAAAALLLLLL!!!!"
            updateScore(LEFT)
        
def updateScore(scoringSide):
    global score1, score2
    
    if (scoringSide == LEFT):
        score1 += 1
        spawn_ball(LEFT)
    else:
        score2 += 1
        spawn_ball(RIGHT)

# create frame
frame = simplegui.create_frame("Pong", WIDTH, HEIGHT)

frame.add_label('Left player controls:')
frame.add_label('  w = up, s = down')
frame.add_label(' ')
frame.add_label('Right player controls:')
frame.add_label('  up-arrow = up')
frame.add_label('  down-arrow = down')
frame.add_label(' ')
frame.add_button('Reset', new_game, 100)

frame.set_draw_handler(draw)
frame.set_keydown_handler(keydown)
frame.set_keyup_handler(keyup)
timer = simplegui.create_timer(UPDATE_RATE, updatePosition)
timer.start()


# start frame
new_game()
frame.start()

