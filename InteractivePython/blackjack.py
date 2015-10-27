# Mini-project #6 - Blackjack

import simplegui
import random

# load card sprite - 936x384 - source: jfitz.com
CARD_SIZE = (72, 96)
CARD_CENTER = (36, 48)
card_images = simplegui.load_image("http://storage.googleapis.com/codeskulptor-assets/cards_jfitz.png")

CARD_BACK_SIZE = (72, 96)
CARD_BACK_CENTER = (36, 48)
card_back = simplegui.load_image("http://storage.googleapis.com/codeskulptor-assets/card_jfitz_back.png")    

CANVAS_WIDTH = 600
CANVAS_HEIGHT = 600

MOVEMENT_STEPS = 50.0

# initialize some useful global variables
in_play = False
score = 0

# define globals for cards
SUITS = ('C', 'S', 'H', 'D')
RANKS = ('A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K')
VALUES = {'A':1, '2':2, '3':3, '4':4, '5':5, '6':6, '7':7, '8':8, '9':9, 'T':10, 'J':10, 'Q':10, 'K':10}

# define card class
class Card:
    def __init__(self, suit, rank):
        if (suit in SUITS) and (rank in RANKS):
            self.suit = suit
            self.rank = rank
        else:
            self.suit = None
            self.rank = None
            print "Invalid card: ", suit, rank
        self.pos = [CANVAS_WIDTH / 2, 400]
        self.face_up = False
        self.dest = []
        self.moving = False
        self.movingStep = []
        self.stepNumber = -1
        self.sploding = False
        

    def __str__(self):
        return self.suit + self.rank #+ "@(" + str(self.pos) + ")"

    def get_suit(self):
        return self.suit

    def get_rank(self):
        return self.rank
    
    def animation_event(self):
        if self.moving:
            # Card is animating, update it's position
            self.pos = [self.pos[0] + self.movingStep[0],
                        self.pos[1] + self.movingStep[1]]
            #print str(self)
            if not self.sploding:
                # Cards are just moving to defined locations
                self.stepNumber -= 1
                if self.stepNumber == 0:
                    self.moving = False
                    self.pos = self.dest # Eliminate rounding errors and what not
                    #print "done moving" + str(self)
            else:
                # Cards sploding' just bounce around
                if self.pos[0] < CARD_SIZE[0] / 2:
                    self.pos[0] = CARD_SIZE[0] / 2
                    self.movingStep[0] *= -1
                if self.pos[1] < CARD_SIZE[1] / 2:
                    self.pos[1] = CARD_SIZE[1] / 2
                    self.movingStep[1] *= -1
                if self.pos[0] > (CANVAS_WIDTH - CARD_SIZE[0] / 2):
                    self.pos[0] = CANVAS_WIDTH - CARD_SIZE[0] / 2
                    self.movingStep[0] *= -1
                if self.pos[1] > (CANVAS_HEIGHT - CARD_SIZE[1] / 2):
                    self.pos[1] = CANVAS_HEIGHT - CARD_SIZE[1] / 2
                    self.movingStep[1] *= -1
               
        

    def draw(self, canvas):
        if self.face_up:
            card_loc = (CARD_CENTER[0] + CARD_SIZE[0] * RANKS.index(self.rank), 
                       CARD_CENTER[1] + CARD_SIZE[1] * SUITS.index(self.suit))
            canvas.draw_image(card_images, 
                              card_loc, 
                              CARD_SIZE, 
                              [int(self.pos[0]),  int(self.pos[1])], 
                              CARD_SIZE)
        else:
            canvas.draw_image(card_back, 
                              CARD_BACK_CENTER, 
                              CARD_BACK_SIZE, 
                              [int(self.pos[0]),  int(self.pos[1])], 
                              CARD_SIZE)
            
    def turn_over(self):
        self.face_up = True
        
    def move_card(self, pos):
        self.moving = True
        self.sploding = False
        self.dest = [float(pos[0]), float(pos[1])]
        self.pos = [float(self.pos[0]), float(self.pos[1])]
        self.movingStep = [(self.dest[0] - self.pos[0]) / MOVEMENT_STEPS,
                           (self.dest[1] - self.pos[1]) / MOVEMENT_STEPS]
        self.stepNumber = MOVEMENT_STEPS
        
    def splode(self):
        randomX = random.choice(range(-CARD_SIZE[0], CANVAS_WIDTH + CARD_SIZE[0]))
        randomY = random.choice(range(-CARD_SIZE[1], CANVAS_HEIGHT + CARD_SIZE[1]))
        #whichSide = random.choice(range(0,4))
        #if (whichSide == 0):
        #    self.move_card([-CARD_SIZE[0], randomY])
        #elif (whichSide == 1):
        #    self.move_card([CANVAS_WIDTH + CARD_SIZE[0], randomY])
        #elif (whichSide == 2):
        #    self.move_card([randomX, -CARD_SIZE[1]])
        #else:
        #    self.move_card([randomX, CANVAS_HEIGHT+CARD_SIZE[1]])
        self.move_card([randomX, randomY])
        self.sploding = True
        
