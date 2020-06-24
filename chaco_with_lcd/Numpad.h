void TFTWrite(int x, int y, String text) {
  myGLCD.setCursor(x, y);
  myGLCD.println(text);
}
void TFTKeypad(int x, int y, int width, int height){
    myGLCD.setColor(ORANGE);
    myGLCD.fillRect(0, 0, 240, 360);
    myGLCD.setColor(LIME);
    
    myGLCD.fillRect(1, 1, (width/3)-1, (height/4) -1 );
    myGLCD.fillRect(width/3+1, 1, (2*width/3)-1, (height/4)-1);
    myGLCD.fillRect((2*width/3)+1, 1, width-1, (height/4)-1);
    
    myGLCD.fillRect(1, (height/4) +1, (width/3)-1, (height/2) -1 );
    myGLCD.fillRect(width/3+1, (height/4) +1, (2*width/3)-1, (height/2)-1);
    myGLCD.fillRect((2*width/3)+1, (height/4) +1, width-1, (height/2)-1);
    
    myGLCD.fillRect(1, (height/2) +1, (width/3)-1, (3*height/4) -1 );
    myGLCD.fillRect(width/3+1, (height/2) +1, (2*width/3)-1, (3*height/4)-1);
    myGLCD.fillRect((2*width/3)+1, (height/2) +1, width-1, (3*height/4)-1);

    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0, (3*height/4)+1, 240, 360);
    myGLCD.print("1", 20, 20);
    myGLCD.print("2", (width/3)+20, 20);
    myGLCD.print("3", (2*width/3)+20, 20);
    
    myGLCD.print("4", 20, (height/4) +20);
    myGLCD.print("5", (width/3)+20, (height/4) +20);
    myGLCD.print("6", (2*width/3)+20, (height/4) +20);
    
    myGLCD.print("7", 20, (height/2) +20);
    myGLCD.print("8", (width/3)+20, (height/2) +20);
    myGLCD.print("9", (2*width/3)+20, (height/2) +20);
    myGLCD.setColor(LIME);
}
int GetNumpadValue(int x, int y, int width, int height){
  // Number of key is 3
  int keyWidth = width/3;
  int keyHeight = height/3;
  int keyx = x/keyWidth;
  int keyy = y/keyHeight;
  int num = (3*keyx)+keyy+1;
  if(num > 9)
    return 0;
    return num;
}
