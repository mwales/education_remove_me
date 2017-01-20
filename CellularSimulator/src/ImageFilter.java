// Filename:  ImageFilter.java
// Author:  Michael Wales
// Version:  1.0
// Date:  November 10, 2000

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.File;
import javax.swing.filechooser.*;

public class ImageFilter extends FileFilter
   {

   public boolean accept ( File x )
      {
      if ( x.isDirectory() )
         return true;

      String Filename = x.getName();
      Filename.toLowerCase();

      if ( ( Filename.endsWith(".jpeg") ) || ( Filename.endsWith(".jpg") ) )
         return true;
      if ( Filename.endsWith(".gif") )
         return true;
      if ( ( Filename.endsWith(".tiff") ) || ( Filename.endsWith(".tif") ) )
         return true;

      return false;
      }

   public String getDescription()
      {
      return("Image Files");
      }
   };