# define hand class
class Hand:
    def __init__(self, pos):
        self.cards = []
        self.pos = pos
        
    def __str__(self):
        # return a string representation of a hand
        cardStrings = []
        for singleCard in self.cards:
            cardStrings.append(str(singleCard))
        return "Hand: " + ",".join(cardStrings)    

    def add_card(self, card, face_up):
        self.cards.append(card)	# add a card object to a hand
        if face_up:
            card.turn_over()
        #card.move_card([self.pos[0] + ( CARD_SIZE[0] + 10) * (len(self.cards) - 1), 
        #                self.pos[1] ])
        card.move_card([self.pos[0] + 20 * (len(self.cards) - 1), self.pos[1] ])

        
    def get_value(self):
        # count aces as 1, if the hand has an ace, then add 10 to hand value if it doesn't bust
        # compute the value of the hand, see Blackjack video
        value = 0
        hasAce = False
        for singleCard in self.cards:
            if singleCard.get_rank() == 'A':
                hasAce = True
            value += VALUES[singleCard.get_rank()]
            
        if (hasAce and value <= 11):
            value += 10
        return value
            
   
    def draw(self, canvas):
        for singleCard in self.cards:
            singleCard.draw(canvas)
            
    def animation_event(self):
        for singleCard in self.cards:
            singleCard.animation_event()    
            
    def show_em(self):
        for singleCard in self.cards:
            singleCard.turn_over()
    
    def splode(self):
        for singleCard in self.cards:
            singleCard.splode()
            
# define deck class 
class Deck:
    def __init__(self):
        self.cards = []
        
        for aSuit in SUITS:
            for aRank in RANKS:
                self.cards.append(Card(aSuit,aRank))

    def shuffle(self):
        # shuffle the deck 
        random.shuffle(self.cards)

    def deal_card(self):
        return self.cards.pop()
    
    def __str__(self):
        cardStrings = []
        for singleCard in self.cards:
            cardStrings.append(str(singleCard))
        #return "Deck: " + ",".join(cardStrings)
    
    def draw(self, canvas):
        for singleCard in self.cards:
            singleCard.draw(canvas)

    def animation_event(self):
        for singleCard in self.cards:
            singleCard.animation_event()
            
    def splode(self):
        for singleCard in self.cards:
            if not in_play:
                singleCard.turn_over()
            singleCard.splode()
        

#define event handlers for buttons
def deal():
    global outcome, score, in_play, playerHand, dealerHand, dealerDeck, score

    if in_play:
        print "Whoops, I guess you forfeit..."
        score -= 1

    dealerDeck = Deck()
    dealerDeck.shuffle()
    
    playerHand = Hand([175, 500])
    dealerHand = Hand([175, 300])
    
    #print ("Shuffled deck-" + str(dealerDeck))
    playerHand.add_card(dealerDeck.deal_card(), True)
    dealerHand.add_card(dealerDeck.deal_card(), False)
    playerHand.add_card(dealerDeck.deal_card(), True)
    dealerHand.add_card(dealerDeck.deal_card(), True)
    #print ("Post deal deck-" + str(dealerDeck))
    
    #print ("player-" + str(playerHand) + \
    #       "    Val=" + str(playerHand.get_value()))
    #print ("dealer-" + str(dealerHand))
    
    in_play = True
    
    if playerHand.get_value() == 21:
        print("Winnner winnner, chicken dinner")
        in_play = False
        score += 1

