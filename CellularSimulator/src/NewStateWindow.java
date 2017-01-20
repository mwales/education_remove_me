// Filename:  NewStateWindow.java
// Author:  Michael Wales
// Date:  November 3, 2000
// Version:  1.0B

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class NewStateWindow
   {

   public JTextField ParamField;
   public JFrame StateFrame;
   private JButton OKButton, CancelButton;
   private JPanel ContentPane;
   private JLabel Message;
   private GridLayout NSWGridLayout;
   private NSWButtonListener NSWBL;
   public TransitionWindow TW;

   public NewStateWindow(TransitionWindow x)
      {
      TW = x;

      StateFrame = new JFrame("New State");
      NSWGridLayout = new GridLayout(2,2);
      ContentPane = new JPanel( NSWGridLayout );
      NSWBL = new NSWButtonListener( this , TW ); 

      // Initialize buttons
      OKButton = new JButton("OK");
      OKButton.setActionCommand("NSWOK");
      OKButton.addActionListener( NSWBL );
      CancelButton = new JButton("Cancel");
      CancelButton.setActionCommand("NSWCancel");
      CancelButton.addActionListener( NSWBL );

      // Initialize the text field
      ParamField = new JTextField();
      }

   public void show()
      {
      // Add components to the content pane
      Message = new JLabel("Enter the name of the new state");
      ContentPane.add( Message );
      ContentPane.add( ParamField );
      ContentPane.add( OKButton );
      ContentPane.add( CancelButton );

      ContentPane.setOpaque( true );

      StateFrame.setContentPane( ContentPane );
      StateFrame.pack();
      StateFrame.setVisible( true ); 
      }

   public void done()
      {
      StateFrame.setVisible( false );
      StateFrame = null;
      TW.ContentPane.updateUI();
      TW.TransitionMenuBar.updateUI();
      }

   };
