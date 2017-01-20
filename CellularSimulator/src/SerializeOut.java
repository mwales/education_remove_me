//November 11,2000
//Version 0.5
//Jeff Miller

//This file is to test if an object can be serialized to a file.

import java.io.*;
//import ImageMap;

class SerializeOut
{
      FileOutputStream ostream;
      ObjectOutputStream ObjectOut;
      PrintWriter Printer;

      SerializeOut()
      {
      };


      void openStream(String FileName)
      {
         try
         {
            ostream= new FileOutputStream(FileName);
            ObjectOut = new ObjectOutputStream(ostream);
         }
         catch(IOException e)
         {
            System.out.println(""+e.getMessage());
         }
      };
      void writeObject(ImageMap Map)
      {
         try
         {
            ObjectOut.writeObject(Map);
         }
         catch(IOException e)
         {
            System.out.println(""+e.getMessage());
         }
       };
      void closeStream()
      {
         try
         {
            ostream.close();
         }
         catch(IOException e)
         {
            System.out.println(""+e.getMessage());
         };
      };
}



            
