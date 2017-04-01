import java.awt.*;
import java.util.Random;

//Kristofer is awesome
public class Particle
{
    int red=200;
    int green=145;
    int blue=134;
    private Random random=new Random();
    
    Circle[] circleArray = new Circle[Random()+1];
    public int Random(){
        int randomAmount=(random.nextInt(7)+2);
        return randomAmount;
    }
    
    public Particle(){
        for (int i=0;i<circleArray.length;i++){
            int randomSize=(random.nextInt(5)+1);
            circleArray[i]= new Circle(0,0,randomSize, new Color(red,green,blue));
        }
    }
    
    public void render(Graphics g){
        for (int i=0;i<circleArray.length;i++){
            circleArray[i].draw(g);
        }
    }
   
    public void ParticleStartPoint(int x, int y){
         for (int i=0;i<circleArray.length;i++){
                circleArray[i].setX(x);
                circleArray[i].setY(y);
            }
    }
    
    public void ParticleRandomSpeed(){
          for (int i=0;i<circleArray.length;i++){
              int randomXSpeed=(random.nextInt(12)-6);
              int randomYSpeed=(random.nextInt(12)-6);
              circleArray[i].setxVel(randomXSpeed);
              circleArray[i].setyVel(randomYSpeed);
          }
    }
    
    public void speedSlowDown(){
          for (int i=0; i<circleArray.length;i++){
          circleArray[i].setxVel(circleArray[i].getxVel()/1.025);
          circleArray[i].setyVel(circleArray[i].getyVel()/1.025);
        }
    }
    
    public void radiusGrow(){
         for (int i=0; i<circleArray.length-3;i++){
             circleArray[i].setRadius(circleArray[i].getRadius()+1);
            }
    }
    
    public void colorFade(){
        for (int i=0; i<circleArray.length;i++){
            int randomFade=(random.nextInt(2)+1);
            red=red-randomFade;
            green=green-randomFade;
            blue=blue-randomFade;
            if (blue > 0){
                circleArray[i].setCcolor(new Color(red,green,blue));
            }else{
                circleArray[i].setCcolor(Color.black);
            }
        }
    }
    
    public void particleInitialize(int x, int y){
        Random();
        ParticleStartPoint(x,y);
        ParticleRandomSpeed();
    }
    
    public boolean deleteParticle(){
        int amountout=0;
        boolean delete=false;
        for (int i=0;i<circleArray.length;i++){
            if(Color.black==circleArray[i].getColor()){
                amountout++;
            }
        }
        if(amountout==circleArray.length){
            delete=true;
        }
        return delete;
    }

    //put in all things that need to keep happening after initilization sup old comment
    public void particleLogic(){
         speedSlowDown();
         radiusGrow();
         colorFade();
         for (int i=0;i<circleArray.length;i++){
             if(circleArray[i]!=null)
                circleArray[i].move();
        }
    }
}
