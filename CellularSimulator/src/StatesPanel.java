//StatesPanel.java
//Jeffrey Miller
//November 22, 2000
//Version 1.0

import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
//import StateList;
//import pane;

class StatesPanel extends JPanel implements ActionListener
{

   StateList SL;
   JButton Play;
   JButton Stop;
   JButton ButtonArray[];
   int CurrentState=0;
   String Action;
   pane ParentPane;


   StatesPanel(StateList SList,pane tempobj)
   {
      super();
      ParentPane = tempobj;
      SL = SList;
      ButtonArray = new JButton[SL.getSize()];
      setLayout(new GridLayout(8,1,5,5));
      addButtons();
   };

   private void addButtons()
   {
      
      Play = new JButton("Play");
      Play.setActionCommand("Play");
      Play.addActionListener(this);
      add(Play);
      Stop = new JButton("Stop");
      Stop.setActionCommand("Stop");
      Stop.addActionListener(this);
      add(Stop);
      for(int x = 0; x < (SL.getSize());++x)
      {
         ButtonArray[x] = new JButton(SL.getImage(x));
         String ActionCom = "State" + x;
         ButtonArray[x].setActionCommand(ActionCom);
         ButtonArray[x].addActionListener(this);
         add(ButtonArray[x]);
      }
   };
    
   public void actionPerformed(ActionEvent e)
   {
      String event = e.getActionCommand();

      if(event.equals("Play"))
         ParentPane.run();
      if(event.equals("Stop"))
         ParentPane.stop();


      if(event.startsWith("State"))
      {
        CurrentState = ((event.charAt(event.length()-1))-48);
        setSelectedState();
      }
   };

   void setSelectedState()
   {
     ParentPane.setSelectedState(CurrentState);
   }

}
