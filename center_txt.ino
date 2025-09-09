
void center_txt (const char* str[], int count, int txt_size){
  int th = 8 * txt_size;
  int block_h = th * count;
  int y_start = (display.height() - block_h) / 2; // empty space divided by 2 is a center

  display.clearDisplay();
  display.setTextSize(txt_size);
  display.setTextColor(SSD1306_WHITE);

  // center x 
  for (int i = 0; i < count; i++){
    int tw = 6 * strlen(str[i]) * txt_size;
    int x = (display.width() - tw) / 2; // empty space divided by 2 is a center
    int y = y_start + i * th;
    
    display.setCursor(x,y);
    display.print(str[i]);
  }
  display.display();
}