import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class Runner
{
    public Circle circle,leg1,leg2,head,hand1,hand2;
    public int jumpCounter = 70;
    public boolean touching = false;
    private boolean headRightX;
    private boolean headUpY;
    private int walkCounter;
    private double newAngle = -120;
    private int legHeight = 88;
    private boolean touchLeft;
    private boolean touchRight;
    private int asteroidLimit = 10;
    private Asteroid[] asteroids = new Asteroid[asteroidLimit];
    private Particle[] particles = new Particle[asteroidLimit];
    int bulletLimit = 6;
    Bullet[] bullets = new Bullet[bulletLimit];
    
    public void soundEffect() {
        SoundEffect.init();
        SoundEffect.volume = SoundEffect.Volume.MEDIUM;
    }
    
    public void music() {
        SoundEffect.GLITCH.play();
    }
    
    public Runner(){
        circle= new Circle(100,100,35,Color.blue);
        leg1=new Circle(100,100,17,Color.red);
        leg2= new Circle(100,100,17,Color.red);
        head= new Circle(100,100,20,Color.green);
        hand1=new Circle(100,100,15,Color.red);
        hand2=new Circle(100,100,15,Color.red);
    }
    
    public void head(){
        double circleX = circle.getX();
        double circleY = circle.getY();
        double headX = circleX;
        double headY = circleY - 60;
        head.setX((int)headX);
        head.setY((int)headY);
    }
    
    public void hands(){
        double circleX = circle.getX();
        double circleY = circle.getY();
        double  handX = circleX + 30;
        double handY = circleY - 10;
        double handX2 = circleX - 30;
        double handY2 = circleY - 10;
        hand1.setX(handX);
        hand1.setY(handY);
        hand2.setX(handX2);
        hand2.setY(handY2);
    }
    
    public void legs(){
        double circleX = circle.getX();
        double circleY = circle.getY();
        double legX = circleX + 40;
        double legY = circleY + 80;
        double legX2 = circleX - 40;
        double legY2 = circleY + 80;
        leg1.setX(legX);
        leg1.setY(legY);
        leg2.setX(legX2);
        leg2.setY(legY2);
    }
    
    public void legs2(){
        double circleX = circle.getX();
        double circleY = circle.getY();
        double legX = leg1.getX();
        double legY = circleY + 80;
        double legX2 = leg2.getX();
        double legY2 = circleY + 80;
        if (legX  -  circleX <  - 60)
            legX = circleX  +  59;
        else if (legX  -  circleX  >  60)
            legX = circleX  +   - 59;
        leg1.setX(legX);
        leg1.setY(legY);
        leg2.setX((legX  -  circleX  +  90) % 120  +  circleX  -  60);
        leg2.setY(legY2);
    }
     
    public void keyboardInput(boolean up, boolean down, boolean left, boolean right,Wall wall){
        double ySpeed=circle.getyVel();
        double xSpeed=circle.getxVel();
        boolean jump = false;
        boolean duck = false;
        legHeight = 88;
        int wallxJump = 20;
        int wallyJump = 13;
        if(up && !down) {
            jump = true;
        }else{jump = false;}
        if(down && !up) {
            duck = true;
            legHeight = 35;
            ySpeed += .5;
        }else{duck = false;}
        
        if(right && !left && xSpeed<7) {
            xSpeed += .4;
            if(touchLeft){
                xSpeed += wallxJump;
                if(ySpeed > - wallyJump){
                    ySpeed -= wallyJump;
                }
            }
        }
        if(left && !right && xSpeed >  - 7) {
            xSpeed -= .4;
            if(touchRight){
                xSpeed -= wallxJump;
                if(ySpeed > - wallyJump){
                    ySpeed -= wallyJump;
                }
            }
        }
         if(!left && !right) {
            if(xSpeed<.02 && xSpeed >  - .02){
                xSpeed=0;}else{xSpeed /= 1.025;
           }
        }
        circle.setxVel(xSpeed);
        circle.setyVel(ySpeed);
        jump(jump,wall);
        duck(duck);
    }
    
    /**
     * \brief
     *   Call when the mouse is clicked with the coords
     */
    public void mouseInput(int x, int y){
        for (int i = 0; i < bullets.length; i++){
            if (bullets[i] != null)
                continue;
            Circle hand = x < circle.getX() ? hand2 : hand1;
            double xVelocityDirection = x - hand.getX();
            double yVelocityDirection = y - hand.getY();
            double velocityRatio = 14/Math.sqrt(xVelocityDirection*xVelocityDirection + yVelocityDirection*yVelocityDirection);
            bullets[i] = new Bullet(
                velocityRatio * xVelocityDirection,
                velocityRatio * yVelocityDirection,
                hand.getX(),
                hand.getY(),
                Color.orange);
            break;
        }
    }
    
    public void jump(boolean jump,Wall wall){
        if(jump && touching){
            circle.setY((int)(circle.getY() - 1.5));
            jumpCounter = 0;
        }
        if(jumpCounter < 10 && jump){
            jumpCounter++;
            circle.setyVel(circle.getyVel() - 1);
        }else{jumpCounter = 70;}
    }
    
    public void duck(boolean duck){}
      public void circleCollision(Wall wall) {
        double ySpeed = circle.getyVel();
        double xSpeed = circle.getxVel();
        
        touchRight = circle.collidesRight(wall);
        touchLeft = circle.collidesLeft(wall);
        boolean top = circle.collidesTop(wall);
        boolean bottom = circle.collidesBottom(wall);
        touching = false;
        if(touchRight){
           circle.setX(wall.width - circle.getRadius() - 1);
           touching = true;
           touchRight = true;
        }
        if(touchLeft){
           circle.setX(circle.getRadius() + 1);
           touching = true;
           touchLeft = true;
        }
        if(top){
           ySpeed=1;
           circle.setY(circle.getRadius());
        }
        if(bottom){
           ySpeed=0;
           circle.setY(wall.height - circle.getRadius());
           touching = true;
        }
        circle.setyVel(ySpeed);
        circle.setxVel(xSpeed);
    }
    
    public void legCollision(Wall wall){
        double ySpeed = leg1.getyVel();
        double xSpeed = leg1.getxVel();
        
        boolean right = leg1.collidesRight(wall);
        boolean left = leg1.collidesLeft(wall);
        boolean top = leg1.collidesTop(wall);
        boolean bottom = leg1.collidesBottom(wall);
        
        double circleY = circle.getY();
        double legY = leg1.getY();
        double ratio = (legY - (circleY + 98))/60;
        if(right){
           leg1.setX(wall.width - leg1.getRadius() - 1);
           circle.setxVel(circle.getxVel() - .15);
        }
        if(left){
           leg1.setX(leg1.getRadius() + 1);
           circle.setxVel(circle.getxVel() + .15);
        }
        if(top){
           ySpeed = 1;
           leg1.setY(leg1.getRadius());
        }
        if(bottom){
           ySpeed = 0;
           leg1.setY(wall.height - leg1.getRadius());
           touching = true;
           if(circleY + legHeight > legY){
               circle.setyVel(circle.getyVel() + ratio);
           }
           if(circle.getyVel() > 3){
               circle.setyVel(circle.getyVel() / 1.25);
            }
        }
        leg1.setyVel(ySpeed);
        leg1.setxVel(xSpeed);
    }
    
    public void leg2Collision(Wall wall){
        double ySpeed = leg2.getyVel();
        double xSpeed = leg2.getxVel();
        
        boolean right = leg2.collidesRight(wall);
        boolean left = leg2.collidesLeft(wall);
        boolean top = leg2.collidesTop(wall);
        boolean bottom = leg2.collidesBottom(wall);
        
        double circleY = circle.getY();
        double legY2 = leg2.getY();
        double ratio = (legY2 - (circleY + 98))/60;
        if(right){
           leg2.setX(wall.width - leg2.getRadius() - 1);
           circle.setxVel(circle.getxVel() - .15);
        }
        if(left){
           leg2.setX(leg2.getRadius() + 1);
           circle.setxVel(circle.getxVel() + .15);
        }
        if(top){
           ySpeed = 1;
           leg2.setY(leg2.getRadius());
        }
        if(bottom){
           ySpeed = 0;
           leg2.setY(wall.height - leg2.getRadius());
           touching = true;
           if(circleY + legHeight > legY2){
               circle.setyVel(circle.getyVel() + ratio);
           }
           if(circle.getyVel() > 3){
               circle.setyVel(circle.getyVel()/1.25);
           }
        }
        leg2.setyVel(ySpeed);
        leg2.setxVel(xSpeed);
    }
    
    public void hand1Collision(Wall wall){
        double ySpeed = hand1.getyVel();
        double xSpeed = hand1.getxVel();
        
        boolean right = hand1.collidesRight(wall);
        boolean left = hand1.collidesLeft(wall);
        boolean top = hand1.collidesTop(wall);
        boolean bottom = hand1.collidesBottom(wall);
        if(right){
           hand1.setX(wall.width - hand1.getRadius() - 1);
           circle.setxVel(circle.getxVel() - .15);
        }
        if(left){
           hand1.setX(hand1.getRadius() + 1);
           circle.setxVel(circle.getxVel() + .15);
        }
        if(top){
           ySpeed = 1;
           hand1.setY(hand1.getRadius());
        }
        if(bottom){
           ySpeed = 0;
           hand1.setY(wall.height - hand1.getRadius());
        }
        
        hand1.setyVel(ySpeed);
        hand1.setxVel(xSpeed);
    }
    
    public void hand2Collision(Wall wall){
        double ySpeed = hand2.getyVel();
        double xSpeed = hand2.getxVel();
        
        boolean right = hand2.collidesRight(wall);
        boolean left = hand2.collidesLeft(wall);
        boolean top = hand2.collidesTop(wall);
        boolean bottom = hand2.collidesBottom(wall);
        if(right){
           hand2.setX(wall.width - hand2.getRadius() - 1);
        }
        if(left){
           hand2.setX(hand2.getRadius() + 1);
        }
        if(top){
           ySpeed = 1;
           hand2.setY(hand2.getRadius());
        }
        if(bottom){
           ySpeed = 0;
           hand2.setY(wall.height - hand2.getRadius());
        }
        
        hand2.setyVel(ySpeed);
        hand2.setxVel(xSpeed);
    }
    
    private void bulletsCollision(Wall wall)
    {
        for (int i = 0; i < bullets.length; i++)
        {
            Bullet b = bullets[i];
            if (b == null)
                continue;
            if (b.circle.collidesWith(wall) != ' ')
            {
                /* kaboom */
                bullets[i] = null;
                continue;
            }
            for (int j = 0; j < asteroids.length; j++)
            {
                Asteroid a = asteroids[j];
                if (a == null)
                    continue;
                if (b.circle.collidesWith(a.circle))
                {
                    int radius = a.circle.getRadius();
                    if (radius < 25){
                        /* kaploughie! */
                        SoundEffect.EXPLOSION.play();
                        for (int v = 0; v < particles.length; v++){
                            if (particles[v] == null){
                                particles[v] = new Particle();
                                particles[v].particleInitialize((int)a.circle.getX(), (int)a.circle.getY());
                                continue;
                            }
                        }
                        asteroids[j] = null;
                    }else{
                                a.circle.setRadius(radius - 10);
                            }
                    bullets[i] = null;
                }
            }
        }
    }
    
    private void asteroidsCollision(Wall wall)
    {
        for (int i = 0; i < asteroids.length; i++)
        {
            Asteroid a = asteroids[i];
            if (a == null)
                continue;
            if (a.circle.collidesWith(wall) != ' ')
                asteroids[i] = null;
        }
    }

    public void gravity(){
        circle.setyVel(circle.getyVel() + .5);
    }
    
    private void rainAsteroids(Wall wall)
    {
        for (int i = 0; i < asteroids.length; i++)
        {
            if (asteroids[i] != null)
                continue;
            asteroids[i] = new Asteroid(
                new Circle(
                    Math.random() * wall.width,
                    Math.random() * wall.height * 0.1,
                    (int)(Math.random() * 40 + 10),
                    Color.gray));
            asteroids[i].circle.setyVel(2 + Math.random()*2);
            asteroids[i].circle.setxVel((wall.width * Math.random() - asteroids[i].circle.getX()) / wall.height);
        }
    }

    public void draw(Graphics g){
        hand2.draw(g);
        leg2.draw(g);
        circle.draw(g);
        leg1.draw(g);
        head.draw(g);
        hand1.draw(g);
        for (Bullet b : bullets) {
            if (b == null)
                continue;
            b.render(g);
        }
        for (Asteroid a : asteroids)
        {
            if (a == null)
                continue;
            a.render(g);
        }
        for (Particle p : particles)
        {
            if (p == null)
                continue;
            p.render(g);
        }
    }
    
    public void runnerLogic(Wall wall){
        head();
        hands();
        legs();
        gravity();
        rainAsteroids(wall);
        circleCollision(wall);
        hand1Collision(wall);
        hand2Collision(wall);
        legCollision(wall);
        leg2Collision(wall);
        bulletsCollision(wall);
        asteroidsCollision(wall);
        circle.move();
        for (Bullet b : bullets) {
            if (b == null)
                continue;
            b.circle.move();
        }
        for (Asteroid a : asteroids)
        {
            if (a == null)
                continue;
            a.move();
        }
        for (int d = 0; d < particles.length; d++)
        {
            if (particles[d] == null)
                continue;
            particles[d].particleLogic();
            if (particles[d].deleteParticle())
                particles[d] = null;
        } 
    }
}
