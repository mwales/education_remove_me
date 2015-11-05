# program template for Spaceship
import simplegui
import math
import random

# globals for user interface
WIDTH = 800
HEIGHT = 600
score = 0
lives = 3
time = 0
game_mode = 0 # 0 = splash screen, 1 = game mode, 2 = game over

ANGULAR_ACCEL_SCALAR = math.pi / 800.0
ANGULAR_FRICTION = 0.95
LINEAR_ACCEL_SCALAR = 0.25
LINEAR_FRICTION = 0.99
RANDOM_VEL_MAX = 4.0
RANDOM_VEL_MIN = 0.5
RANDOM_ANG_MAX = math.pi / 100.0
BULLET_VEL = 10
SMALL_ROCK_SPEED = 3

class ImageInfo:
    def __init__(self, center, size, radius = 0, lifespan = None, animated = False):
        self.center = center
        self.size = size
        self.radius = radius
        if lifespan:
            self.lifespan = lifespan
        else:
            self.lifespan = float('inf')
        self.animated = animated

    def get_center(self):
        return self.center

    def get_size(self):
        return self.size

    def get_radius(self):
        return self.radius

    def get_lifespan(self):
        return self.lifespan

    def get_animated(self):
        return self.animated

    
# art assets created by Kim Lathrop, may be freely re-used in non-commercial projects, please credit Kim
    
# debris images - debris1_brown.png, debris2_brown.png, debris3_brown.png, debris4_brown.png
#                 debris1_blue.png, debris2_blue.png, debris3_blue.png, debris4_blue.png, debris_blend.png
debris_info = ImageInfo([320, 240], [640, 480])
debris_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/debris2_blue.png")

# nebula images - nebula_brown.png, nebula_blue.png
nebula_info = ImageInfo([400, 300], [800, 600])
nebula_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/nebula_blue.f2014.png")

# splash image
splash_info = ImageInfo([200, 150], [400, 300])
splash_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/splash.png")

# ship image
ship_info = ImageInfo([45, 45], [90, 90], 35)
ship_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/double_ship.png")

# missile image - shot1.png, shot2.png, shot3.png
missile_info = ImageInfo([5,5], [10, 10], 3, 75)
missile_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/shot2.png")

# asteroid images - asteroid_blue.png, asteroid_brown.png, asteroid_blend.png
asteroid_info = ImageInfo([45, 45], [90, 90], 40)
asteroid_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/asteroid_blue.png")

# animated explosion - explosion_orange.png, explosion_blue.png, explosion_blue2.png, explosion_alpha.png
explosion_info = ImageInfo([64, 64], [128, 128], 17, 24, True)
explosion_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/explosion_alpha.png")

# sound assets purchased from sounddogs.com, please do not redistribute
soundtrack = simplegui.load_sound("http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/soundtrack.mp3")
missile_sound = simplegui.load_sound("http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/missile.mp3")
missile_sound.set_volume(.5)
ship_thrust_sound = simplegui.load_sound("http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/thrust.mp3")
explosion_sound = simplegui.load_sound("http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/explosion.mp3")

## CC 3.0 sound file by Jesus Lastra, http://opengameart.org/content/8-bit-pickup-1
extra_life_sound = simplegui.load_sound("http://mwales.net/junk/SFX_Pickup_44.mp3")

# helper functions to handle transformations
def angle_to_vector(ang):
    return [math.cos(ang), math.sin(ang)]

def vector_to_angle(v):
    return math.atan2(v[0],v[1])

def vector_scale(vec, scale):
    return [vec[0] * scale, vec[1] * scale]

def vector_add(vec1, vec2):
    return [vec1[0] + vec2[0], vec1[1] + vec2[1]]

def dist(p,q):
    return math.sqrt((p[0] - q[0]) ** 2+(p[1] - q[1]) ** 2)


