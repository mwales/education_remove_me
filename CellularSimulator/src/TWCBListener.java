// Filename:  TWCBListener.java
// Author:  Michael Wales
// Version:  1.0
// Date:  October 29, 2000

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class TWCBListener implements ItemListener
   {

   private TransitionWindow TW;

   public TWCBListener ( TransitionWindow y)
      {
      TW = y;
      }

   public void itemStateChanged (ItemEvent E)
      {
      Object Source;
      int TransNum=0;

      // Which Checkbox fired the event
      Source = E.getItemSelectable();
      if ( Source == TW.DisplayTrans.TA.StatusBox)
         TransNum = 0;
      if ( Source == TW.DisplayTrans.TB.StatusBox)
         TransNum = 1;
      if ( Source == TW.DisplayTrans.TC.StatusBox)
         TransNum = 2;
      if ( Source == TW.DisplayTrans.TD.StatusBox)
         TransNum = 3;
      if ( Source == TW.DisplayTrans.TE.StatusBox)
         TransNum = 4;

      if ( E.getStateChange() == ItemEvent.DESELECTED )
         TW.CheckState[TransNum] = false;
      else
         TW.CheckState[TransNum] = true; 
      }

   };

   
