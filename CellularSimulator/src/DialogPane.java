//DialogPane.java
//Jeffrey Miller
//November 22, 2000
//Version 1.0


import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
//import SizeDialog;
//import SizeHolder;


class DialogPane extends JPanel implements ActionListener
{

   JLabel EntryText1;
   JLabel EntryText2;
   JTextField Text1;
   JTextField Text2;
   JButton OkButton;
   SizeDialog ContainedIn;
   String TextIn1;
   String TextIn2;
   SizeHolder XYSize;
   Integer IntX;
   Integer IntY;

   DialogPane(SizeDialog C)
   {
      super();
      ContainedIn = C;
      setLayout(new GridLayout(3,2,5,5));
      addLabel1();
      addTextField1();
      addLabel2();
      addTextField2();
      addButton();
      
   };

   private void addLabel1()
   {
      EntryText1 = new JLabel("Enter the number of horizontal cells");      
      add(EntryText1);
   };

   private void addLabel2()
   {
      EntryText2 = new JLabel("Enter the number of vertical cells");
      add(EntryText2);
   };

   private void addTextField1()
   {
      Text1 = new JTextField(5);
      Text1.setActionCommand("Text1");
      Text1.addActionListener(this);
      add(Text1);
   }

   private void addTextField2()
   {
      Text2 = new JTextField(5);
      Text2.setActionCommand("Text2");
      Text2.addActionListener(this);
      add(Text2);
   }

   private void addButton()
   {
      OkButton = new JButton("OK");
      OkButton.setActionCommand("OK");
      OkButton.addActionListener(this);
      add(OkButton);
   }

   //handles the events
   public void actionPerformed(ActionEvent evt)
   {
      String action = evt.getActionCommand();

      if(action.equals("OK"))
      {
         try
         {
         TextIn1 = Text1.getText();
         TextIn2 = Text2.getText();
         int x = IntX.parseInt(TextIn1);
         int y = IntY.parseInt(TextIn2);
         if(x > 0 && x <= 45 && y > 0 &&  y <= 35) 
            ContainedIn.remove();
         else
            JOptionPane.showMessageDialog(this,"You must enter an integer between 0 and 35 on the Y Dim, and a number between 0 and 45 on the X Dim."
         ,     "Inane Error", JOptionPane.ERROR_MESSAGE);
            
         }
      catch(Exception e)
      {
            JOptionPane.showMessageDialog(this,"You must enter an integer between 0 and 35 on the Y Dim, and a number between 0 and 45 on the X Dim."
         ,     "Inane Error", JOptionPane.ERROR_MESSAGE);
      }
      }
   }
   public String getText1()
   {
      return(TextIn1);
   };

   public String getText2()
   {
      return(TextIn2);
   };
}