def smallRockExplode(rockInstance):
    # Return an explosion sprite
    explodeObj = Sprite(rockInstance.get_position(), 
                           (0,0), 
                           random.random() * 2 * math.pi, 
                           0, 
                           explosion_image,
                           explosion_info,
                           explosion_sound,
                           relSize = 0.3)
    return explodeObj


def rockExplode(rockInstance, deathBullet):
    # Return an explosion sprite
    explodeObj = Sprite(rockInstance.get_position(), 
                           (0,0), 
                           random.random() * 2 * math.pi, 
                           0, 
                           explosion_image,
                           explosion_info,
                           explosion_sound)
    
    # Create 4 smaller rocks that explode away based on angle bullet came in at
    bulletAngle = vector_to_angle(deathBullet.get_velocity())
    
    smallRockAngle = bulletAngle + 45.0 / 360.0 * math.pi * 2.0
    for i in range(0,4):
        smallRockAngle += math.pi / 2.0
        smallRockVel = angle_to_vector(smallRockAngle)
        smallRockVel = vector_scale(smallRockVel, SMALL_ROCK_SPEED)
        smallRockVel = vector_add(smallRockVel, rockInstance.get_velocity())
        randomAngVel = random.random() * RANDOM_ANG_MAX * 4.0 - RANDOM_ANG_MAX
        smallRock = Sprite(rockInstance.get_position(),
                           smallRockVel,
                           random.random() * 2 * math.pi,
                           randomAngVel,
                           asteroid_image, 
                           asteroid_info, 
                           relSize = 0.5)
        smallRockList.append(smallRock)
                           
    
    return explodeObj

