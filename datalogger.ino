
/*
Layout:
 MOSI - pin 11
 MISO - pin 1
 CLK - pin 13
 CS - pin 4
*/
//Libraries 
#include <SPI.h>
#include <SD.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

//Data wire is plugged to pin 2 on the board
#define ONE_WIRE_BUS 2

//Instance of oneWire that communicates to with other oneWire devices
OneWire oneWire(ONE_WIRE_BUS);

//Pass oneWire reference to Dallas Temperature
DallasTemperature sensors(&oneWire);

const int chipSelect = 8;
int count;

String file_name;
String current_file;
String header;

void setup() {
  /* Setup Serial Coms
     
     Open serial communications and wait for port to open:
  */
  Serial.begin(9600);
  while (!Serial) {
    ; // arbitrary wait for serial port
  }

  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
  
  count = countFiles(SD.open("/", FILE_READ));

  file_name = "dL";
  int newFileNum = count + 1;
  current_file = file_name + newFileNum + ".csv";
  header = "Time, T1, T2, T3";
  File dataFile = SD.open(current_file, FILE_WRITE);

  Serial.println(count);
 
  if (dataFile){
    dataFile.println(header);
    dataFile.close();
    }
  
  Serial.println(current_file);  
}

void loop(){
  /* Open and write
     Writes pressureString to the sdcard log.txt file, closes the file after every write.
  */  
  sensors.requestTemperatures();
  float t1 = sensors.getTempFByIndex(0);
  float t2 = sensors.getTempFByIndex(1);
  float t3 = sensors.getTempFByIndex(2);
  delay(1000);
  Serial.print("\nT1: ");
  Serial.println(sensors.getTempFByIndex(0));
//  Serial.println(" ");
  Serial.print("T2: ");
  Serial.println(sensors.getTempFByIndex(1));

  
  // Creates new file in absence of existing files
  //current_file is passed to SD.open and gets made into a new file

  
  File dataFile = SD.open(current_file, FILE_WRITE);
 
  if (dataFile){
    dataFile.print(millis());
    dataFile.print(',');
    dataFile.print(t1, 4);
    dataFile.print(',');
    dataFile.print(t2, 4);
    dataFile.print(',');
    dataFile.println(t3, 4);
    dataFile.close();
  }
  
  else {
    Serial.println("error opening datafile");
  }
}

int countFiles(File f){
  int file_count = 0;
  while (true) {
    // f.openNextFile() is a method of File types from the SD card library
    File entry =  f.openNextFile();
    if (! entry) {
      return(file_count);
    }
    //File class has a method 'name()' that returns the name of one file
    Serial.print("file name = ");
    Serial.println(entry.name());
    if(!entry.isDirectory()) {
      file_count++;
    }
    else {
      Serial.println("Directory!");
    }
    entry.close();
    }
}
