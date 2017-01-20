//SizeHolder.java
//Jeffrey Miller
//November 22, 2000
//Version 1.0

//This object will hold the size of the map, in two strings.
//Call the getXValue() and getYValue() methods to return the
//size of the grid

class SizeHolder
{
   String XSize;
   String YSize;
   Integer XInt;
   Integer YInt;
   int X;
   int Y;

   SizeHolder()
   {                       
   };

   public int getXSize()
   {
      X = 20*(XInt.parseInt(XSize));
      return(X);                   
   };

   public int getYSize()
   {
      Y = 20*(YInt.parseInt(YSize));
      return(Y);
   };
   public void setSize(String x, String y)
   {
      XSize = x;
      YSize = y;
   };
}
