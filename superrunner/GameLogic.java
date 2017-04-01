import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
public class GameLogic extends JPanel{
    //Timer
    private javax.swing.Timer timer;
    //Key booleans
    private boolean keyUp=false;
    private boolean keyDown=false;
    private boolean keyLeft=false;
    private boolean keyRight=false;
    private int mouseX;
    private int mouseY;
    private boolean mouseClicked = false;
    
    private int countTimer=0; //Going to have numbers! yay
    
    private Runner runner;
    
    public GameLogic(Color backColor){
        runner= new Runner();
        runner.soundEffect();
        setBackground(backColor);
        setPreferredSize(new Dimension(getWidth(), getHeight()));
        
        timer = new javax.swing.Timer(1000/60, new MoveListener());
        timer.start();
        addKeyListener(new BounceKeyListener());
        addMouseListener(new BounceMouseListener());
    }
    
    public void paintComponent(Graphics g){
         super.paintComponent(g);
         runner.draw(g);
    }
    
    private class BounceKeyListener implements KeyListener{
        public void keyPressed (KeyEvent e) {
            int c = e.getKeyCode ();
            if(c==KeyEvent.VK_W)
                keyUp=true;
            if(c==KeyEvent.VK_S)
                keyDown=true;
            if(c==KeyEvent.VK_A)
                keyLeft=true;
            if(c==KeyEvent.VK_D)
                keyRight=true;
        }
        public void keyReleased (KeyEvent e) {
            int c = e.getKeyCode ();
            if (c==KeyEvent.VK_W)
                keyUp=false;
            if (c==KeyEvent.VK_S)
                keyDown=false;
            if (c==KeyEvent.VK_A)
                keyLeft=false;
            if (c==KeyEvent.VK_D)
                keyRight=false;
            }
        public void keyTyped(KeyEvent e){}
    }
    private class BounceMouseListener implements MouseListener{
            public void mousePressed(MouseEvent e) {
               mouseX = e.getX();
               mouseY = e.getY();
               mouseClicked = true;
            }
        
            public void mouseReleased(MouseEvent e) {}
        
            public void mouseEntered(MouseEvent e) {}
        
            public void mouseExited(MouseEvent e) {}
        
            public void mouseClicked(MouseEvent e) {}
    }
    private class MoveListener implements ActionListener{
        public void actionPerformed(ActionEvent e){
             countTimer++;
             if (countTimer == 5){
                 runner.music();
             }
             Wall wall=new Wall(getWidth(),getHeight());
             runner.keyboardInput(keyUp,keyDown,keyLeft,keyRight,wall);
             if (mouseClicked)
                runner.mouseInput(mouseX, mouseY);
             mouseClicked = false;
             runner.runnerLogic(wall);
             repaint();
          }
    }
}
            