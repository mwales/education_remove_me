// TransitionB.java
// Author:  Michael Wales
// Version: 1.0
// Date:  November 8, 2000

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.util.*;

public class TransitionB implements Serializable
   {

   public static final String Description = "Random";
   public static final String Param1Description = "Percentage";
   public static final String Param2Description = "Trails";
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
      if ( Param2Value == 1)
         {
         if (SimulationArea.getStateNum( XCell, YCell) == SN)
            return true;
         }
      
      int RandomNum = SimulationArea.getRandomNumber(100);
      if (RandomNum >= Param1Value)
         return false;
      return true;
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
