// TransitionE.java
// Author:  Michael Wales
// Version: 1.0
// Date:  November 8, 2000

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;

public class TransitionE implements Serializable
   {

   public static final String Description="Firelike";
   public static final String Param1Description="Lowest state to burn";
   public static final String Param2Description="Burnout rate";
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
      int NumOfSurroundingFires=0;
      boolean WillCatch = false;

      //Check cell above for fire
      if (YCell - 1 < 0)
         NumOfSurroundingFires++;
      else
         {
         if (SN == SimulationArea.getStateNum(XCell, YCell-1) )
            {
            NumOfSurroundingFires++;
            WillCatch = true;
            }
         }

      //Check below for fire
      if (YCell+1 >= SimulationArea.getYDim() )
         NumOfSurroundingFires++;
      else
         {
         if (SN == SimulationArea.getStateNum(XCell, YCell+1) )
            {
            NumOfSurroundingFires++;
            WillCatch = true;
            }
         }

      //Check cell to the left
      if (XCell - 1 < 0)
         NumOfSurroundingFires++;
      else
         {
         if (SN == SimulationArea.getStateNum(XCell-1, YCell) )
            {
            NumOfSurroundingFires++;
            WillCatch = true;
            }
         }

      //Check cell on right for fire
      if (XCell+1 >= SimulationArea.getXDim() )
         NumOfSurroundingFires++;
      else
         {
         if (SN == SimulationArea.getStateNum(XCell+1, YCell) )
            {
            NumOfSurroundingFires++;
            WillCatch = true;
            }
         }

      //If on fire and surrounded by fire, false
      if ( SN == SimulationArea.getStateNum( XCell, YCell) )
         {
         if (NumOfSurroundingFires == 4)
            return false;
         // Do calculation for burnout rate
         if ( SimulationArea.getRandomNumber(100) <= Param2Value )
            return false;
         return true;
         }

      if (SimulationArea.getStateNum(XCell, YCell) <= Param1Value)
         return WillCatch;

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
