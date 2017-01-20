// Filename:  TransitionHolder.java
// Author:  Michael Wales
// Version:  1.0
// Date:  November 8, 2000

import java.io.*;

public class TransitionHolder implements Serializable
   {
   public TransitionA TA;
   public TransitionB TB;
   public TransitionC TC;
   public TransitionD TD;
   public TransitionE TE;

   public TransitionHolder()
      {
      TA = new TransitionA();
      TB = new TransitionB();
      TC = new TransitionC();
      TD = new TransitionD();
      TE = new TransitionE();
      }

   public TransitionHolder ( TransitionA T1, TransitionB T2, TransitionC T3,
         TransitionD T4, TransitionE T5)
      {
      TA = T1;
      TB = T2;
      TC = T3;
      TD = T4;
      TE = T5;
      }

   }
