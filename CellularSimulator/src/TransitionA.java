// TransitionA.java
// Author:  Michael Wales
// Version: 1.0
// Date:  October 22, 2000

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;

public class TransitionA implements Serializable
   {

   public static final String Description="Movement";
   public static final String Param1Description="Direction";
   public static final String Param2Description="Trails";
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
      int XDim = SimulationArea.getXDim();
      int YDim = SimulationArea.getYDim();
      int WhereItWas;

      //Check for trailings
      if (Param2Value == 1)
         {
         if (SN == SimulationArea.getStateNum( XCell, YCell))
            return true;
         }

      switch (Param1Value)
         {
         // Moving left
         case 180:
         case -180:

            //Special case:  On right side of the map
            if (XCell + 1 >= XDim)
               return false;

            WhereItWas = SimulationArea.getStateNum( XCell+1, YCell );
            if (WhereItWas == SN)
               return true;
            return false;

         //Moving up
         case 90:
         case -270:

            //Special case:  Bottom of map
            if (YCell + 1 >= YDim)
               return false;

            WhereItWas = SimulationArea.getStateNum( XCell, YCell+1 );
            if (WhereItWas == SN)
               return true;
            return false;

         //Moving right
         case 0:
         case -360:
         case 360:

            //Special case:  Left side of the map
            if (XCell - 1 < 0)
               return false;

            WhereItWas = SimulationArea.getStateNum( XCell-1, YCell );
            if (WhereItWas == SN)
               return true;
            return false;

         //Moving down
         case -90:
         case 270:

            //Special case:  Top of map
            if (YCell - 1 < 0)
               return false;

            WhereItWas = SimulationArea.getStateNum( XCell, YCell-1 );
            if (WhereItWas == SN)
               return true;
            return false;



         default:           
            System.out.println("Invalid direction");

         }
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