# Ship class
class Ship:
    def __init__(self, pos, vel, angle, image, info, bulletTimer):
        self.pos = [pos[0],pos[1]]
        self.vel = [vel[0],vel[1]]
        self.thrust = False
        self.angle = angle
        self.angle_vel = 0
        self.angle_acc = 0
        self.image = image
        self.image_center = info.get_center()
        self.image_size = info.get_size()
        self.radius = info.get_radius()
        self.bullet_timer = bulletTimer
        self.spawn_bullets = False
        self.bullets = []
        self.bullet_type = 0
        self.weapon_name = {}
        self.weapon_name[0] = "Speed Shot"
        self.weapon_name[1] = "Spread Shot"
        self.weapon_name[2] = "Power Shot"
        
    def get_weapon_name(self):
        return self.weapon_name[self.bullet_type]
        
    def draw(self,canvas):
        if self.thrust:
            canvas.draw_image(self.image,
                              (self.image_center[0] + self.image_size[0], self.image_center[1]),
                              self.image_size,
                              self.pos,
                              self.image_size,
                              self.angle)
        else:
            canvas.draw_image(self.image,
                              self.image_center,
                              self.image_size,
                              self.pos,
                              self.image_size,
                              self.angle)
        
        for singleBullets in self.bullets:
            singleBullets.draw(canvas)
        

    def update(self):
        self.pos = vector_add(self.pos, self.vel)
        
        # Position should wrap around the screen
        self.pos = [self.pos[0] % WIDTH, self.pos[1] % HEIGHT]
        
        # Handle ship thrust
        if self.thrust:
            accel = angle_to_vector(self.angle)
            accel = vector_scale(accel, LINEAR_ACCEL_SCALAR)
            self.vel = vector_add(self.vel, accel)
            
        # Friction against motion
        self.vel = vector_scale(self.vel, LINEAR_FRICTION)
        
        self.angle = self.angle + self.angle_vel
        self.angle_vel = self.angle_vel + self.angle_acc
        self.angle_vel = self.angle_vel * ANGULAR_FRICTION
    
        oldBullets = []
        for singleBullets in self.bullets:
            if singleBullets.update():
                oldBullets.append(singleBullets)
                
        for bulletToDelete in oldBullets:
            self.bullets.remove(bulletToDelete)
            
         
    def process_collisions(self, rockList, smallRockList, explosionList):
        global score, lives, extra_life_sound
        
        # Don't change containers while looping through them
        shipExplodes = False
        
        rockListCopy = rockList
        
        bulletListCopy = self.bullets        
        
        for singleRock in rockListCopy:
            for singleBullet in bulletListCopy:
            # Collisions of bullets and rocks
                if singleBullet.collide(singleRock):
                    # delete the bullet
                    self.bullets.remove(singleBullet)
                    
                    # delete and explode the rock
                    if singleRock in rockList:
                        rockList.remove(singleRock)
                    
                        explosionList.append(rockExplode(singleRock, singleBullet))
                    
                        print "Rock goes boom"
                    
                        # increase score , 1-up consideration
                        self.scorePoint()                        
        
            # Collisions of rock and ship
            if singleRock.collide(self):
                #print "Ship goes boom"
                shipExplodes = True
        
        smallRockListCopy = smallRockList
        bulletListCopy = self.bullets
        for singleSmallRock in smallRockListCopy:
            for singleBullet in bulletListCopy:
                if singleBullet.collide(singleSmallRock):
                    # delete the bullet
                    self.bullets.remove(singleBullet)
                    
                    # delete and explode the rock
                    if singleSmallRock in smallRockList:
                        smallRockList.remove(singleSmallRock)
                    
                        explosionList.append(smallRockExplode(singleSmallRock))
                    
                        print "Small Rock goes boom"
                    
                        # increase score , 1-up consideration
                        self.scorePoint()                        
        
                # Collisions of rock and ship
                if singleSmallRock.collide(self):
                    #print "Ship goes boom"
                    shipExplodes = True
        
        if shipExplodes:
            self.attemptRespawn(rockList, explosionList)
        
            
    def scorePoint(self):
        global lives, score
        score += 1
        if ((score % 100) == 0):
            print "1-up"
            lives += 1
            extra_life_sound.rewind()
            extra_life_sound.play()
            
    def attemptRespawn(self, rockList, explosionList):
        global lives
        lives -= 1
        
        if (lives == 0):
            game_over()
            return
        
        # Find a safe spot to respawn
        bestLocation = []
        bestLocationClosestRock = 0
        for respawnX in range( int(WIDTH / 10), int(WIDTH * .9), 10):
            for respawnY in range( int(HEIGHT / 10), int(HEIGHT * .9), 10):
                closestRock = WIDTH * HEIGHT
                potentialLocation = [respawnX, respawnY]
                    
                # Determine at this location how close closest rock is
                for singleRock in rockList:
                    distFromRock = dist(potentialLocation, singleRock.get_position())
                    
                    if (distFromRock < closestRock):
                        closestRock = distFromRock
                        
                for singleRock in smallRockList:
                    distFromRock = dist(potentialLocation, singleRock.get_position())
                    
                    if (distFromRock < closestRock):
                        closestRock = distFromRock
                
                # If the closest rock is farther away than other locations, use this location
                if (closestRock > bestLocationClosestRock):
                    bestLocationClosestRock = closestRock
                    bestLocation = potentialLocation
        
        # Move ship to new location
        shipExplosion = Sprite(self.pos, 
                              (0,0), 
                              random.random() * 2 * math.pi, 
                              0, 
                              explosion_image,
                              explosion_info,
                              explosion_sound,
                              relSize = 3.0)
        explosionList.append(shipExplosion)
        self.pos = bestLocation
        self.vel = [0,0]
        self.angle_vel = 0
        
        
    # Just pass in -1 to rotate right, +1 to rotate left
    def rotate(self, angularAcceleration):
        self.angle_acc = angularAcceleration * ANGULAR_ACCEL_SCALAR
        #print "Alpha =" + str(self.angle_acc)
        
    # Just pass in True to thrust, False to not thrust
    def setThrust(self, thrustBool):
        global ship_thrust_sound
        
        self.thrust = thrustBool
        
        if thrustBool:
            ship_thrust_sound.rewind()
            ship_thrust_sound.play()
        else:
            ship_thrust_sound.pause()
        
    def startShooting(self):
        self.spawn_bullets = True;
        self.bullet_timer.start()
        
        self.spawn_bullet()
       
    def stopShooting(self):
        self.spawn_bullets = False
        self.bullet_timer.stop()
        
    def change_bullet_type(self):
        self.bullet_type = (self.bullet_type + 1) % 3
        
    def set_bullet_type(self, bulletType):
        self.bullet_type = bulletType % 3
        
    def get_bullet_type(self):
        return self.bullet_type
        
    def spawn_bullet(self):
        if (self.bullet_type == 0):        
            # speed shot
            self.make_bullet()
        elif (self.bullet_type == 1):
            # spread
            self.make_bullet(relSpeed=0.5)
            self.make_bullet(relAngle=-math.pi * 2 * 30.0 / 360.0, 
                             relSpeed=0.5)
            self.make_bullet(relAngle=math.pi * 2 * 30.0 / 360.0, 
                             relSpeed=0.5)
        else:
            # big bullet
            self.make_bullet(relSpeed=0.25,
                             relSize=3.0,
                             relLifetime=5.0)
            curDirection = angle_to_vector(self.angle)
            recoil = vector_scale(curDirection, -1.0)
            self.vel = vector_add(self.vel, recoil)
    
        
    def make_bullet(self, relAngle=0, relSpeed=1.0, relSize=1.0, relLifetime=1.0):
        global missle_sound
        bulletPos = angle_to_vector(self.angle)
        bulletPos = vector_scale(bulletPos, self.image_size[0] / 2)
        bulletPos = vector_add(self.pos, bulletPos)
        
        bulletVel = angle_to_vector(self.angle + relAngle)
        bulletVel = vector_scale(bulletVel, BULLET_VEL * relSpeed)
        bulletVel = vector_add(bulletVel, self.vel)

        bulletObj = Sprite(bulletPos, 
                           bulletVel, 
                           self.angle, 
                           0, 
                           missile_image,
                           missile_info,
                           missile_sound,
                           relSize,
                           relLifetime)
        self.bullets.append(bulletObj)
        
    def get_position(self):
        return self.pos
    
    def reset(self):
        self.pos = [WIDTH / 2, HEIGHT / 2]
        self.vel = [0,0]
        self.angle = 0
        self.bullets = []
    
    def get_radius(self):
        return self.radius
    
    def get_velocity(self):
        return self.vel
        

    
