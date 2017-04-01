import java.awt.*;
public class Bullet
{
    public Circle circle;
    
    public Bullet(double xVel, double yVel, double x, double y, Color color)
    {
        circle = new Circle((int)x,(int)y,10, color);
        circle.setxVel(xVel);
        circle.setyVel(yVel);
    }
    public void render(Graphics g){
        circle.draw(g);
        circle.move();
    }
}