def hit():
    global playerHand, dealerHand, dealerDeck, in_play, score
    if in_play:
        playerHand.add_card(dealerDeck.deal_card(), True)
        val = playerHand.get_value()
        #print ("player-" + str(playerHand) + \
        #   "    Val=" + str(playerHand.get_value()))
        if (val > 21):
            print "You busted"
            dealerHand.show_em()
            in_play = False
            score -= 1
        if (val == 21):
            print "You win!"
            dealerHand.show_em()
            in_play = False
            score += 1
 
    # if the hand is in play, hit the player
   
    # if busted, assign a message to outcome, update in_play and score
       
def stand():
    global in_play, dealerDeck, dealerHand, score
    
    dealerHand.show_em()
    
    if in_play:
        print "Dealer has " + str(dealerHand.get_value()) \
              + " points"
        
        while (dealerHand.get_value() < 17):
            nextCard = dealerDeck.deal_card()
            print "Dealer picks up " + str(nextCard)
            dealerHand.add_card(nextCard, True)
            print "Dealer has " + str(dealerHand.get_value()) \
                  + " points"
        
        in_play = False
        
        if dealerHand.get_value() > 21:
            print "Dealer busted"
            score += 1
        elif dealerHand.get_value() > playerHand.get_value():
            print "Dealer wins"
            score -= 1
        elif dealerHand.get_value() < playerHand.get_value():
            print "Player wins"
            score += 1
        else:
            print "Tie game, you still lose"
            score -= 1
def splode():
    global dealerDeck
    print "Sploding"
    dealerDeck.splode()
    
    if not in_play:
        playerHand.splode()
        dealerHand.splode()

    
    
# draw handler    
def draw(canvas):
    # test to make sure that card.draw works, replace with your code below
    
    global dealerDeck, playerHand, dealerHand, in_play
    
    dealerDeck.draw(canvas)
    playerHand.draw(canvas)
    dealerHand.draw(canvas)
    
    canvas.draw_text("Black", (60,110), 75, 'Black')
    canvas.draw_text("Jack", (160, 170), 75, 'Red')
    
    canvas.draw_text("Dealer:", (40, 300), 25, 'Black')
    canvas.draw_text("Player:", (40, 500), 25, 'Black')
    canvas.draw_text("Score: " + str(score), (400, 100), 50, 'Black')
    
    points = playerHand.get_value()
    if (points < 15):
        color = 'Black'
    elif (points < 21):
        color = 'Yellow'
    elif (points == 21):
        color = 'Blue'
    else:
        color = 'Red'
    
    canvas.draw_text(str(playerHand.get_value()), (500, 500), 40, color)
    
    if not in_play:
        canvas.draw_text(str(dealerHand.get_value()), (500, 300), 40, 'Black')
        
        if (playerHand.get_value() > 21):
            outcome = "Busted"
            outcomeColor = 'Red'
        elif (playerHand.get_value() == 21):
            outcome = "BlackJack"
            outcomeColor = 'Blue'
        elif ( (playerHand.get_value() > dealerHand.get_value()) or 
               (dealerHand.get_value() > 21) ):
            outcome = "You Win"
            outcomeColor = 'Black'
        else:
            outcome = "Loser"
            outcomeColor = 'Red'
    
        canvas.draw_text(outcome, (375, 415), 50, outcomeColor)

def animation_event():
    global dealerDeck, playerHand, dealerHand
    
    dealerDeck.animation_event()
    playerHand.animation_event()
    dealerHand.animation_event()


    
    
# initialization frame
frame = simplegui.create_frame("Blackjack", CANVAS_WIDTH, CANVAS_HEIGHT)
frame.set_canvas_background("Green")

#create buttons and canvas callback
frame.add_button("Deal", deal, 200)
frame.add_button("Hit",  hit, 200)
frame.add_button("Stand", stand, 200)
frame.add_button("Splode", splode, 200)
frame.set_draw_handler(draw)

timer = simplegui.create_timer(1000/50, animation_event)


# get things rolling
deal()
timer.start()
frame.start()


# remember to review the gradic rubric
