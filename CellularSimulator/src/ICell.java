//ICell.java
//Jeffrey Miller
//November 23, 2000
//Version 1.0

/* This class contains all the information about the current
   state of an indivdual cell.
   The CurrentState variable represents the state which the cell
   is currently in (coresponds to the value in the StateList class).
   The Selected boolean value is true when the cell is selected, and
   false when the cell is clear.
*/

   
import java.io.*;

class ICell implements Serializable
{
   int CurrentState;
   

   ICell(int StateNum)
   {
      CurrentState = StateNum;
   };
   public int getCurrentState()
   {
      return(CurrentState);
   };

   public void setCurrentState(int x)
      {
      CurrentState = x;
      }

}


