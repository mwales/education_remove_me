//HelpPane.java
//Jeffrey Miller
//December 5, 2000
//Version 1.0

import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
//import HelpDialog;
import java.io.*;

class HelpPane extends JPanel implements ActionListener
{
   HelpDialog ContainedIn;
   JTextArea TextArea1;
   String ReadInText;
   JScrollPane ScrPane;

   HelpPane(HelpDialog Contain)
   {
      setLayout(new BorderLayout());
      ContainedIn = Contain;
      setText();
      JButton Ok = new JButton("Done");
      Ok.setActionCommand("OK");
      Ok.addActionListener(this);
      add(ScrPane);
      add(Ok,BorderLayout.SOUTH);
   };

   public void actionPerformed(ActionEvent e)
   {
      String act = e.getActionCommand();
      if(act.equals("OK"))
      {
         ContainedIn.dispose();
      }
   };

   private void setText()
   {

      String temp;
      TextArea1 = new JTextArea();
      try
      {
         BufferedReader Reader = new BufferedReader(new FileReader("help.txt"));
         do
         {
           
            temp = Reader.readLine();
            if(temp != null)
            {
               ReadInText = temp;
               ReadInText += "\n";
               TextArea1.append(ReadInText);
            }
         }
         while(temp != null);
        
      }
      catch(Exception e)
      {
         System.out.println(""+e.getMessage());
      }
      TextArea1.setEditable(false);
      ScrPane = new JScrollPane(TextArea1);
      ScrPane.setPreferredSize(new Dimension(700,600));
   };   
}
