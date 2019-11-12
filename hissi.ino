


#define ledi A0

//array for 7-segment display
int leds[7] = {13, 12, 11, 10, 9, 8, 7};

int btns[6] = {A5, A4, 2, 3, 4, 5};//array for buttons

bool stops[6] = {0, 0, 0, 0, 0, 0};//array for stop-flags

int cur_floor = 3;//number of current floor

int ending_stop = -1;//last stop for now

int direction = 0;// ylös tai alas

//---------define type for array of functions----------
void yksi(void);
void kaksi(void);
void kolme(void);
void nelja(void);
void viisi(void);
void kuusi(void);

typedef void (* FuncPtr) (); 

FuncPtr floors[6] {yksi, kaksi, kolme, nelja, viisi, kuusi};
//------------------------------------------------------


unsigned current = millis();//time-counter
unsigned next = current + 1000;


void setup()
{
  //-----define leds------
  for (int i = 0; i < 7; i++) {
    pinMode(leds[i], OUTPUT);  
  }
  //----------------------
  
  //-----define btns------
  for (int i = 0; i < 6; i++) {
    pinMode(btns[i], OUTPUT);
   digitalWrite(btns[i], HIGH);
  }
  //----------------------
  
  
  pinMode(ledi, OUTPUT);
  
  digitalWrite(ledi, HIGH);
    
  Serial.begin(115200);  
  
  //----------------------------------
  Serial.println("---------------------");
  Serial.print("hissi kerroksessa ");
  Serial.println(cur_floor + 1);
  //---------------------------------
}

void loop()
{  
  floors[cur_floor](); // show current floor
  
 
  for (int i = 0; i < 6; i++){
    
    //if btn was pushed
    if ((digitalRead(btns[i]) == LOW)&&(stops[i] == 0)){
      
      //--------------------------
      Serial.println("---------------------");
      Serial.print("hissi tilattu kerrokseen ");
      Serial.println(i + 1);
      //--------------------------
      
      //if elevator on the same floor then blinking
      if (cur_floor == i){
        Serial.println("---------------------");
        Serial.println("Hissin tilaus peruttu");
        blink();        
      } 
      
      //if pushed not current floor btn
      if (cur_floor != i) {
        //mark floor for stop
      	stops[i] = 1;
        //check is elevator in motion
        if (ending_stop == -1){
          //if no mark the stop like ending stop
          ending_stop = i;
                    
          current = millis();
          next = current + 1000;
        }
      }
    }
  }
  
  //if elevator in motion
  if (ending_stop != -1){
    
    current = millis();
    
    if (current > next){
      
      //if next stop is upper
      if (cur_floor < ending_stop){
        direction = 1; 
        goUp();
      }
      
      if (cur_floor > ending_stop){
        direction = -1;
        goDown();        
      }
      //if ending stop is this one
      if (cur_floor == ending_stop){
        ending_stop = -1;
        
        //change direction if last floor
        if (cur_floor == 5){
          direction = -1;
        }
        
        if (cur_floor == 0){
          direction = 1;
        }
        
        if (direction == 1){
          for (int i = cur_floor; i < 6; i++){
            if (stops[i]){
              ending_stop = i;
              break;
            }
          }
        }
        
        if (direction == -1){
          for (int i = cur_floor; i >= 0; i--){
            if (stops[i]){
              ending_stop = i;
              break;
            }
          }
        }
        
        for (int i = 0; i < 6; i++){
            if (stops[i]){
              ending_stop = i;
              break;
            }
        }
      }
    }
  }
}
 

void goUp()
{
  cur_floor++;
  Serial.println("---------------------");
  Serial.println("--- hissi matkalla ylos ---");
  
  //if there is stops on the way than stop
  if(stops[cur_floor]){
    floors[cur_floor]();
    openDoors();
    stops[cur_floor] = 0;
  }
    
  //if there is no stop, than pass the floor
  else{
    Serial.println("---------------------");
    Serial.print("hissi ohitti kerroksen ");
    Serial.println(cur_floor + 1);
  }
  
  current = millis();
  next = current + 1000;
}

void goDown()
{
  cur_floor--;
  Serial.println("---------------------");
  Serial.println("--- hissi matkalla alas ---");
  
  //if there is stops on the way than stop
  if (stops[cur_floor]){
    floors[cur_floor]();
    openDoors();
    stops[cur_floor] = 0;
  }
  
  //if there is no stop, than pass the floor
  else{
    Serial.println("---------------------");
    Serial.print("hissi ohitti kerroksen ");
    Serial.println(cur_floor + 1);
  }
  
  current = millis();
  next = current + 1000;
}


void openDoors()
{
  Serial.println("---------------------");
  Serial.print("hissi kerroksessa ");
  Serial.println(cur_floor + 1);
  digitalWrite(ledi, LOW);
  Serial.println("---------------------");
  Serial.println("--- ovi auki ---");
  delay(1000);
  Serial.println("---------------------");
  Serial.println("--- ovi kiinni ---");
  digitalWrite(ledi, HIGH);
}


// STATES OF DISPLAY

void blink()
{
  for (int k = 0; k < 3; k++){
    sammu();	
    delay(300);
  	floors[cur_floor]();
    delay(300);
  }
}

void sammu()
{
  digitalWrite(13, HIGH);  
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(7, HIGH);
}

void yksi()
{
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(7, HIGH);
}
  
void kaksi()
{
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
}
  
void kolme()
{
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
}
  
void nelja()
{
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
}

void viisi()
{
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
}
  
void kuusi()
{
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
}