# Sprite class
class Sprite:
    def __init__(self, pos, vel, ang, ang_vel, image, info, sound = None, relSize=1.0, relLifetime=1.0):
        self.pos = [pos[0],pos[1]]
        self.vel = [vel[0],vel[1]]
        self.angle = ang
        self.angle_vel = ang_vel
        self.image = image
        self.image_center = info.get_center()
        self.image_size = info.get_size()
        self.draw_size = vector_scale(self.image_size, relSize)
        self.radius = info.get_radius() * relSize
        self.lifespan = info.get_lifespan() * relLifetime
        self.animated = info.get_animated()
        self.age = 0
        if sound:
            sound.rewind()
            sound.play()
   
    def draw(self, canvas):
        if self.animated:
            frameCenter = vector_add(self.image_center, [self.image_size[0] * self.age,0])
            canvas.draw_image(self.image,
                              frameCenter,
                              self.image_size,
                              self.pos,
                              self.draw_size,
                              self.angle)
        else:
            canvas.draw_image(self.image,
                              self.image_center,
                              self.image_size,
                              self.pos,
                              self.draw_size,
                              self.angle)
    
    def update(self):
        pass
        self.pos = vector_add(self.pos, self.vel)
        
        # Position should wrap around the screen
        self.pos = [self.pos[0] % WIDTH, self.pos[1] % HEIGHT]
        
        self.angle = self.angle + self.angle_vel
        
        # Age out?
        self.age += 1
        return (self.age > self.lifespan)
        
        
    def collide(self, otherObject):
        currentDistOfCenters = dist(otherObject.get_position(), 
                                    self.pos)
        minSafeDistance = (otherObject.get_radius() + \
                          self.radius) * 0.9
        return (currentDistOfCenters < minSafeDistance)
        
    def get_position(self):
        return self.pos
    
    def get_radius(self):
        return self.radius

    def get_velocity(self):
        return self.vel
    
        
