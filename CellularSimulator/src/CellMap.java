// CellMap.java
// Author:  Michael Wales
// Version: 1.0
// Date:  October 22, 2000

import java.io.*;
import java.util.*;

public class CellMap implements Serializable
   {
   private int Element[][];
   private int XDim = 0;
   private int YDim = 0;
   private int size;
   private Random NumberGenerator;

   public CellMap (ImageMap IMap, int s)
      {
      NumberGenerator = new Random();
      XDim = IMap.getXPix()/20;
      YDim = IMap.getYPix()/20;
      size = s;
      Element = new int[XDim][YDim];

      for( int yCount = 0; yCount < YDim; yCount++)
         {
         for( int xCount = 0; xCount < XDim; xCount++)
            {
            Element[xCount][yCount] = IMap.Clicks[xCount*20][yCount*20].getCurrentState();
            }
         }
      }

   public int getRandomNumber( int x)
      {
      return NumberGenerator.nextInt( x );
      }

   public int getXDim()
      {
      return XDim;
      }

   public int getYDim()
      {
      return YDim;
      }

   public int getStateNum( int XCell, int YCell)
      {
      return Element[XCell][YCell];
      }

   public void printMap()
      {
      for( int yCount = 0; yCount < YDim; yCount++)
         {
         for( int xCount = 0; xCount < XDim; xCount++)
            {
            System.out.print(Element[xCount][yCount]);
            }
         System.out.println("");
         }
      }

   public int getNumberOfStates()
      {
      return size;
      }
   };

