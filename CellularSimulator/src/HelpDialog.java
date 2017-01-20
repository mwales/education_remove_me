//HelpDialog.java
//Jeffrey Miller
//December 5, 2000
//Version 1.0

import java.awt.*;
import javax.swing.*;
//import HelpPane;


class HelpDialog extends JDialog
{
   HelpPane Content;
      
   HelpDialog(Frame parent, String title, boolean modal)
   {
      super(parent, title, modal);
      dialogInit();
      Content= new HelpPane(this);
      setContentPane(Content);
           
   }
}

