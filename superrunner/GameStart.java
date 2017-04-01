
import javax.swing.*;
import java.awt.*;
public class GameStart{
    public static void main(String[] args){
        JFrame theGUI= new JFrame();
        theGUI.setTitle("Fabulous Runner");
         theGUI.setExtendedState(Frame.MAXIMIZED_BOTH);
         theGUI.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
         GameLogic panel = new GameLogic(Color.blue);
         Container pane = theGUI.getContentPane();
         pane.add(panel);
         // http://forums.devx.com/showthread.php?141527-adding-KeyListener-to-JPanel-!!!!    Wow Kristofer is cool
         panel.setFocusable(true);
         panel.requestFocusInWindow();
         panel.setBackground(Color.black);
         theGUI.setVisible(true);
        }
    }