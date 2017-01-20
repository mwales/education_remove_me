// TransitionD.java
// Author:  Michael Wales
// Version: 1.0
// Date:  November 8, 2000

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;

public class TransitionD implements Serializable
   {

   public static final String Description="Game of Life";
   public static final String Param1Description="State # for living";
   public static final String Param2Description="State # for dead";
   public static final boolean UseParam1 = true;
   public static final boolean UseParam2 = true;
   public static JTextField Param1TextField;
   public static JTextField Param2TextField;
   public static JCheckBox StatusBox;

   private int Param1Value=0;
   private int Param2Value=0;
   private boolean InUse = false;

   public boolean willCellTrans ( CellMap SimulationArea, int XCell, int YCell, int SN)
      {
      if ( (Param1Value != SimulationArea.getStateNum(XCell, YCell)) && (Param2Value != SimulationArea.getStateNum(XCell,YCell)) ) 
         return false;

      int NumOfLiving=0;
      //Special case top row
      if (YCell-1 >= 0)
         {
         // Check top right
         if (XCell-1 >= 0)
            {
            if (Param1Value == SimulationArea.getStateNum(XCell-1, YCell-1))
               NumOfLiving++;
            }
         // Check top
         if ( Param1Value == SimulationArea.getStateNum(XCell, YCell-1) )
            {
            NumOfLiving++;
            }
         // Check top right
         if (XCell+1 < SimulationArea.getXDim())
            {
            if ( Param1Value == SimulationArea.getStateNum(XCell+1, YCell-1))
               NumOfLiving++;
            }
         }

      // print out top row living
      // System.out.print("top = " + NumOfLiving + "      ");

      // Check the right cell
      if (XCell+1 < SimulationArea.getXDim())
         {
         if ( Param1Value == SimulationArea.getStateNum(XCell+1, YCell))
            {
            NumOfLiving++;
            }
         }

      // Check left state
      if ( XCell-1 >= 0)
         {
         if ( Param1Value == SimulationArea.getStateNum( XCell-1, YCell))
            {
            NumOfLiving++;
            }
         }

      // println out landr
      // System.out.print("l and r=" + NumOfLiving +"     ");

      // Checking the bottom row
      if (YCell+1 < SimulationArea.getYDim())
         {
         // Check bottom right
         if (XCell+1 < SimulationArea.getXDim())
            {
            if ( Param1Value == SimulationArea.getStateNum(XCell+1, YCell+1))
               NumOfLiving++;
            }
         // Check bottom
         if ( Param1Value == SimulationArea.getStateNum(XCell, YCell+1))
            NumOfLiving++;
         // Check bottom left
         if ( XCell-1 >= 0 )
            {
            if ( Param1Value == SimulationArea.getStateNum( XCell-1, YCell+1))
               NumOfLiving++;
            }
         }

      //print out bottom
      // System.out.println("bot = " + NumOfLiving);

      int CurrentState = SimulationArea.getStateNum( XCell, YCell);
      if ( SN == Param1Value)
         {
         if ( CurrentState == Param1Value)
            {
            if ( (NumOfLiving==2) || (NumOfLiving==3) )
               return true;
            else
               return false;
            }
         if ( CurrentState == Param2Value)
            {
            if ( NumOfLiving == 3 )
               return true;
            else
               return false;
            }
         }

      if ( SN == Param2Value )
         {
         if (NumOfLiving == 3)
            return false;
         else
            return true;
         }

      // Bad Case:
      return false;
      }

   public void setParam1Value( int x)
      {
      Param1Value = x;
      }

   public int getParam1Value ()
      {
      return Param1Value;
      }

   public void setParam2Value ( int x )
      {
      Param2Value = x;
      }

   public int getParam2Value ()
      {
      return Param2Value;
      }

   public void setIfInUse( boolean x)
      {
      InUse = x;
      }

   public boolean getIfInUse()
      {
      return InUse;
      }

   public void setAllValues( boolean x, int y, int z)
      {
      InUse = x;
      Param1Value = y;
      Param2Value = z;
      }
   };
