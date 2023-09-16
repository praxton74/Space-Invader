import turtle
import os
import tkinter
import math
import random
import time
import winsound
# set Screen
win = turtle.Screen()
win.bgcolor("blue")
win.title("Space Invaders")
win.setup(700, 700, 1, -1)
win.bgpic("background.gif")
# register shape
turtle.register_shape("player2.gif")
turtle.register_shape("invader3.gif")
turtle.register_shape("tenor2.gif")

# Draw border
border_pen = turtle.Turtle()

border_pen.speed(0)
border_pen.color("white")
border_pen.penup()
border_pen.setposition(-300, -300)
border_pen.pendown()
border_pen.pensize(3)

for side in range(4):
    border_pen.fd(600)
    border_pen.lt(90)
border_pen.hideturtle()


# set Score
Score = 0
# scrore
Score_pen = turtle.Turtle()
Score_pen.speed(0)
Score_pen.color("white")
Score_pen.penup()
Score_pen.setposition(-290, 280)
Scorestr = "Score : %s" % Score
Score_pen.write(Scorestr, False, align="left", font=("Arial", 14, "normal"))
Score_pen.hideturtle()

Number_of_life = 3
life = []

x = 220
y = 280
for i in range(Number_of_life):
    life.append(turtle.Turtle())
print(life)


for l in life:
    l.color("red")
    l.shape("circle")
    l.penup()
    l.speed(0)
    l.setposition(x, y)
    x = x + 30


# game over
# scrore
gameover_pen = turtle.Turtle()
gameover_pen.speed(0)
gameover_pen.color("white")
gameover_pen.penup()
gameover_pen.setposition(0, 0)
gameover_pen.hideturtle()

# Create Player

Player = turtle.Turtle()
Player.color("blue")
Player.shape("player2.gif")
Player.penup()
Player.speed(0)
Player.setposition(0, -250)
Player.setheading(90)


Playerspeed = 15


Enemyspeed = 12


# Choose the Number of Enmeys
Number_of_enemies = 10
Enemies = []

# ADD Enemies

for i in range(Number_of_enemies):
    Enemies.append(turtle.Turtle())

# create Enemy
for Enemy in Enemies:
    Enemy.color("red")
    Enemy.shape("invader3.gif")
    Enemy.penup()
    Enemy.speed(0)
    x = random.randint(-200, 200)
    y = random.randint(100, 250)
    Enemy.setposition(x, y)
# Create Bullet
Bullet = turtle.Turtle()
Bullet.color("white")
Bullet.shape("tenor2.gif")
Bullet.penup()
Bullet.speed(0)
Bullet.setheading(90)

Bullet.hideturtle()
Bullet.setposition(0, 400)
Bulletspeed = 20

# Define state
# reday -ready to fire
#fire -firing
Bulletstate = "ready"
# Move layer Left ANd right


def move_left():
    x = Player.xcor()
    x = x - Playerspeed
    if x < -280:
        x = -280
    Player.setx(x)


def move_right():
    x = Player.xcor()
    x = x + Playerspeed
    if x > 280:
        x = 280
    Player.setx(x)


def fire_bullet():
        # global state bcoz it will chang
    global Bulletstate
    # move bullet jst above player
    if Bulletstate == "ready":
        Bulletstate = "fire"

        winsound.PlaySound("laser.wav", winsound.SND_ASYNC)  # sound for windows

        x = Player.xcor()
        y = Player.ycor() + 10
        Bullet.setposition(x, y)
        Bullet.showturtle()

# collision


def isCollision(t1, t2):
    distance = math.sqrt(math.pow(t1.xcor() - t2.xcor(), 2) + math.pow(t1.ycor() - t2.ycor(), 2))
    if distance < 20:
        return True
    else:
        return False


collision = 0
# keyboard
turtle.listen()
turtle.onkey(move_left, "Left")
turtle.onkey(move_right, "Right")
turtle.onkey(fire_bullet, "space")
# Main game loop
winsound.PlaySound("space.wav", winsound.SND_ASYNC)
while True:

    for Enemy in Enemies:

        x = Enemy.xcor()
        x = x + Enemyspeed
        Enemy.setx(x)

        # move enemy back and down
        if x > 280:
            for E in Enemies:
                y = E.ycor()

                y = y - 40
                E.sety(y)
            Enemyspeed = Enemyspeed * -1
        if x < -280:
            for E in Enemies:
                y = E.ycor()

                y = y - 40
                E.sety(y)
            Enemyspeed = Enemyspeed * -1
        if Enemy.ycor() < -300:
            winsound.PlaySound("explosion-e+b.wav", winsound.SND_ASYNC)
            Number_of_life = Number_of_life - 1
            m = life.pop()
            print(m)
            m.hideturtle()
            for e in Enemies:
                e.showturtle()
                x = random.randint(-200, 200)
                y = random.randint(100, 250)
                e.setposition(x, y)
            print("touch to bottom")
        # chcek collision
        if isCollision(Bullet, Enemy):
            winsound.PlaySound("low.wav", winsound.SND_ASYNC)
            Bullet.hideturtle()
            Bulletstate = "ready"
            Bullet.setposition(0, - 400)
            x = random.randint(-200, 200)
            y = random.randint(100, 250)
            Enemy.setposition(x, y)
            Score = Score + 10
            Scorestr = "Score : %s" % Score
            Score_pen.clear()
            Score_pen.write(Scorestr, False, align="left", font=("Arial", 14, "normal"))
            # sound for windows

        if isCollision(Player, Enemy):
            winsound.PlaySound("explosion-e+b.wav", winsound.SND_ASYNC)
            Number_of_life = Number_of_life - 1

            m = life.pop()
            print(m)
            m.hideturtle()

            Player.hideturtle()
            time.sleep(1)
            Player.showturtle()
            Enemy.hideturtle()
            for e in Enemies:
                e.showturtle()
                x = random.randint(-200, 200)
                y = random.randint(100, 250)
                e.setposition(x, y)
            Bulletstate = "ready"
            if Number_of_life == 0:
                Bulletstate = "fire"
                gameover_pen.write("Game Over", False, align="center", font=("Arial", 50, "normal"))
                gameover_pen.hideturtle()
                collision = 1
                break
    if collision == 1:
        Player.hideturtle()
        Enemy.hideturtle()
        Bulletstate = "fire"
        gameover_pen.write("Game Over", False, align="center", font=("Arial", 50, "normal"))
        gameover_pen.hideturtle()

        break

    # move bullet
    # if Bulletstate == "fire":
    y = Bullet.ycor()
    y = y + Bulletspeed
    Bullet.sety(y)

    if Bullet.ycor() > 275:
        Bullet.hideturtle()
        Bulletstate = "ready"

tkinter.mainloop()
