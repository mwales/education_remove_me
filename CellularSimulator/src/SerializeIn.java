//November 11, 2000
//Version 0.5
//Jeff Miller

//This is the serial object reader

import java.io.*;
//import ImageMap;

class SerializeIn
{
   FileInputStream istream;
   ObjectInputStream objectreader;

   SerializeIn()
   {
   };

   void openStream(String FileName)
   {
      try
      {
         istream = new FileInputStream(FileName);
         objectreader = new ObjectInputStream(istream);
      }
      catch(IOException e)
      {
         System.out.println(""+e.getMessage());
      }
   }
   public ImageMap readObject()
   {
      ImageMap tempobj;
      int flag=0;
      try
      {
         tempobj = (ImageMap) objectreader.readObject();
         return(tempobj);
      }
      catch(Exception e)
      {
         System.out.println(""+e.getMessage());
      }
      tempobj =new ImageMap();
      return(tempobj);

   }
   void closeStream()
   {
      try
      {
         istream.close();
      }
      catch(IOException e)
      {
         System.out.println(""+e.getMessage());
      }

   };
}
