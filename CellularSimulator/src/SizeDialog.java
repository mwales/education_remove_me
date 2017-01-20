//SizeDialog.java
//Jeffrey Miller
//November 22, 2000
//Version 1.0

import java.awt.*;
import javax.swing.*;
//import DialogPane;


class SizeDialog extends JDialog
{
   DialogPane Content;
   String Text1;
   String Text2;

   SizeDialog(Frame parent, String title, boolean modal)
   {
      super(parent, title, modal);
      dialogInit();
      Content= new DialogPane(this);
      setContentPane(Content);
           
   }
   public void remove()
   {
       Text1 = Content.getText1();
       Text2 = Content.getText2();
       setVisible(false);
   };

   public String getText1()
   {
      return(Text1);
   };

   public String getText2()
   {
      return(Text2);
   };
};