def process_sprites(canvas):
    global explodeList
    
    # draw ship and sprites
    my_ship.draw(canvas)
    for singleRock in rockList:
        singleRock.draw(canvas)  
    for smallRock in smallRockList:
        smallRock.draw(canvas)
    
    # update ship and sprites
    my_ship.update()
    for singleRock in rockList:
        singleRock.update()
    for smallRock in smallRockList:
        smallRock.update()
        
    # update explosions
    splodeCopy = explodeList
    for singleSplosion in splodeCopy:
        singleSplosion.draw(canvas)
        if singleSplosion.update():
            explodeList.remove(singleSplosion)
        
        
    my_ship.process_collisions(rockList, smallRockList, explodeList)
    
    
def draw(canvas):
    global time
    
    # animiate background
    time += 1
    wtime = (time / 4) % WIDTH
    center = debris_info.get_center()
    size = debris_info.get_size()
    canvas.draw_image(nebula_image, nebula_info.get_center(), nebula_info.get_size(), [WIDTH / 2, HEIGHT / 2], [WIDTH, HEIGHT])
    canvas.draw_image(debris_image, center, size, (wtime - WIDTH / 2, HEIGHT / 2), (WIDTH, HEIGHT))
    canvas.draw_image(debris_image, center, size, (wtime + WIDTH / 2, HEIGHT / 2), (WIDTH, HEIGHT))

    if game_mode == 1:
        process_sprites(canvas)
    
    if ( (game_mode == 1) or (game_mode == 2) ):
        canvas.draw_text("Score: " + str(score),
                         (WIDTH - 250,60),
                         30,
                         'White')
        canvas.draw_text("Lives: " + str(lives),
                         (150,60),
                         30,
                         'White')
        canvas.draw_text("Weapon: " + my_ship.get_weapon_name(),
                         (WIDTH-400, HEIGHT - 50),
                         25,
                         'White',
                         'monospace')
    if game_mode == 0:
        canvas.draw_image(splash_image,
                          splash_info.get_center(),
                          splash_info.get_size(),
                          [WIDTH / 2, HEIGHT / 2],
                          splash_info.get_size())
    
    
            
# timer handler that spawns a rock    
def rock_spawner(recurseDepth = 10):
    global rockList
    
    if (len(rockList) > 12):
        print "Too many rocks"
        return
    
    randomX = random.choice(range(0, WIDTH))
    randomY = random.choice(range(0, HEIGHT))
    
    #print "Rock + " + str(recurseDepth) + "   dist = " + str(dist(my_ship.get_position(), [randomX, randomY]))
    if (dist(my_ship.get_position(), [randomX, randomY]) < 150):
        print "too close for a rock"
        if recurseDepth == 0:
            return
        else:
            rock_spawner(recurseDepth - 1)
            return
    
    randomVel = angle_to_vector(random.random() * math.pi * 2.0)
    randomVel = vector_scale(randomVel, random.random() * (RANDOM_VEL_MAX - RANDOM_VEL_MIN) + RANDOM_VEL_MIN)
    randomAngVel = random.random() * RANDOM_ANG_MAX * 2.0 - RANDOM_ANG_MAX
    
    #print "Spawn rock: [" + str(randomX) + "," + str(randomY) + "] v=" + \
    #      str(randomVel) + " Alpha=" + str(randomAngVel)
    
    spawnRock = Sprite([randomX, randomY], 
                       randomVel,
                       random.random() * math.pi * 2.0, 
                       randomAngVel, 
                       asteroid_image, 
                       asteroid_info)
    rockList.append(spawnRock)
    
    
