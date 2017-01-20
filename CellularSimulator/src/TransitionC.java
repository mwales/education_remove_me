// TransitionC.java
// Author:  Michael Wales
// Version: 1.0
// Date:  November 8, 2000

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;

public class TransitionC implements Serializable
   {

   public static final String Description="Evolution";
   public static final String Param1Description="Evolve from";
   public static final String Param2Description="# of Neighbors";
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
      int NumOfSurrounding=0;

      //Check cell above for fire
      if (YCell - 1 < 0)
         NumOfSurrounding++;
      else
         {
         if ( (Param1Value == SimulationArea.getStateNum(XCell, YCell-1)) || (SN == SimulationArea.getStateNum(XCell, YCell-1)) )
            {
            NumOfSurrounding++;
            }
         }

      //Check below for fire
      if (YCell+1 >= SimulationArea.getYDim() )
         NumOfSurrounding++;
      else
         {
         if ( (Param1Value == SimulationArea.getStateNum(XCell, YCell+1)) || (SN == SimulationArea.getStateNum(XCell, YCell+1)) )
            {
            NumOfSurrounding++;
            }
         }

      //Check cell to the left
      if (XCell - 1 < 0)
         NumOfSurrounding++;
      else
         {
         if ( (Param1Value == SimulationArea.getStateNum(XCell-1, YCell)) || (SN == SimulationArea.getStateNum(XCell-1, YCell)) )
            {
            NumOfSurrounding++;
            }
         }

      //Check cell on right for fire
      if (XCell+1 >= SimulationArea.getXDim() )
         NumOfSurrounding++;
      else
         {
         if ( (Param1Value == SimulationArea.getStateNum(XCell+1, YCell)) || (SN == SimulationArea.getStateNum(XCell+1, YCell)) )
            {
            NumOfSurrounding++;
            }
         }

      if (NumOfSurrounding >= Param2Value)
         return true;

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
