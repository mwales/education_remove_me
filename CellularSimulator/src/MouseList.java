//MouseList.java
//Jeffrey Miller
//December 5, 2000
//Version 1.0

import javax.swing.event.*;
import java.awt.event.*;
//import ImageMap;
import java.io.*;

class MouseList extends MouseInputAdapter implements Serializable
{
    ImageMap Map;

    MouseList(ImageMap temp)
    {
      Map = temp;

    }

   //adds a single cell to the ImageMap
    public void mouseClicked(MouseEvent e)
    {
      Map.addCell(e.getX(),e.getY(),Map.StateNum);
    };
    public void mouseEntered(MouseEvent e)
    {};
    public void mouseExited(MouseEvent e)
    {};

    //adds multiple cells to the ImageMap
    public void mouseReleased(MouseEvent e)
    {
      Map.Dragger = 0;
      Map.addGroupCell(Map.MousePressX,Map.MousePressY,e.getX(),e.getY(),Map.StateNum); 
    };

    public void mousePressed(MouseEvent e)
    {
      Map.AnchorX = e.getX();
      Map.AnchorY = e.getY();
      Map.MousePressX = e.getX();
      Map.MousePressY = e.getY();
      
    };

    public void mouseMoved(MouseEvent e)
    {};

    public void mouseDragged(MouseEvent e)
    {
      Map.DrawToX = e.getX();
      Map.DrawToY = e.getY();
      Map.Dragger = 1;
      Map.repaint();
    }
}