def bullet_spawner():
    global my_ship
    
    my_ship.spawn_bullet()
    

def key_down_handler(key):
    global my_ship, game_mode
    
    if (game_mode == 1):
        if ( (key == simplegui.KEY_MAP['left']) or (key == simplegui.KEY_MAP['a']) ):
            my_ship.rotate(-1)
        elif ( (key == simplegui.KEY_MAP['right']) or (key == simplegui.KEY_MAP['d']) ):
            my_ship.rotate(1)
        elif ( (key == simplegui.KEY_MAP['up']) or (key == simplegui.KEY_MAP['w']) ):
            my_ship.setThrust(True)
        elif ( (key == simplegui.KEY_MAP['down']) or (key == simplegui.KEY_MAP['s']) ):
            pass
        elif (key == simplegui.KEY_MAP['space']):
            my_ship.startShooting()
        elif (key == simplegui.KEY_MAP['1']):
            pass
        elif (key == simplegui.KEY_MAP['2']):
            pass
        elif (key == simplegui.KEY_MAP['3']):
            pass
    elif (game_mode == 0):
        if (key == simplegui.KEY_MAP['space']):
            start_game()
    else:
        if (key == simplegui.KEY_MAP['space']):
            game_mode = 0
        
       


def key_up_handler(key):
    global my_ship
    
    if ( (key == simplegui.KEY_MAP['left']) or (key == simplegui.KEY_MAP['a']) ):
        my_ship.rotate(0)
    elif ( (key == simplegui.KEY_MAP['right']) or (key == simplegui.KEY_MAP['d']) ):
        my_ship.rotate(0)
    elif ( (key == simplegui.KEY_MAP['up']) or (key == simplegui.KEY_MAP['w']) ):
        my_ship.setThrust(False)
    elif ( (key == simplegui.KEY_MAP['down']) or (key == simplegui.KEY_MAP['s']) ):
        my_ship.change_bullet_type()
    elif (key == simplegui.KEY_MAP['space']):
        my_ship.stopShooting()
    elif (key == simplegui.KEY_MAP['1']):
        my_ship.set_bullet_type(0)
    elif (key == simplegui.KEY_MAP['2']):
        my_ship.set_bullet_type(1)
    elif (key == simplegui.KEY_MAP['3']):
        my_ship.set_bullet_type(2)

def game_over():
    global my_ship, rockList, smallRockList, timer, game_mode, soundtrack
    rockList = []
    smallRockList = []
    timer.stop()
    game_mode = 2
    soundtrack.pause()
    
def start_game():
    global timer, game_mode, lives, score, soundtrack
    
    my_ship.reset()
    timer.start()
    
    game_mode = 1
    lives = 3
    score = 0
    
    soundtrack.rewind()
    soundtrack.play()
        
def mouse_handler(position):
    if (game_mode == 0):
        start_game()
        
# initialize frame
frame = simplegui.create_frame("Asteroids", WIDTH, HEIGHT)
frame.set_keydown_handler(key_down_handler)
frame.set_keyup_handler(key_up_handler)
frame.set_mouseclick_handler(mouse_handler)
frame.add_label("A/D or Left/Right to rotate")
frame.add_label("W or Up to thrust")
frame.add_label("S or Down to change weapon")
frame.add_label("1,2,3 are weapon hot key")

# initialize ship and two sprites
bulletSpawnerTimer = simplegui.create_timer(200, bullet_spawner)
my_ship = Ship([WIDTH / 2, HEIGHT / 2], [0, 0], math.pi, ship_image, ship_info, bulletSpawnerTimer)

rockList = []
smallRockList = []
explodeList = []

# register handlers
frame.set_draw_handler(draw)

timer = simplegui.create_timer(1000.0, rock_spawner)


# get things rolling
frame.start()

