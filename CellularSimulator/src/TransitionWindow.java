// File:  TransitionWindow.java
// Author:  Michael Wales
// Version:  1.0
// Date:  November 9, 2000

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import java.util.*;
import javax.swing.event.*;
import javax.swing.table.*;

public class TransitionWindow
   {
   // GUI Components
   public JFrame TransitionsFrame;
   public JPanel ContentPane;
   private GridBagLayout GBLayout;
   private GridBagConstraints GBConstraints;
   private JMenu StateMenu;
   public JMenuBar TransitionMenuBar;
   private JPanel TransitionAPanel;
   private JPanel TransitionBPanel;
   private JPanel TransitionCPanel;
   private JPanel TransitionDPanel;
   private JPanel TransitionEPanel;
   private JButton OKButton;
   private JButton CancelButton;
   private JPanel PrecedencePanel;
   private JButton UpButton, DownButton;
   public JList PrecedenceList;
   public DefaultListModel PrecedenceListModel;
   private JScrollPane ListScrollPane;
   private TWButtonListener TWBL;
   public StateList SimulationStates;
   public StateList OldStateList;
   public TransitionHolder DisplayTrans;
                                 
   
   // Transition Property Classes
   public boolean CheckState[];

   private void setupPrecedencePanel()
      {
      GridBagLayout PPGBLayout;
      GridBagConstraints PPGBConstraints = new GridBagConstraints();
      PPGBLayout = new GridBagLayout();
      PrecedencePanel = new JPanel( PPGBLayout );

      //Setup list model
      int size = SimulationStates.getSize();
      PrecedenceListModel = new DefaultListModel();
      for ( int counter=0; counter<size; counter++)
         {
         PrecedenceListModel.addElement( SimulationStates.getStateName(counter) );
         }

      //Setup the TransitionHolders corresponding to the states


      PrecedenceList = new JList( PrecedenceListModel );
      ListScrollPane = new JScrollPane ( PrecedenceList );
      ListScrollPane.setPreferredSize( new Dimension ( 100, 300 ) );
                                                 
      PrecedenceList.setSelectionMode(  ListSelectionModel.SINGLE_SELECTION );

      PPGBConstraints.gridx = 1;
      PPGBConstraints.gridy = 1;
      PPGBConstraints.gridwidth = 1;
      PPGBConstraints.gridheight = 4;
      PPGBLayout.setConstraints( ListScrollPane, PPGBConstraints);
      PrecedencePanel.add( ListScrollPane );

      //Setup up button
      UpButton = new JButton("Move Up");
      UpButton.setActionCommand("MoveUpTW");
      PPGBConstraints.gridx = 2;
      PPGBConstraints.gridy = 2;
      PPGBConstraints.gridwidth = 1;
      PPGBConstraints.gridheight = 1;
      PPGBConstraints.fill = GridBagConstraints.HORIZONTAL;
      PPGBLayout.setConstraints( UpButton, PPGBConstraints);
      PrecedencePanel.add( UpButton );

      //Setup down button
      DownButton = new JButton("Move Down");
      DownButton.setActionCommand("MoveDownTW");
      PPGBConstraints.gridx = 2;
      PPGBConstraints.gridy = 3;
      PPGBConstraints.gridwidth = 1;
      PPGBConstraints.gridheight = 1;
      PPGBConstraints.fill = GridBagConstraints.HORIZONTAL;
      PPGBLayout.setConstraints( DownButton, PPGBConstraints);
      PrecedencePanel.add( DownButton );

      PrecedencePanel.setOpaque( true );
      }

   private void setupMenuBar()
      {
      JMenuItem TransitionMenuItem;
      TransitionMenuBar = new JMenuBar();
      
      //Construct the State Menu
      StateMenu = new JMenu("States");

      TransitionMenuItem = new JMenuItem("Add a new state...");
      TransitionMenuItem.setActionCommand("CreateStateTW");
      StateMenu.add( TransitionMenuItem );
      TransitionMenuItem.addActionListener( TWBL );

      TransitionMenuItem = new JMenuItem("Delete a state");
      TransitionMenuItem.setActionCommand("DeleteStateTW");
      StateMenu.add( TransitionMenuItem );
      TransitionMenuItem.addActionListener( TWBL );

      TransitionMenuBar.add( StateMenu );
      }

   private void setupTransitionPanelA ()
      {
      //Initialize and set up layout for panel
      GridLayout AGridLayout = new GridLayout(2,3);
      TransitionAPanel = new JPanel(AGridLayout);
      TitledBorder BorderA;
      BorderA = BorderFactory.createTitledBorder( DisplayTrans.TA.Description );
      TransitionAPanel.setBorder( BorderA );

      // Initialize the GUI components
      DisplayTrans.TA.StatusBox = new JCheckBox("Use");
      if (DisplayTrans.TA.UseParam1 == true)
         DisplayTrans.TA.Param1TextField = new JTextField(5);
      if (DisplayTrans.TA.UseParam2 == true)
         DisplayTrans.TA.Param2TextField = new JTextField(5); 
        
      // Add Checkbox to the panel
      TransitionAPanel.add( DisplayTrans.TA.StatusBox );
      
      // If parameter 1 is used by class, add this stuff
      if ( DisplayTrans.TA.UseParam1 )
         {
         TransitionAPanel.add( new JLabel ( DisplayTrans.TA.Param1Description ) );
         TransitionAPanel.add( DisplayTrans.TA.Param1TextField );
         }
      else
         {
         TransitionAPanel.add( new JLabel(" ") );
         TransitionAPanel.add( new JLabel(" ") );
         }
      
      // If parameter 2 is used by class, add this stuff
      if ( DisplayTrans.TA.UseParam2 )
         {
         TransitionAPanel.add( new JLabel(" ") );
         TransitionAPanel.add( new JLabel ( DisplayTrans.TA.Param2Description ) );
         TransitionAPanel.add( DisplayTrans.TA.Param2TextField );
         }
      else
         {
         TransitionAPanel.add( new JLabel(" ") );
         TransitionAPanel.add( new JLabel(" ") );
         TransitionAPanel.add( new JLabel(" ") );
         }
      }

   private void setupTransitionPanelB ()
      {
      //Initialize and set up layout for panel
      GridLayout BGridLayout = new GridLayout(2,3);
      TransitionBPanel = new JPanel(BGridLayout);
      TitledBorder BorderB;
      BorderB = BorderFactory.createTitledBorder( DisplayTrans.TB.Description );
      TransitionBPanel.setBorder( BorderB );

      // Initialize the GUI components
      DisplayTrans.TB.StatusBox = new JCheckBox("Use");
      if (DisplayTrans.TB.UseParam1 == true)
         DisplayTrans.TB.Param1TextField = new JTextField(5);
      if (DisplayTrans.TB.UseParam2 == true)
         DisplayTrans.TB.Param2TextField = new JTextField(5); 
        
      // Add Checkbox to the panel
      TransitionBPanel.add( DisplayTrans.TB.StatusBox );
      
      // If parameter 1 is used by class, add this stuff
      if ( DisplayTrans.TB.UseParam1 )
         {
         TransitionBPanel.add( new JLabel ( DisplayTrans.TB.Param1Description ) );
         TransitionBPanel.add( DisplayTrans.TB.Param1TextField );
         }
      else
         {
         TransitionBPanel.add( new JLabel(" ") );
         TransitionBPanel.add( new JLabel(" ") );
         }
      
      // If parameter 2 is used by class, add this stuff
      if ( DisplayTrans.TB.UseParam2 )
         {
         TransitionBPanel.add( new JLabel(" ") );
         TransitionBPanel.add( new JLabel ( DisplayTrans.TB.Param2Description ) );
         TransitionBPanel.add( DisplayTrans.TB.Param2TextField );
         }
      else
         {
         TransitionBPanel.add( new JLabel(" ") );
         TransitionBPanel.add( new JLabel(" ") );
         TransitionBPanel.add( new JLabel(" ") );
         }
      }

   private void setupTransitionPanelC ()
      {
      //Initialize and set up layout for panel
      GridLayout CGridLayout = new GridLayout(2,3);
      TransitionCPanel = new JPanel(CGridLayout);
      TitledBorder BorderC;
      BorderC = BorderFactory.createTitledBorder( DisplayTrans.TC.Description );
      TransitionCPanel.setBorder( BorderC );

      // Initialize the GUI components
      DisplayTrans.TC.StatusBox = new JCheckBox("Use");
      if (DisplayTrans.TC.UseParam1 == true)
         DisplayTrans.TC.Param1TextField = new JTextField(5);
      if (DisplayTrans.TC.UseParam2 == true)
         DisplayTrans.TC.Param2TextField = new JTextField(5); 
        
      // Add Checkbox to the panel
      TransitionCPanel.add( DisplayTrans.TC.StatusBox );
      
      // If parameter 1 is used by class, add this stuff
      if ( DisplayTrans.TC.UseParam1 )
         {
         TransitionCPanel.add( new JLabel ( DisplayTrans.TC.Param1Description ) );
         TransitionCPanel.add( DisplayTrans.TC.Param1TextField );
         }
      else
         {
         TransitionCPanel.add( new JLabel(" ") );
         TransitionCPanel.add( new JLabel(" ") );
         }
      
      // If parameter 2 is used by class, add this stuff
      if ( DisplayTrans.TC.UseParam2 )
         {
         TransitionCPanel.add( new JLabel(" ") );
         TransitionCPanel.add( new JLabel ( DisplayTrans.TC.Param2Description ) );
         TransitionCPanel.add( DisplayTrans.TC.Param2TextField );
         }
      else
         {
         TransitionCPanel.add( new JLabel(" ") );
         TransitionCPanel.add( new JLabel(" ") );
         TransitionCPanel.add( new JLabel(" ") );
         }
      }
   
   private void setupTransitionPanelD ()
      {
      //Initialize and set up layout for panel
      GridLayout DGridLayout = new GridLayout(2,3);
      TransitionDPanel = new JPanel(DGridLayout);
      TitledBorder BorderD;
      BorderD = BorderFactory.createTitledBorder( DisplayTrans.TD.Description );
      TransitionDPanel.setBorder( BorderD );

      // Initialize the GUI components
      DisplayTrans.TD.StatusBox = new JCheckBox("Use");
      if (DisplayTrans.TD.UseParam1 == true)
         DisplayTrans.TD.Param1TextField = new JTextField(5);
      if (DisplayTrans.TD.UseParam2 == true)
         DisplayTrans.TD.Param2TextField = new JTextField(5); 
        
      // Add Checkbox to the panel
      TransitionDPanel.add( DisplayTrans.TD.StatusBox );
      
      // If parameter 1 is used by class, add this stuff
      if ( DisplayTrans.TD.UseParam1 )
         {
         TransitionDPanel.add( new JLabel ( DisplayTrans.TD.Param1Description ) );
         TransitionDPanel.add( DisplayTrans.TD.Param1TextField );
         }
      else
         {
         TransitionDPanel.add( new JLabel(" ") );
         TransitionDPanel.add( new JLabel(" ") );
         }
      
      // If parameter 2 is used by class, add this stuff
      if ( DisplayTrans.TD.UseParam2 )
         {
         TransitionDPanel.add( new JLabel(" ") );
         TransitionDPanel.add( new JLabel ( DisplayTrans.TD.Param2Description ) );
         TransitionDPanel.add( DisplayTrans.TD.Param2TextField );
         }
      else
         {
         TransitionDPanel.add( new JLabel(" ") );
         TransitionDPanel.add( new JLabel(" ") );
         TransitionDPanel.add( new JLabel(" ") );
         }
      }

   private void setupTransitionPanelE ()
      {
      //Initialize and set up layout for panel
      GridLayout EGridLayout = new GridLayout(2,3);
      TransitionEPanel = new JPanel(EGridLayout);
      TitledBorder BorderE;
      BorderE = BorderFactory.createTitledBorder( DisplayTrans.TE.Description );
      TransitionEPanel.setBorder( BorderE );

      // Initialize the GUI components
      DisplayTrans.TE.StatusBox = new JCheckBox("Use");
      if (DisplayTrans.TE.UseParam1 == true)
         DisplayTrans.TE.Param1TextField = new JTextField(5);
      if (DisplayTrans.TE.UseParam2 == true)
         DisplayTrans.TE.Param2TextField = new JTextField(5); 
        
      // Add Checkbox to the panel
      TransitionEPanel.add( DisplayTrans.TE.StatusBox );
      
      // If parameter 1 is used by class, add this stuff
      if ( DisplayTrans.TE.UseParam1 )
         {
         TransitionEPanel.add( new JLabel ( DisplayTrans.TE.Param1Description ) );
         TransitionEPanel.add( DisplayTrans.TE.Param1TextField );
         }
      else
         {
         TransitionEPanel.add( new JLabel(" ") );
         TransitionEPanel.add( new JLabel(" ") );
         }
      
      // If parameter 2 is used by class, add this stuff
      if ( DisplayTrans.TE.UseParam2 )
         {
         TransitionEPanel.add( new JLabel(" ") );
         TransitionEPanel.add( new JLabel ( DisplayTrans.TE.Param2Description ) );
         TransitionEPanel.add( DisplayTrans.TE.Param2TextField );
         }
      else
         {
         TransitionEPanel.add( new JLabel(" ") );
         TransitionEPanel.add( new JLabel(" ") );
         TransitionEPanel.add( new JLabel(" ") );
         }
      }
                
      public void setupButtons()
         {
         OKButton = new JButton("OK");
         OKButton.setActionCommand("OKTW");
         CancelButton = new JButton("Cancel");
         CancelButton.setActionCommand("CancelTW");
         }

      public TransitionWindow ( StateList x)
         {
         OldStateList = x;
         SimulationStates = new StateList(OldStateList);
         

         DisplayTrans = new TransitionHolder();

         //Setup each sub panel
         setupTransitionPanelA();
         setupTransitionPanelB();
         setupTransitionPanelC();
         setupTransitionPanelD();
         setupTransitionPanelE();
         setupPrecedencePanel();

         CheckState = new boolean[5];

         setupButtons();

         TransitionsFrame = new JFrame("Transition Modifications");
         GBLayout = new GridBagLayout();
         GBConstraints = new GridBagConstraints();
         ContentPane = new JPanel(GBLayout);

         //Add the 1st transition panel
         GBConstraints.gridx = 3;
         GBConstraints.gridy = 1;
         GBConstraints.gridwidth = 1;
         GBConstraints.gridheight = 1;
         GBConstraints.fill = GridBagConstraints.HORIZONTAL;
         GBLayout.setConstraints( TransitionAPanel , GBConstraints );
         ContentPane.add(TransitionAPanel);

         //Add the 2nd transition panel
         GBConstraints.gridx = 3;
         GBConstraints.gridy = 2;
         GBLayout.setConstraints( TransitionBPanel , GBConstraints );
         ContentPane.add(TransitionBPanel);

         //Add the 3rd transition panel
         GBConstraints.gridx = 3;
         GBConstraints.gridy = 3;
         GBLayout.setConstraints( TransitionCPanel , GBConstraints );
         ContentPane.add(TransitionCPanel);

         //Add the 4th transition panel
         GBConstraints.gridx = 3;
         GBConstraints.gridy = 4;
         GBLayout.setConstraints( TransitionDPanel , GBConstraints );
         ContentPane.add(TransitionDPanel);

         //Add the 5th transition panel
         GBConstraints.gridx = 3;
         GBConstraints.gridy = 5;
         GBLayout.setConstraints( TransitionEPanel , GBConstraints );
         ContentPane.add(TransitionEPanel);

         //Add OK Button
         GBConstraints.gridx = 1;
         GBConstraints.gridy = 5;
         GBLayout.setConstraints( OKButton, GBConstraints );
         ContentPane.add( OKButton );

         //Add Cancel Button
         GBConstraints.gridx = 2;
         GBConstraints.gridy = 5;
         GBLayout.setConstraints( CancelButton, GBConstraints );
         ContentPane.add( CancelButton);

         //Add Precedence Panel
         GBConstraints.gridx = 1;
         GBConstraints.gridy = 1;
         GBConstraints.gridwidth = 2;
         GBConstraints.gridheight = 4;
         GBLayout.setConstraints( PrecedencePanel, GBConstraints );
         ContentPane.add( PrecedencePanel );

         //Adding Listeners
         TransitionsFrame.addWindowListener( new WindowAdapter()
            {
               public void windowClosing(WindowEvent e)
                  {
                  CSim.TransitionWindowFinished( OldStateList );
                  }
            } );

         TWBL = new TWButtonListener ( this );
         OKButton.addActionListener( TWBL );
         CancelButton.addActionListener( TWBL );
         UpButton.addActionListener( TWBL );
         DownButton.addActionListener( TWBL );

         TWCBListener CBL = new TWCBListener( this );
         DisplayTrans.TA.StatusBox.addItemListener( CBL );
         DisplayTrans.TB.StatusBox.addItemListener( CBL );
         DisplayTrans.TC.StatusBox.addItemListener( CBL );
         DisplayTrans.TD.StatusBox.addItemListener( CBL );
         DisplayTrans.TE.StatusBox.addItemListener( CBL );

         // Add the list listener
         TWListListener TWLL = new TWListListener( this );
         ListSelectionModel LSM = PrecedenceList.getSelectionModel();
         LSM.addListSelectionListener ( (ListSelectionListener) TWLL );
         
         //Add Menu Bar
         setupMenuBar();
         TransitionsFrame.setJMenuBar( TransitionMenuBar );
         }

      public void showGUI()
         {
         //Pack and show the GUI
         ContentPane.setOpaque(true);
         TransitionsFrame.setContentPane( ContentPane );
         TransitionsFrame.pack();
         TransitionsFrame.setVisible(true);
              
         }

      public void update()
         {
         ContentPane.updateUI();
         TransitionMenuBar.updateUI();
         }

   };
