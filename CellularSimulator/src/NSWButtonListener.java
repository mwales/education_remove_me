// Filename:  NSWButtonListener.java
// Author:  Michael Wales
// Date:  November 8, 2000
// Version:  1.0

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;

public class NSWButtonListener implements ActionListener
   {

   private NewStateWindow NSW;
   private TransitionWindow TW;

   public NSWButtonListener ( NewStateWindow x, TransitionWindow y)
      {
      NSW = x;
      TW = y;
      }

   public void actionPerformed (ActionEvent E)
      {
      String Command = E.getActionCommand();

      // System.out.println( Command );

      if ( Command.equals("NSWCancel") )
         {
         NSW.done();
         }

      if ( Command.equals("NSWOK") )
         {
         String Param = NSW.ParamField.getText();

         if ( !Param.equals("") )
            {

            ImageFilter IFilter = new ImageFilter();

            JFileChooser PickImage = new JFileChooser();
            PickImage.addChoosableFileFilter( IFilter );

            int FileFlag = PickImage.showOpenDialog( NSW.StateFrame );

            if ( FileFlag == JFileChooser.APPROVE_OPTION )
               {
               File ImageFile = PickImage.getSelectedFile();
               String Filename = ImageFile.getPath();

               // System.out.println("Selected File:  " + Filename );
               // System.out.println("Corresponding state description:  " + Param);

               String ErrorMsg = TW.SimulationStates.addNewState( Param, Filename);

               // System.out.println("Errors: " + ErrorMsg);
               if ( ErrorMsg != null )
                  {
                  JOptionPane.showMessageDialog(NSW.StateFrame, ErrorMsg,
                        "State Creation Error",JOptionPane.ERROR_MESSAGE);
                  }
               else
                  TW.PrecedenceListModel.addElement( NSW.ParamField.getText() );
                  

               NSW.done();
               }
            else
               {
               JOptionPane.showMessageDialog(NSW.StateFrame, "Cannot create new state",
                     "No file selected error",JOptionPane.ERROR_MESSAGE);
               NSW.done();
               }
            }
         }

      }





   }
