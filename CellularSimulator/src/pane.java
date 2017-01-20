//pane.java
//Jeffrey Miller
//October 29, 2000
//Version 0.5

/* This class is the main panel of the Simulation window
   It setups up the ImagaMap class (the class that holds all the info
   about the states) and it setups up the StatesPanel (which controls the
   running of the simulation).

   This class also handles the adding of new cells through its MouseListener
   (It calls the methods contained in the class ImageMap to add either
    group of cells or a single cell, depending on the MouseEvent detected)
*/

import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
//import ImageMap;
//import Scroller;
//import StateList;
//import StatesPanel;

class pane extends JPanel
{
    StateList SL;
    private Scroller ScrollingPane;
    private ImageMap Map;
    private Scroller ScrPane;
    StatesPanel Buttons;
    String Run;
    int CurrentState;

//constructor called by class window if there is a map loaded
    pane(StateList SList, ImageMap LoadedMap, int x, int y)
    {
        super();

        //sets up grid layout
        GridBagLayout grid = new GridBagLayout();
        GridBagConstraints CJ = new GridBagConstraints();
      

        SL = SList;
        Map = LoadedMap;
        Map.setStateList(SL);
        ScrPane = new Scroller(x, y , Map);
        Buttons = new StatesPanel(SL, this);

        //setups up size of the panels
        CJ.weightx = 1.0;
        grid.setConstraints(ScrPane,CJ);
        CJ.gridwidth = GridBagConstraints.REMAINDER;
        grid.setConstraints(Buttons,CJ);
        setLayout(grid);

        add(ScrPane);
        add(Buttons);
    };
    
//this construtor setups up the panel when a new simulation is desired
    pane(StateList SList, int x, int y)
    {
        super();


        //sets up grid layout
        GridBagLayout grid = new GridBagLayout();
        GridBagConstraints CJ = new GridBagConstraints();
      

        SL = SList;
       


        Buttons = new StatesPanel(SL,this);
        Map = new ImageMap(SL,x,y);
        ScrPane = new Scroller(x,y,Map);
        CJ.weightx = 1.0;
        grid.setConstraints(ScrPane,CJ);
        CJ.gridwidth = GridBagConstraints.REMAINDER;
        grid.setConstraints(Buttons,CJ);
        add(ScrPane);
        add(Buttons);
    };

    public ImageMap getImageMap()
    {
      return(Map);
    };

    void setSelectedState(int select)
    {
      Map.setSelectedState(select);
    }


    //starts the simulation, called by the StatesPanel
    public void run()
    {
      Map.start();
    }

    //stops the simulation, called by the StatesPanel
    public void stop()
    {
      Map.stop();
    }
}

