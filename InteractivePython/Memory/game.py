# implementation of card game - Memory

import simplegui
import random

# important constants
GAME_ASSETS = "http://mwales.net/junk/memory-cards/"
PADDING = 25
CARD_WIDTH = 150
CARD_HEIGHT = 200

IMAGE_NAME_MAP = { "samus-1.jpg":   "samus-2.jpg",    # Heather 1337
                   "rogue-1.jpg":   "rogue-2.jpg",    # Ainlina @ deviantart
                   "fett-1.jpg" :   "fett-2.jpg",     # Heather 1337
                   "zelda-1.jpg":   "zelda-2.jpg",
                   "ewok-1.jpg" :   "ewok-2.jpg",     # Heather 1337
                   "sgirl-1.jpg":   "sgirl-2.jpg",    # Heather 1337
                   "psylocke-1.jpg":"psylocke-2.jpg", # Jenifer Ann
                   "rebel-1.jpg":   "rebel-2.jpg" }   # Heather 1337

imageList = []
imageMap = { }
showCard = [ False ]  * 16

guesses = []
matchAttempts = 0

def loadSounds():
    global matchSound
    global failSound
    global victorySound
    matchSound = simplegui.load_sound(GAME_ASSETS + "win.ogg")
    failSound = simplegui.load_sound(GAME_ASSETS + "lose.ogg")
    victorySound = simplegui.load_sound(GAME_ASSETS + "goal.ogg")

def loadImages():
    global imageMap
    global imageList
    global facedownImage    
    
    # Make 1 single list of all image names
    for key in IMAGE_NAME_MAP:
        print "Adding " + key + " to image list"
        print "Adding " + IMAGE_NAME_MAP[key] + " to image list"
        imageList.append(key)
        imageList.append(IMAGE_NAME_MAP[key])
        
    # Load each image
    for imageName in imageList:
        imageMap[imageName] = simplegui.load_image(GAME_ASSETS + imageName)
        
    # Load the facedown image - from weheartnintendo.tumblr
    facedownImage = simplegui.load_image(GAME_ASSETS + "unturned.jpg")
    
    

# helper function to initialize globals
def new_game():
    global showCard
    global guesses
    global matchAttempts
    
    showCard = [ False ]  * 16
    guesses = []
    matchAttempts = 0

    # Shuffle cards!
    random.shuffle(imageList)
    
    updateAttempts()
     
# define event handlers
def mouseclick(pos):
    # add game state logic here
    (posX, posY) = pos
    # print "Position:", posX, posY
    
    # Figure out coordinates of each card, and determine if click
    # is within the card or not
    for cardNumber in range(0,len(imageList)):
        cardX = cardNumber % 4
        cardY = cardNumber / 4
        
        cardMinX = PADDING * (1 + cardX) + CARD_WIDTH * cardX
        cardMaxX = cardMinX + CARD_WIDTH
        
        cardMinY = PADDING * (1 + cardY) + CARD_HEIGHT * cardY
        cardMaxY = cardMinY + CARD_HEIGHT
        
        if ( (posX >= cardMinX) and (posX <= cardMaxX) and
             (posY >= cardMinY) and (posY <= cardMaxY) ):
            # User clicked this card!
            cardClicked(cardNumber)
            return
    
    print "Try clicking a card next time!"

def cardClicked(cardNumber):
    global guesses
    global label
    global matchAttempts
    
    print "Card clicked: ", cardNumber
    
    if showCard[cardNumber]:
        # Card is already face up, doesn't count
        print "Try clicking on a face down card instead"
        return
    
    if (len(guesses) == 2):
        # Clear out the wrong guesses
        print "Flipping bad guesses back face down"
        flipMe = guesses.pop()
        showCard[imageList.index(flipMe)] = False
        flipMe = guesses.pop()
        showCard[imageList.index(flipMe)] = False
        
    guesses.append(imageList[cardNumber])
    showCard[cardNumber] = True
    
    if (len(guesses) == 2):
        # Need to determine if user picked a match or not
        matchAttempts += 1
        guesses = sorted(guesses)
        updateAttempts()
        
        if ( (guesses[0] in IMAGE_NAME_MAP) and \
             (IMAGE_NAME_MAP[guesses[0]] == guesses[1]) ):
            print "You made a match!"
            guesses = []
            
            if (False not in showCard):
                # All matched
                print "You win the game!"
                victorySound.play()
            else:
                # Match found
                matchSound.play()
        else:
            print "Try again"
            failSound.play()
        
    
def updateAttempts():
    label.set_text("Turns = " + str(matchAttempts))
        
                        
# cards are logically 150x200 pixels in size    
def draw(canvas):
    
    global imageMap
    global count
    global facedownImage
    
    for cardNumber in range(0,len(imageList)):
        cardX = cardNumber % 4
        cardY = cardNumber / 4
        
        cardX = (cardX + 1) * PADDING + CARD_WIDTH * (cardX + 0.5)
        cardY = (cardY + 1) * PADDING + CARD_HEIGHT * (cardY + 0.5)
        
        #print('Card %d is at (%d, %d) named %s' % \
        #      (cardNumber, cardX, cardY, imageList[cardNumber]))
        
        if showCard[cardNumber]:
            # Draw a face up card
            canvas.draw_image(imageMap[imageList[cardNumber]], 
                              (CARD_WIDTH / 2, CARD_HEIGHT / 2),
                              (CARD_WIDTH, CARD_HEIGHT),
                              (cardX, cardY),
                              (CARD_WIDTH, CARD_HEIGHT) )
        else:
            # Draw a face down card
            canvas.draw_image(facedownImage, 
                              (CARD_WIDTH / 2, CARD_HEIGHT / 2),
                              (CARD_WIDTH, CARD_HEIGHT),
                              (cardX, cardY),
                              (CARD_WIDTH, CARD_HEIGHT) )
        


# create frame and add a button and labels
frame = simplegui.create_frame("Memory", 725, 925)
frame.add_button("Reset", new_game)
label = frame.add_label("Turns = 0")

loadImages()
loadSounds()

# register event handlers
frame.set_mouseclick_handler(mouseclick)
frame.set_draw_handler(draw)

# get things rolling
new_game()
frame.start()


# Always remember to review the grading rubric
