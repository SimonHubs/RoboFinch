/* ------------------------------------------------
* SERIAL COM - HANDELING MULTIPLE BYTES inside ARDUINO - 03_function development
* by beltran berrocal
*
* this prog establishes a connection with the pc and waits for it to send him
* a long string of characters like "hello Arduino!". 
* Then Arduino informs the pc that it heard the whole sentence
*
* the same as examlpe 02 but it deploys 2 reusable functions.
* for doing the same job. 
* readSerialString() and  printSerialString()
* the only problem is that they use global variables instead of getting them passed 
* as parameters. this means that in order to reuse this code you should also copy
* the 4 variables instantiated at the beginning of the code.
* Another problem is that if you expect more than one string at a time 
* you will have to duplicate and change names to all variables as well as the functions.
* Next version should have the possibility to pass the array as a parameter to the function.
*
* created 15 Decembre 2005;
* copyleft 2005 Progetto25zero1  <http://www.progetto25zero1.com>
*
* --------------------------------------------------- */

int serInString[9000];  // array that will hold the different bytes  100=100characters;
                        // -> you must state how long the array will be else it won't work.
int  serInIndx  = 0;    // index of serInString[] in which to insert the next incoming byte
boolean doOutput = false;
/*read a string from the serial and store it in an array
//you must supply the array variable and the index count
void readSerialString (char *strArray, int indx) {
    int sb;                               //declare local serial byte before anything else
    Serial.print("reading Serial String: ");   
    if(serialAvailable()) {     
       while (serialAvailable()){ 
          sb = serialRead();             
          strArray[indx] = sb;
          indx++;
          serialWrite(sb);
       }
    }  
    Serial.println();
}
*/

//read a string from the serial and store it in an array
//this func uses globally set variable so it's not so reusable
//I need to find the right syntax to be able to pass to the function 2 parameters:
// the stringArray and (eventually) the index count
void readSerialString () {
    
    //if(Serial.available()) { 
       //Serial.print("reading Serial String: ");     //optional confirmation
       while (Serial.available()){ 
          char sb = Serial.read(); 
          //Serial.print(sb);
          if(sb==','){
            serInIndx++;           
          }
          else if (sb=='\n'){
            doOutput = true;
          }
          else if (sb=='\r'){
            ;
          }
          else{
            serInString[serInIndx] *= 10;
            serInString[serInIndx] += (int)sb-48; 
            //Serial.print(    serInString[serInIndx]);       
          }
          
          //serialWrite(sb);                        //optional confirmation
       }
       //Serial.println();
    //}  
}

//print the string all in one time
//this func as well uses global variables
void printSerialString() {
   if( serInIndx > 0) {
      Serial.print("Arduino memorized that you said: ");     
      //loop through all bytes in the array and print them out
      for(int serOutIndx=0; serOutIndx < serInIndx; serOutIndx++) {
          Serial.print( serInString[serOutIndx] );    //print out the byte at the specified index
          //serInString[serOutIndx] = "";            //optional: flush out the content
      }        
      //reset all the functions to be able to fill the string back with content
      serInIndx  = 0;
      for (int idx=0; idx < 9000; idx++){
        serInString[idx] = 0;
      }
      Serial.println();
   }

}


void setup() {
  Serial.begin(115200);
  Serial.println("Hello World");
  for (int idx=0; idx < 9000; idx++){
    serInString[idx] = 0;
  }
}

void loop () {
  
  readSerialString();
  
  
   
  if (doOutput){
    for(int serOutIndx=1; serOutIndx <= serInIndx; serOutIndx=serOutIndx+2) {
      //int temp=serInString[serOutIndx];  
      //wavform=temp.toInt();
      Serial.println(serInString[serOutIndx+1]);          
      dacWrite(25,serInString[serOutIndx]);
      dacWrite(26,serInString[serOutIndx+1]);
      delay(serInString[0]);
    }
    doOutput = false;
    for (int idx=0; idx < 9000; idx++){
      serInString[idx] = 0;
    }
    serInIndx  = 0;
  }

}
