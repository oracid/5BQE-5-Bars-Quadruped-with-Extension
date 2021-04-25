// 5BQE-1 - 5 Bars Quadruped with Extension - 22/04/2021
void(* resetFunc) (void) = 0;             // soft reset
#include <Servo.h>
int Speed=1500;                           // Speed determines the speed
const int nb=8;                           // Number of servos
Servo Srv[nb];                            // Servos table
int OnOff[nb]={1,1,1,1,1,1,1,1};          // on/off Servos table
int FRLS=0, FRRS=1, FLLS=2, FLRS=3, BRLS=4, BRRS=5, BLLS=6, BLRS=7;
//***************** errors correction for LS and RS servos position  ***************************************************
// servos      FRLS  FRRS  FLLS  FLRS  BRLS  BRRS  BLLS  BLRS     you must modify these values according to your servos
int Err[nb]={    6,   -1,    1,   -8,   -3,   -9,    9,    -1    };  
//**********************************************************************************************************************

// 25/04/2021 - asymetric gaits values in millimeter for one cycle
int FRx[]= { 50, 45, 40, 35, 30, 25, 20, 15, 10,  5,  0, -5,-10,-15,-20,-25,-30,-35,-40,-45,-50,-55,-60,-65,-70,    -70,-65,-60,-55,-50,-45,-40,-35,-30,-25,-20,-15,-10, -5,  0,  5, 10, 15, 20, 25, 30, 35, 40, 45, 50};
int FLx[]= { 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10,  5,  0, -5,-10,-15,-20,-25,-30,-35,-40,-45,-50,    -50,-45,-40,-35,-30,-25,-20,-15,-10, -5,  0,  5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70};
int BRx[]= {-70,-65,-60,-55,-50,-45,-40,-35,-30,-25,-20,-15,-10, -5,  0,  5, 10, 15, 20, 25, 30, 35, 40, 45, 50,     50, 45, 40, 35, 30, 25, 20, 15, 10,  5,  0, -5,-10,-15,-20,-25,-30,-35,-40,-45,-50,-55,-60,-65,-70};
int BLx[]= {-50,-45,-40,-35,-30,-25,-20,-15,-10, -5,  0,  5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70,     70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10,  5,  0, -5,-10,-15,-20,-25,-30,-35,-40,-45,-50};

int FRy[]= {  0, -5,-10,-15,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-15,-10, -5,  0,      0,  5, 10, 15, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 15, 10,  5,  0};
int FLy[]= {  0,  5, 10, 15, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 15, 10,  5,  0,      0, -5,-10,-15,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-15,-10, -5,  0};
int BRy[]= {  0,  5, 10, 15, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 15, 10,  5,  0,      0, -5,-10,-15,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-15,-10, -5,  0};
int BLy[]= {  0, -5,-10,-15,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-15,-10, -5,  0,      0,  5, 10, 15, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 15, 10,  5,  0};

int lgTab = (sizeof(FRx)/sizeof(int))-1;  // size of gaits tables -1
         
void  setup() {
  delay(300);                             // for reset consideration
  Serial.begin(9600);
  pinMode(0,INPUT_PULLUP);                // start/stop/reset button attachment

  int a=60;                               // Servos angle initialization
  if(OnOff[FRLS]){ Srv[FRLS].attach(2); Srv[FRLS].write(    a+Err[FRLS]); } // FR Front Right leg - LS Left   Servo
  if(OnOff[FRRS]){ Srv[FRRS].attach(3); Srv[FRRS].write(180-a+Err[FRRS]); } // FR Front Right leg - RS Right  Servo
  if(OnOff[FLLS]){ Srv[FLLS].attach(4); Srv[FLLS].write(    a+Err[FLLS]); } // FL Front Left  leg - LS Left   Servo
  if(OnOff[FLRS]){ Srv[FLRS].attach(5); Srv[FLRS].write(180-a+Err[FLRS]); } // FL Front Left  leg - RS Right  Servo
  if(OnOff[BRLS]){ Srv[BRLS].attach(6); Srv[BRLS].write(    a+Err[BRLS]); } // BR Back  Right leg - LS Left   Servo
  if(OnOff[BRRS]){ Srv[BRRS].attach(7); Srv[BRRS].write(180-a+Err[BRRS]); } // BR Back  Right leg - RS Right  Servo
  if(OnOff[BLLS]){ Srv[BLLS].attach(8); Srv[BLLS].write(    a+Err[BLLS]); } // BL Back  Left  leg - LS Left   Servo
  if(OnOff[BLRS]){ Srv[BLRS].attach(9); Srv[BLRS].write(180-a+Err[BLRS]); } // BL Back  Left  leg - RS Right  Servo

  Serial.print("\n\t To start, click on the Start button");  while( digitalRead(0) );  delay(400);  Serial.print("\n\t Started");
}

