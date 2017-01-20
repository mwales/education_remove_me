// File:  StateList.java
// Author: Michael Wales
// Version:  1.0
// Date:  November 8, 2000

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;
import javax.swing.ImageIcon.*;
import java.io.*;

public class StateList implements Serializable
   {
   public Vector VPictures;
   public Vector VStateDescription;
   public Vector RulesA;
   public Vector RulesB;
   public Vector RulesC;
   public Vector RulesD;
   public Vector RulesE;
   private CellMap StateMap;

   public StateList( StateList S )
      {
      copyStateList(S);
      }

   public StateList()
      {
      VPictures = new Vector(5,5);
      VStateDescription = new Vector(5,5);
      RulesA = new Vector(5,5);
      RulesB = new Vector(5,5);
      RulesC = new Vector(5,5);
      RulesD = new Vector(5,5);
      RulesE = new Vector(5,5);
      }

   public String addNewState( String NewStateDescription, String Filename )
      {
      TransitionA TransA = null;
      TransitionB TransB = null;
      TransitionC TransC = null;
      TransitionD TransD = null;
      TransitionE TransE = null;

      String ErrorMsg = null;
      ImageIcon NewImage = new ImageIcon( Filename );

      int Width = NewImage.getIconWidth();
      int Height = NewImage.getIconHeight();

      // System.out.println( Height + " x " + Width );

      if ( Width == -1)
         {
         ErrorMsg = Filename + " not found!";
         return ErrorMsg;
         }
      if ( (Width != 20) || (Height != 20) )
         {
         ErrorMsg = "Image file is " + Width + "x" + Height + " and not 20x20";
         return ErrorMsg;
         }

      // System.out.println( NewStateDescription + "(" + Filename + ") loaded OK");

      VPictures.addElement( NewImage );
      VStateDescription.addElement( NewStateDescription );

      RulesA.addElement( TransA );
      RulesB.addElement( TransB );
      RulesC.addElement( TransC );
      RulesD.addElement( TransD );
      RulesE.addElement( TransE );

      return ErrorMsg;
      }

   public int deleteState( String StateName )
      {
      int counter;
      int StateNum = -1;
      String StateDescription;

      for ( counter = 0; counter < VStateDescription.size(); counter++ )
         {
         StateDescription = (String) VStateDescription.elementAt( counter );
         if ( StateDescription.equals( StateName ) )
            StateNum = counter;
         }

      // State not found
      if ( StateNum == -1)
         return -1;

      VStateDescription.removeElementAt( StateNum );
      VPictures.removeElementAt( StateNum );

      RulesA.removeElementAt( StateNum );
      RulesB.removeElementAt( StateNum );
      RulesC.removeElementAt( StateNum );
      RulesD.removeElementAt( StateNum );
      RulesE.removeElementAt( StateNum );

      // Successfull
      return 0;
      }

   public TransitionHolder getTransitions(int x, TransitionHolder y)
      {
      if ( (x >= VStateDescription.size()) || (x < 0) )
         return null;
      y = new TransitionHolder();

      y.TA = (TransitionA) RulesA.elementAt(x);
      y.TB = (TransitionB) RulesB.elementAt(x);
      y.TC = (TransitionC) RulesC.elementAt(x);
      y.TD = (TransitionD) RulesD.elementAt(x);
      y.TE = (TransitionE) RulesE.elementAt(x);

      return y;
      }

   public int setTransitions( int x, TransitionHolder y)
      {
      if ( (x >= VStateDescription.size()) || (x < 0) )
         return -1;

      RulesA.setElementAt(y.TA, x);
      RulesB.setElementAt(y.TB, x);
      RulesC.setElementAt(y.TC, x);
      RulesD.setElementAt(y.TD, x);
      RulesE.setElementAt(y.TE, x);

      return 0;
      }

   public int getSize()
      {
      return VStateDescription.size();
      }

   public String getStateName( int x )
      {
      if ( (x >= VStateDescription.size()) || (x < 0) )
         return null; 

      return (String) VStateDescription.elementAt(x);
      }

   public int getStateNumber( String x )
      {
      if (x == null)
         return -1;

      for( int count = 0; count < getSize(); count++)
         {
         if (x.equals( VStateDescription.elementAt(count) ))
            {
            return count;
            }
         }
      return -1;
      }

   public ImageIcon getImage( int x )
      {
      if ( (x >= VStateDescription.size()) || (x < 0) )
         return null; 

      return (ImageIcon) VPictures.elementAt(x);
      }

   public int swapStates( int State1, int State2)
      {
      // Make sure state ranges are valid
      if ( (State1 >= VStateDescription.size()) || (State1 < 0) )
         {
         System.out.println("State index out of range");
         return -1;
         }

      if ( (State2 >= VStateDescription.size()) || (State2 < 0) )
         {
         System.out.println("State index out of range");
         return -1;
         }

      ImageIcon TempImage = (ImageIcon) VPictures.elementAt(State1);
      String TempName = (String) VStateDescription.elementAt(State1);
      TransitionA TempTransA = (TransitionA) RulesA.elementAt(State1);
      TransitionB TempTransB = (TransitionB) RulesB.elementAt(State1);
      TransitionC TempTransC = (TransitionC) RulesC.elementAt(State1);
      TransitionD TempTransD = (TransitionD) RulesD.elementAt(State1);
      TransitionE TempTransE = (TransitionE) RulesE.elementAt(State1);

      VPictures.setElementAt( (ImageIcon)VPictures.elementAt(State2), State1);
      VStateDescription.setElementAt( (String)VStateDescription.elementAt(State2), State1);
      RulesA.setElementAt( (TransitionA)RulesA.elementAt(State2), State1);
      RulesB.setElementAt( (TransitionB)RulesB.elementAt(State2), State1);
      RulesC.setElementAt( (TransitionC)RulesC.elementAt(State2), State1);
      RulesD.setElementAt( (TransitionD)RulesD.elementAt(State2), State1);
      RulesE.setElementAt( (TransitionE)RulesE.elementAt(State2), State1);

      VPictures.setElementAt( TempImage, State2 );
      VStateDescription.setElementAt( TempName, State2 );
      RulesA.setElementAt( TempTransA, State2);
      RulesB.setElementAt( TempTransB, State2);
      RulesC.setElementAt( TempTransC, State2);
      RulesD.setElementAt( TempTransD, State2);
      RulesE.setElementAt( TempTransE, State2);

      return 0;
      }

   public void copyStateList(StateList Source)
      {
      VPictures = (Vector) Source.VPictures.clone();
      VStateDescription = (Vector) Source.VStateDescription.clone();
      RulesA = (Vector) Source.RulesA.clone();
      RulesB = (Vector) Source.RulesB.clone();
      RulesC = (Vector) Source.RulesC.clone();
      RulesD = (Vector) Source.RulesD.clone();
      RulesE = (Vector) Source.RulesE.clone();
      }

    public Vector getNameVector()
      {
      return(VStateDescription);
      }

   public void startSimCycle( ImageMap x )
      {
      StateMap = new CellMap( x, getSize() );

      // If you want to see the map's text representation
      // you can uncomment this line
      // StateMap.printMap();
      }

   public int cellsNextState( int x, int y)
      {
      if ( (x>=StateMap.getXDim()) || (x<0) || (y>=StateMap.getYDim()) || (y<0) )
         {
         System.out.println("Cell " + x + ", " + y + " is an invalid location");
         return -1;
         }

      TransitionHolder TempHolder = null;
      int NumOfStates = getSize();
      int NextState = -1;

      if ( NumOfStates <= 0)
         {
         System.out.println("ERROR:  Not enough states to simulate");
         return -1;
         }

      int StateCount = NumOfStates-1;
      while ( StateCount >= 0)
         {
         TempHolder = getTransitions(StateCount, TempHolder);
         if ( TempHolder == null)
            {
            System.out.println("Empty transition holder");
            return -1;
            }

         if (TempHolder.TA.getIfInUse())
            {
            if (TempHolder.TA.willCellTrans( StateMap, x, y, StateCount))
               NextState = StateCount;
            }

         if (TempHolder.TB.getIfInUse())
            {
            if (TempHolder.TB.willCellTrans( StateMap, x, y, StateCount))
               NextState = StateCount;
            }

         if (TempHolder.TC.getIfInUse())
            {
            if (TempHolder.TC.willCellTrans( StateMap, x, y, StateCount))
               NextState = StateCount;
            }

         if (TempHolder.TD.getIfInUse())
            {
            if (TempHolder.TD.willCellTrans( StateMap, x, y, StateCount))
               NextState = StateCount;
            }

         if (TempHolder.TE.getIfInUse())
            {
            if (TempHolder.TE.willCellTrans( StateMap, x, y, StateCount))
               NextState = StateCount;
            }

         StateCount--;
         }
      return NextState;
      }


   }

