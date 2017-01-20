// Filename:  TWButtonListener.java
// Author:  Michael Wales
// Version:  1.0
// Date:  November 8, 2000

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class TWButtonListener implements ActionListener
   {
   TransitionWindow TW;

   public TWButtonListener ( TransitionWindow X )
      {
      TW = X;
      }

   public void actionPerformed ( ActionEvent E )
      {
      String WhichButton = E.getActionCommand();

      // Uncomment to print out when a button is pressed
      // System.out.println(WhichButton);

      if ( WhichButton.equals("OKTW") )
         {
         int CurrentIndex = TW.PrecedenceList.getSelectedIndex();

         if (CurrentIndex >= 0)
            {
            // Save all the old transition data
            TransitionHolder OldTrans = new TransitionHolder();
            parseTransitionA( OldTrans );
            parseTransitionB( OldTrans );
            parseTransitionC( OldTrans );
            parseTransitionD( OldTrans );
            parseTransitionE( OldTrans );
            TW.SimulationStates.setTransitions( CurrentIndex, OldTrans);
            }

         CSim.TransitionWindowFinished( TW.SimulationStates );

         }

      if ( WhichButton.equals("CancelTW") )
         {
         CSim.TransitionWindowFinished( TW.OldStateList );
         }

      if ( WhichButton.equals("MoveUpTW") )
         {
         int IndexNum = TW.PrecedenceList.getSelectedIndex();

         if ( IndexNum > 0)
            {
            // Switch the items on the GUI List
            String ListText = (String) TW.PrecedenceListModel.remove( IndexNum );
            TW.PrecedenceListModel.insertElementAt( ListText, IndexNum -1 );
            TW.PrecedenceList.setSelectedIndex( IndexNum-1 );

            // Switch the items on the StateList
            int Error = TW.SimulationStates.swapStates( IndexNum, IndexNum -1 );
            if ( Error != 0 )
               System.out.println("Internal Error:  Couldn't swap states");
            }
         }

      if ( WhichButton.equals("MoveDownTW") )
         {
         int IndexNum = TW.PrecedenceList.getSelectedIndex();

         if ( (IndexNum != -1) && (IndexNum < (TW.PrecedenceListModel.size()-1)) )
            {
            // Switch the items on the GUI List
            String ListText = (String) TW.PrecedenceListModel.remove( IndexNum );
            TW.PrecedenceListModel.insertElementAt( ListText, IndexNum + 1 );
            TW.PrecedenceList.setSelectedIndex( IndexNum + 1 );

            // Switch the items on the StateList
            int Error = TW.SimulationStates.swapStates( IndexNum, IndexNum + 1 );
            if ( Error != 0 )
               System.out.println("Internal Error:  Couldn't swap states");
            }
         }

      if ( WhichButton.equals("CreateStateTW") )
         {
         NewStateWindow NSW = new NewStateWindow( TW );
         NSW.show();
         }

      if ( WhichButton.equals("DeleteStateTW") )
         {
         if (TW.SimulationStates.getSize() <= 1)
            {
            String ErrorMsg = "Can only delete a state if there are at least 2 states in simulation";
            JOptionPane.showMessageDialog(TW.TransitionsFrame, ErrorMsg,
               "State Deletion Error",
               JOptionPane.ERROR_MESSAGE);
            return;
            }

         int IndexNum = TW.PrecedenceList.getSelectedIndex();
         if ( IndexNum != -1)
            {
            // Remove from GUI list
            String SD = (String) TW.PrecedenceListModel.remove( IndexNum );

            // Remove from the state list for the simulation
            int ErrorNum = TW.SimulationStates.deleteState( SD );
            if ( ErrorNum == -1)
               System.out.println("Internal Error:  Couldn't remove state");
            }
         }

      }

   public void parseTransitionA( TransitionHolder OT )
      {
      String TempText;
      int TempVal;

      // Check parameter 1
      if (OT.TA.UseParam1)
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
      if (OT.TA.UseParam2)
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

   public void parseTransitionB( TransitionHolder OT )
      {
      String TempText;
      int TempVal;

      // Check parameter 1
      if (OT.TB.UseParam1)
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
      if (OT.TB.UseParam2)
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

   public void parseTransitionC( TransitionHolder OT )
      {
      String TempText;
      int TempVal;

      // Check parameter 1
      if (OT.TC.UseParam1)
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
      if (OT.TC.UseParam2)
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

   public void parseTransitionD( TransitionHolder OT )
      {
      String TempText;
      int TempVal;

      // Check parameter 1
      if (OT.TD.UseParam1)
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
      if (OT.TD.UseParam2)
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

   public void parseTransitionE( TransitionHolder OT )
      {
      String TempText;
      int TempVal;

      // Check parameter 1
      if (OT.TE.UseParam1)
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
      if (OT.TE.UseParam2)
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


   };
