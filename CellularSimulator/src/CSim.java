//CSim.java
//Jeffrey Miller
//October 29, 2000
//Version 1.0


import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
//import "listen";
//import "window";
//import "StateList";


public class CSim
{
   static window CellWindow;
   static StateList SL;

   public static void main(String[] args)
   {
      CellWindow=new window();
      listen Closer = new listen();
      CellWindow.addWindowListener(Closer);
      CellWindow.pack();
      CellWindow.setVisible(true);
   };
   public static void TransitionWindowFinished(StateList NewSL)
      {
      SL = NewSL;

      /*
      System.out.println("Transition window operation completed.  Final statelist:");
      for( int x=0; x < SL.getSize(); x++)
         {
         System.out.print( SL.getStateName(x) + ", " );
         }
      System.out.println("");
      */ 

      CellWindow.dispose();
      CellWindow.setNewStateList(SL);
      }              

}

   

