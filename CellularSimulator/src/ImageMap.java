//ImageMap.java
//Jeffrey Miller
//November 22, 2000
//Version 1.0


/* This class is the map of all the cells to be simulated.
   It implements Serializable to enable the saving of itself.
   This class also handles all of the running of the simulation
   through a thread.  In the constructor the StateList is passed in
   to enable all of the calculations of the next states to be made.
   It implements Runnable to make the running of the thread possible
*/
   
import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import javax.swing.event.*;
//import StateList;
//import ICell;
import java.io.*;
//import MouseList;


class ImageMap extends JLabel implements Serializable,Runnable
{

   StateList SL;
   ImageIcon ImageI;
   public int XPix;
   public int YPix;
   public int DrawCoorX;
   public int DrawCoorY;
   public  int DrawCoorX1;
   public int DrawCoorY1;
   ICell Clicks[][];
   Thread runner;
   int XCoor=0;
   int YCoor=0;
   public int MousePressX;
   public int MousePressY;
   public int StateNum;
   public int Dragger = 0;
   public int DrawToX;
   public int DrawToY;
   public int AnchorX;
   public int AnchorY;
   MouseList MyMouseListener;
   int CycleNum = 0;


   //empty constructor
   ImageMap()
   {};

   //this constructer take the state list and the size of the grid to be
   //drawn
   ImageMap(StateList SList, int x, int y)
   {
      SL = SList;
      XPix=x;
      YPix=y;
      Clicks = new ICell[x][y];
  
      //initializes the objects which will contain information about cells
      //all classes must be instansiated, else there will be null pointer
      //exceptions thrown.
      for(int x1 = 0; x1<x; x1 +=20)
         for(int y1 = 0; y1 < y; y1 +=20)
            Clicks[x1][y1] = new ICell( SL.getSize()-1);
   };

   public void paint(Graphics g)
   {
          for(int x = 0 ;x<XPix ; x+=20)
            for(int y = 0; y<YPix ; y+=20)
            {
            if(Clicks[x][y].getCurrentState() > SL.getSize()-1)
               Clicks[x][y].setCurrentState(SL.getSize()-1);
     //if the cell is selected get its image and paint it
               ImageI = SL.getImage(Clicks[x][y].getCurrentState());
               g.drawImage(ImageI.getImage(),x,y,this);
            }
      //paints the grid
         for(int x=20;x<XPix;x=x+20)
          g.drawLine(x,0,x,YPix);
         for(int y=20;y<YPix;y=y+20)
          g.drawLine(0,y,XPix,y);

      //draws feedback
          if(Dragger == 1)
          {
            g.drawRect(AnchorX,AnchorY,DrawToX-AnchorX,DrawToY-AnchorY);
          }
      //draws Cycle Counter
          g.drawString("Cycle Number:"+CycleNum, 10,12);

   };


   //method to handle a single cell being added
   public void addCell(int x, int y, int StateNum)
   {
      try
      {
      DrawCoorX = (x - (x%20));
      DrawCoorY = (y - (y%20));
      Clicks[DrawCoorX][DrawCoorY] = new ICell(StateNum);
      ImageI = SL.getImage(Clicks[DrawCoorX][DrawCoorY]
                 .getCurrentState());
      repaint();
      }
      catch(ArrayIndexOutOfBoundsException e)
      {
         System.out.println("Attempting to add off the map");
      }
   };

   //method to handle a group of cells being added
   public void addGroupCell(int x1, int y1, int x2, int y2, int StateNum)
   {
   try
   {
      DrawCoorX = (x1 - (x1%20));
      DrawCoorX1 = (x2 - (x2%20));
      DrawCoorY = (y1 - (y1%20));
      DrawCoorY1 = (y2 - (y2%20));
      for(int countx = DrawCoorX; countx <= DrawCoorX1; countx += 20)
         for(int county = DrawCoorY; county <= DrawCoorY1; county += 20)
            Clicks[countx][county] = new ICell(StateNum);
      repaint();
    }
      catch(ArrayIndexOutOfBoundsException e)
      {
         System.out.println("Attempting to add off the map");
      }

   };

   //returns the size of the X dimension in pixels
   public int getXPix()
   {
      return(XPix);
   }
      
   //returns the size of the Y dimension in pixels
   public int getYPix()
   {
      return(YPix);
   }

   //returns the current StateList
   public StateList getStateList()
   {
      return(SL);
   }

   //starts the thread
   public void start()
   {
      if(runner == null)
      {
         runner = new Thread(this);
         runner.start();
      }
   }

   //method to handle the repainting
   public void run()
   {
      Thread thisthread = Thread.currentThread();
      while(thisthread == runner)
      {
         SL.startSimCycle(this);
         calcNextState();
         pause();
         ++CycleNum;
         repaint();
      }
   };

   //method to pauses for 100 ms
   public void pause()
   {
      try
      {
         Thread.sleep(200);
      }
      catch(Exception e)
      {
         System.out.println(""+e.getMessage());
      }
   };

   //method to stop the thread
   public void stop()
   {
      if(runner != null)
      {
         runner = null;
         CycleNum = 0;
      }
   };

   public void calcNextState()
   {
       for(XCoor = 0; XCoor < XPix; XCoor += 20)
         {
         for(YCoor = 0; YCoor < YPix; YCoor +=20)
            {
            Clicks[XCoor][YCoor].setCurrentState( SL.cellsNextState( XCoor/20, YCoor/20) );
            }
         }
   };

    public void setStateList(StateList SList)
    {
       SL = SList;
    }

    


    public void setSelectedState(int select)
    {
      MyMouseListener = new MouseList(this);
      addMouseListener(MyMouseListener);
      addMouseMotionListener(MyMouseListener);
      StateNum = select;
    }
}
