public class Asteroid
{
    public Circle circle;
    
    public Asteroid(Circle circle)
    {
        this.circle = circle;
    }
    
    public void move()
    {
        circle.move();
    }
    
    public void render(java.awt.Graphics g)
    {
        circle.draw(g);
    }
}