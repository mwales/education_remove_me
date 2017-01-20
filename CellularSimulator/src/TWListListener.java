// Filename:  TWListListener.java
// Author:  Michael Wales
// Version:  1.0
// Date:  November 10, 2000

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;

public class TWListListener implements ListSelectionListener
   {
   TransitionWindow TW;
   int LastIndex, NewIndex;
   String LastIndexName;
   boolean UserSelection;

   public TWListListener ( TransitionWindow X )
      {
      TW = X;
      LastIndex=-1;
      LastIndexName = null;
      UserSelection = false;
      }
    
   public void valueChanged( ListSelectionEvent E )
      {
      if (E.getValueIsAdjusting() == true)
         {
         UserSelection = true;
         return;
         }

      NewIndex = TW.PrecedenceList.getSelectedIndex();
      
      if ( UserSelection)
         {
         // System.out.println("Valid user selection = " + NewIndex);
         UserSelection = false;
         LastIndex = TW.SimulationStates.getStateNumber( LastIndexName );

         // Check to make sure old state was not deleted
         if (LastIndex != -1)
            {
            // System.out.println("Last state valid");
            // Save all the old transition data
            TransitionHolder OldTrans = new TransitionHolder();
            parseTransitionA( OldTrans );
            parseTransitionB( OldTrans );
            parseTransitionC( OldTrans );
            parseTransitionD( OldTrans );
            parseTransitionE( OldTrans );
            TW.SimulationStates.setTransitions( LastIndex, OldTrans);
            }

         // Load new transition setting into GUI
         TransitionHolder NewTrans = new TransitionHolder();
         NewTrans = TW.SimulationStates.getTransitions( NewIndex, NewTrans);
         setTransitionA( NewTrans );
         setTransitionB( NewTrans );
         setTransitionC( NewTrans );
         setTransitionD( NewTrans );
         setTransitionE( NewTrans );
         }

      LastIndexName = (String) TW.PrecedenceListModel.getElementAt( NewIndex );
      // System.out.println("Last index set to:  " + LastIndexName);
      TW.update();
      }

   public void parseTransitionA( TransitionHolder OT )
      {
      String TempText;
      int TempVal;

      // Check parameter 1
      if (TransitionA.UseParam1)
         {
         // Read the text field and convert to int
         TempText = OT.TA.Param1TextField.getText();
         try
            {
            TempVal = Integer.parseInt( TempText );
            }
         catch ( NumberFormatException e)
            {
            TempVal = 0;
            }

         // Save the text field in the OldTrans object
         OT.TA.setParam1Value(TempVal);
         }
      
      // Check parameter 2
      if (TransitionA.UseParam2)
         {
         // Read the text field and convert to int
         TempText = OT.TA.Param2TextField.getText();
         try
            {
            TempVal = Integer.parseInt( TempText );
            }
         catch ( NumberFormatException e)
            {
            TempVal = 0;
            }

         // Save the text field in the OldTrans object
         OT.TA.setParam2Value(TempVal);
         }

      // Check checkbox state
      boolean CS = TW.CheckState[0];
      OT.TA.setIfInUse( CS );  
      
      }

   public void setTransitionA( TransitionHolder NT)
      {
      Integer Temp = null;
      String TS = null;

      if (TransitionA.UseParam1)
         {
         //Setting Parameter 1
         try
            {
            Temp = new Integer( NT.TA.getParam1Value() );
            TS = Temp.toString();
            }
         catch(Exception e)
            {
            TS = "0";
            }
         TW.DisplayTrans.TA.Param1TextField.setText( TS );
         }

      if (TransitionA.UseParam2)
         {
            try
            {
            Temp = new Integer( NT.TA.getParam2Value() );
            TS = Temp.toString();
            }
         catch(Exception e)
            {
            TS = "0";
            }
         TW.DisplayTrans.TA.Param2TextField.setText( TS );
         }

      try
         {
         TW.CheckState[0] = NT.TA.getIfInUse();
         }
      catch(Exception e)
         {
         TW.CheckState[0] = false;
         }

      TW.DisplayTrans.TA.StatusBox.setSelected( TW.CheckState[0] );
      // System.out.println(TS);  
      }

   public void parseTransitionB( TransitionHolder OT )
      {
      String TempText;
      int TempVal;

      // Check parameter 1
      if (TransitionB.UseParam1)
         {
         // Read the text field and convert to int
         TempText = OT.TB.Param1TextField.getText();
         try
            {
            TempVal = Integer.parseInt( TempText );
            }
         catch ( NumberFormatException e)
            {
            TempVal = 0;
            }

         // Save the text field in the OldTrans object
         OT.TB.setParam1Value(TempVal);
         }
      
      // Check parameter 2
      if (TransitionB.UseParam2)
         {
         // Read the text field and convert to int
         TempText = OT.TB.Param2TextField.getText();
         try
            {
            TempVal = Integer.parseInt( TempText );
            }
         catch ( NumberFormatException e)
            {
            TempVal = 0;
            }

         // Save the text field in the OldTrans object
         OT.TB.setParam2Value(TempVal);
         }

      // Check checkbox state
      boolean CS = TW.CheckState[1];
      OT.TB.setIfInUse( CS );  
      
      }

   public void setTransitionB( TransitionHolder NT)
      {
      Integer Temp = null;
      String TS = null;

      if (TransitionB.UseParam1)
         {
         //Setting Parameter 1
         try
            {
            Temp = new Integer( NT.TB.getParam1Value() );
            TS = Temp.toString();
            }
         catch(Exception e)
            {
            TS = "0";
            }
         TW.DisplayTrans.TB.Param1TextField.setText( TS );
         }

      if (TransitionB.UseParam2)
         {
            try
            {
            Temp = new Integer( NT.TB.getParam2Value() );
            TS = Temp.toString();
            }
         catch(Exception e)
            {
            TS = "0";
            }
         TW.DisplayTrans.TB.Param2TextField.setText( TS );
         }

      try
         {
         TW.CheckState[1] = NT.TB.getIfInUse();
         }
      catch(Exception e)
         {
         TW.CheckState[1] = false;
         }

      TW.DisplayTrans.TB.StatusBox.setSelected( TW.CheckState[1] );
      // System.out.println(TS);  
      }

   public void parseTransitionC( TransitionHolder OT )
      {
      String TempText;
      int TempVal;

      // Check parameter 1
      if (TransitionC.UseParam1)
         {
         // Read the text field and convert to int
         TempText = OT.TC.Param1TextField.getText();
         try
            {
            TempVal = Integer.parseInt( TempText );
            }
         catch ( NumberFormatException e)
            {
            TempVal = 0;
            }

         // Save the text field in the OldTrans object
         OT.TC.setParam1Value(TempVal);
         }
      
      // Check parameter 2
      if (TransitionC.UseParam2)
         {
         // Read the text field and convert to int
         TempText = OT.TC.Param2TextField.getText();
         try
            {
            TempVal = Integer.parseInt( TempText );
            }
         catch ( NumberFormatException e)
            {
            TempVal = 0;
            }

         // Save the text field in the OldTrans object
         OT.TC.setParam2Value(TempVal);
         }

      // Check checkbox state
      boolean CS = TW.CheckState[2];
      OT.TC.setIfInUse( CS );  
      
      }

   public void setTransitionC( TransitionHolder NT)
      {
      Integer Temp = null;
      String TS = null;

      if (TransitionC.UseParam1)
         {
         //Setting Parameter 1
         try
            {
            Temp = new Integer( NT.TC.getParam1Value() );
            TS = Temp.toString();
            }
         catch(Exception e)
            {
            TS = "0";
            }
         TW.DisplayTrans.TC.Param1TextField.setText( TS );
         }

      if (TransitionC.UseParam2)
         {
            try
            {
            Temp = new Integer( NT.TC.getParam2Value() );
            TS = Temp.toString();
            }
         catch(Exception e)
            {
            TS = "0";
            }
         TW.DisplayTrans.TC.Param2TextField.setText( TS );
         }

      try
         {
         TW.CheckState[2] = NT.TC.getIfInUse();
         }
      catch(Exception e)
         {
         TW.CheckState[2] = false;
         }

      TW.DisplayTrans.TC.StatusBox.setSelected( TW.CheckState[2] );
      // System.out.println(TS);  
      }

   public void parseTransitionD( TransitionHolder OT )
      {
      String TempText;
      int TempVal;

      // Check parameter 1
      if (TransitionD.UseParam1)
         {
         // Read the text field and convert to int
         TempText = OT.TD.Param1TextField.getText();
         try
            {
            TempVal = Integer.parseInt( TempText );
            }
         catch ( NumberFormatException e)
            {
            TempVal = 0;
            }

         // Save the text field in the OldTrans object
         OT.TD.setParam1Value(TempVal);
         }
      
      // Check parameter 2
      if (TransitionD.UseParam2)
         {
         // Read the text field and convert to int
         TempText = OT.TD.Param2TextField.getText();
         try
            {
            TempVal = Integer.parseInt( TempText );
            }
         catch ( NumberFormatException e)
            {
            TempVal = 0;
            }

         // Save the text field in the OldTrans object
         OT.TD.setParam2Value(TempVal);
         }

      // Check checkbox state
      boolean CS = TW.CheckState[3];
      OT.TD.setIfInUse( CS );
      
      }

   public void setTransitionD( TransitionHolder NT)
      {
      Integer Temp = null;
      String TS = null;

      if (TransitionD.UseParam1)
         {
         //Setting Parameter 1
         try
            {
            Temp = new Integer( NT.TD.getParam1Value() );
            TS = Temp.toString();
            }
         catch(Exception e)
            {
            TS = "0";
            }
         TW.DisplayTrans.TD.Param1TextField.setText( TS );
         }

      if (TransitionD.UseParam2)
         {
            try
            {
            Temp = new Integer( NT.TD.getParam2Value() );
            TS = Temp.toString();
            }
         catch(Exception e)
            {
            TS = "0";
            }
         TW.DisplayTrans.TD.Param2TextField.setText( TS );
         }

      try
         {
         TW.CheckState[3] = NT.TD.getIfInUse();
         }
      catch(Exception e)
         {
         TW.CheckState[3] = false;
         }

      TW.DisplayTrans.TD.StatusBox.setSelected( TW.CheckState[3] );
      // System.out.println(TS);  
      }

   public void parseTransitionE( TransitionHolder OT )
      {
      String TempText;
      int TempVal;

      // Check parameter 1
      if (TransitionE.UseParam1)
         {
         // Read the text field and convert to int
         TempText = OT.TE.Param1TextField.getText();
         try
            {
            TempVal = Integer.parseInt( TempText );
            }
         catch ( NumberFormatException e)
            {
            TempVal = 0;
            }

         // Save the text field in the OldTrans object
         OT.TE.setParam1Value(TempVal);
         }
      
      // Check parameter 2
      if (TransitionE.UseParam2)
         {
         // Read the text field and convert to int
         TempText = OT.TE.Param2TextField.getText();
         try
            {
            TempVal = Integer.parseInt( TempText );
            }
         catch ( NumberFormatException e)
            {
            TempVal = 0;
            }

         // Save the text field in the OldTrans object
         OT.TE.setParam2Value(TempVal);
         }

      // Check checkbox state
      boolean CS = TW.CheckState[4];
      OT.TE.setIfInUse( CS );
      
      }

   public void setTransitionE( TransitionHolder NT)
      {
      Integer Temp = null;
      String TS = null;

      if (TransitionE.UseParam1)
         {
         //Setting Parameter 1
         try
            {
            Temp = new Integer( NT.TE.getParam1Value() );
            TS = Temp.toString();
            }
         catch(Exception e)
            {
            TS = "0";
            }
         TW.DisplayTrans.TE.Param1TextField.setText( TS );
         }

      if (TransitionE.UseParam2)
         {
            try
            {
            Temp = new Integer( NT.TE.getParam2Value() );
            TS = Temp.toString();
            }
         catch(Exception e)
            {
            TS = "0";
            }
         TW.DisplayTrans.TE.Param2TextField.setText( TS );
         }

      try
         {
         TW.CheckState[4] = NT.TE.getIfInUse();
         }
      catch(Exception e)
         {
         TW.CheckState[4] = false;
         }

      TW.DisplayTrans.TE.StatusBox.setSelected( TW.CheckState[4] );
      }           

       

   }
