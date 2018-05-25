
# include <MaxMatrix.h>

const int DIN = 7;
const int CLK = 6;
const int CS = 5;
const int maxInUse = 1;


MaxMatrix m (DIN, CS, CLK, maxInUse);

void setup() {
  // put your setup code here, to run once:
  m.init();
  m.setIntensity(8);

}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 9; i++)
  {
    set_led(i);
    delay(1500);
  }

}

void set_led(int direction)
{
  m.clear();
  switch (direction)
  {
    //Straight forward
    case 1: m.setDot(0, 4, true); m.setDot(0,5,true); break;
    //little to the right kinda like North East
    case 2: m.setDot(2, 2, true); m.setDot(1, 3, true); break;
    //right like East
    case 3: m.setDot(3, 2, true); m.setDot(4, 2, true); break;
    case 4: m.setDot(6, 2, true); m.setDot(5,3,true);  break;
    //turn around behind you
    case 5: m.setDot(7, 4, true); m.setDot(7,5,true); break;
    default:
      break;
  }

}
