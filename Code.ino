
/*Welcome!!
 * This is project is titled "Automated LPG Cylinder Booking".
 * Coded in Arduino IDE.
 * Libraries used => HX711.h and SoftwareSerial.h
 * Uses AT commands to run Serial communicaton between SIM800L and NANO
 * Update Serial is a function to check and clear buffer.
 */

#include <HX711.h>
#include <SoftwareSerial.h>

//Defining pinouts
#define DOUT  10
#define CLK  9
HX711 scale;
SoftwareSerial mySerial(5, 6); 

float calibration_factor =104285.00; // Calibration Value set everytime
float weight; // value being read
float gross = 0.1; //Empty cylinder weight 
float threshold = 2.9; //Minimum weight limit  

int LedRed = 7; //Booked light
int LedGreen = 8; //empty light

void setup() 
{
  Serial.begin(9600);
  mySerial.begin(9600);
  
  
  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare();
  Serial.println("Place:");
  delay(5000);
  
}



void loop() //checks and sends SMS to both user and BOOKING
{
 loadRead();
 delay (1500);

 if(weight<= gross)
    {
      SenduserAlert();
      exit(0);
    }
  if(weight <= threshold)
    {
      SenduserReq();
      exit(0);
    }
  
 }


void loadRead() //read sensor value
{
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  Serial.println(scale.get_units()); //Prints the reading
  weight = scale.get_units(); //Prints the reading
}


void SenduserReq() //Asking User Send Function
{
  Serial.println("SenduserReq"); 
  mySerial.println("AT"); //test module command
  updateSerial();
  mySerial.println("AT+CMGF=1"); //changes from PDU to TEXT
  updateSerial();
  mySerial.println("AT+CMGS=\"+919380000844\""); //User number
  updateSerial();
  mySerial.print("Your Gas Cylinder is near emptying."); //text content
  updateSerial();
  mySerial.write(26); //end message
}



void SenduserAlert() //Change cylinder alert
{
  Serial.println("SenduserAlert"); 
  mySerial.println("AT"); //test module command
  updateSerial();
  mySerial.println("AT+CMGF=1"); //changes from PDU to TEXT
  updateSerial();
  mySerial.println("AT+CMGS=\"+910123456789\""); //User number
  updateSerial();
  mySerial.print("Gas is empty!! Change Cylinders!! "); //text content
  updateSerial();
  mySerial.write(26); //end message
}



void SendagencyBook() //Booking message send function
{
  Serial.println("Booking:");
  mySerial.println("AT"); //test module command
  updateSerial();
  mySerial.println("AT+CMGF=1"); //changes from PDU to TEXT
  updateSerial();
  mySerial.println("AT+CMGS=\"+919876543210\""); //Agency number
  updateSerial();
  mySerial.print("HP AddressCode ConsumerNo"); //text content
  updateSerial();
  mySerial.write(26); //end message
}



void ReadagencyReturn() //Booking Verified message
{
  mySerial.println("AT"); //test module command
  updateSerial();
  mySerial.println("AT+CMGF=1");  //changes from PDU to TEXT
  updateSerial();
  mySerial.println("AT+CNMI=1,2,0,0,0"); //Sets Message order
}



void ReadUserReqReturn() //Read User request return
{
  mySerial.println("AT"); //test module command
  updateSerial();
  mySerial.println("AT+CMGF=1");  //changes from PDU to TEXT
  updateSerial();
  mySerial.println("AT+CNMI=1,2,0,0,0"); //Sets Message order
  Serial.println("Message read");
}



void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