void loop() {
  for(int i=0;i<=9;i++)  Walk('F');  delay(100);    // Forward loop
  for(int i=0;i<=8;i++)  Walk('L');  delay(100);    // Turn left loop
}

void Walk(char d){              // d=F=Forward, d=B=Backward, d=L=Turn Left, d=R=Turn Right, 
  int i, j, l=1, r=1, savSpeed=Speed;
  switch (d) {
    case 'F': i=0;      j=lgTab;                    break;
    case 'B': i=-lgTab; j=0;                        break;
    case 'L': i=0;      j=lgTab;  l=5;  Speed=1000; break;
    case 'R': i=0;      j=lgTab;  r=5;  Speed=1000; break;
    default:                                        break;
  }
  for( i;i<=j;i++){
    if (! digitalRead(0)) resetFunc();
    InvKine(FRx[abs(i)]/r,FRy[abs(i)],FRLS,FRRS);
    InvKine(BLx[abs(i)]/l,BLy[abs(i)],BLLS,BLRS);
    InvKine(FLx[abs(i)]/l,FLy[abs(i)],FLLS,FLRS);
    InvKine(BRx[abs(i)]/r,BRy[abs(i)],BRLS,BRRS);
  }
  Speed=savSpeed;
}

void InvKine(int Px, int Py, int LS, int RS){
  const float A1x=0, A1y=160, A2x=0, A2y=160;             // Values of servos positions
  const float a1=112, c1=112, a2=112, c2=112;             // Values of leg sizes lengths

  float d=A1y-Py, e=Px;                                   // Calculation of inverse kinematics
  float b=sqrt((d*d)+(e*e));                              // Calculation of inverse kinematics
  float S=acos(d/b);  if(e<0)S=(-S);                      // Calculation of inverse kinematics
  float A12=acos(((b*b)+(c1*c1)-(a1*a1))/(2*b*c1));       // Calculation of inverse kinematics
  float A22=acos(((b*b)+(c2*c2)-(a2*a2))/(2*b*c2));       // Calculation of inverse kinematics
  float A11=(PI/2)-A12+S;                                 // Calculation of inverse kinematics
  float A21=(PI/2)-A22-S;                                 // Calculation of inverse kinematics
  int S1=round(A11*57.296);                               // left  servo angle in degrees
  int S2=round(180-(A21*57.296));                         // right servo angle in degrees

// DEBUG
//  Serial.print("\n\n\t Position to reach : Px=");Serial.print(Px);Serial.print("  Py=");Serial.print(Py);
//  Serial.print("\n\t d=");Serial.print(d);Serial.print("\t\t e=");Serial.print(e);
//  Serial.print("\t\t b=");Serial.print(b);Serial.print("\t\t S=");Serial.print(S*57.296);
//  Serial.print("\n\t A11=");Serial.print(A11*57.296);Serial.print("\t\t A12=");Serial.print(A12*57.296);
//  Serial.print("\t\t A22=");Serial.print(A22*57.296);Serial.print("\t\t A21=");Serial.print(A21*57.296);
//  Serial.print("\n\t Result of calculations, angles of the servos");
//  Serial.print("\n\t S1=");Serial.print(S1);Serial.print("°\t\t\t S2=");Serial.print(S2);Serial.print("°");

  if ( b>(a1+c1) ){
    Serial.print("\n\t Target point Px=");Serial.print(Px);Serial.print(" Py=");Serial.print(Py);Serial.print("\t b=");Serial.print(b);Serial.print(" > ");
    Serial.print(a1+c1);Serial.print(" is too long. Target impossible to reach   !!!!!");
    return;
  }
  if (S1+Err[LS]<0){
    Serial.print("\n\t Position to reach : Px=");Serial.print(Px);Serial.print("  Py=");Serial.print(Py);Serial.print("\t angle S1<0° is not reachable   !!!!!");
    return;
  }
  if (S2+Err[RS]>180){
    Serial.print("\n\t Position to reach : Px=");Serial.print(Px);Serial.print("  Py=");Serial.print(Py);Serial.print("\t angle S2<0° is not reachable   !!!!!");
    return;
  }
//  Serial.print("\t command executed ");
  if (OnOff[LS]) Srv[LS].write(S1+Err[LS]);   // set target Left  servo position if servo switch is On
  if (OnOff[RS]) Srv[RS].write(S2+Err[RS]);   // set target Right servo position if servo switch is On
  delayMicroseconds(Speed); 
}
