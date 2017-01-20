//Scroller.java
//Jeffrey Miller
//Version 1.0
//November 21, 2000

import javax.swing.*;
import java.awt.event.*;
import java.awt.*;



//constructor takes the size of the desired scroll pane

class Scroller extends JScrollPane
{

   Scroller(int x, int y, JLabel Map)
   {
      super(Map);
      if(x > 900)
        x = 900;
      if(y > 700)
        y = 700;
      setPreferredSize(new Dimension(x,y)); 
   };
}

      
