// Practical 3 Action-Functions
//
// DO NOT MODIFY


// Loads 6 into gJ
inline action_11() {
  unsigned loc1 : 7 ;
  loc1 = 6;
  printf("Action 11 loaded %d\n",loc1);
  gJ = loc1;
}

// Loads 2 into gH
inline action_12() {
  unsigned loc1 : 7 ;
  loc1 = 2;
  printf("Action 12 loaded %d\n",loc1);
  gH = loc1;
}

// Modifies gU + 1
// Modifies gI * 2
inline action_13() {
  unsigned loc1 : 7 ;
  loc1 = 1;
  printf("Action 13 loaded %d\n",loc1);
  gU = (gU + loc1);
  //puff();
  unsigned loc2 : 7 ;
  loc2 = 2;
  printf("Action 13 loaded %d\n",loc2);
  gI = (gI * loc2);
}

// Increment gP by 4
inline action_14() {
  unsigned loc1 : 7 ;
  loc1 = 4;
  printf("Action 14 loaded %d\n",loc1);
  gP = (gP + loc1);
}

// Loads 2 into gL
inline action_15() {
  unsigned loc1 : 7 ;
  loc1 = 2;
  printf("Action 15 loaded %d\n",loc1);
  gL = loc1;
}

// Decrement gL by 4
inline action_16() {
  unsigned loc1 : 7 ;
  loc1 = 4;
  printf("Action 16 loaded %d\n",loc1);
  gL = (gL - loc1);
}

// Modifies gI + 3
// Modifies gM * 4
inline action_21() {
  unsigned loc1 : 7 ;
  loc1 = 3;
  printf("Action 21 loaded %d\n",loc1);
  gI = (gI + loc1);
  //puff();
  unsigned loc2 : 7 ;
  loc2 = 4;
  printf("Action 21 loaded %d\n",loc2);
  gM = (gM * loc2);
}

// Loads 5 into gP
inline action_22() {
  unsigned loc1 : 7 ;
  loc1 = 5;
  printf("Action 22 loaded %d\n",loc1);
  gP = loc1;
}

// Loads 8 into gN
inline action_23() {
  unsigned loc1 : 7 ;
  loc1 = 8;
  printf("Action 23 loaded %d\n",loc1);
  gN = loc1;
}

// Increment gJ by 10
inline action_24() {
  unsigned loc1 : 7 ;
  loc1 = 10;
  printf("Action 24 loaded %d\n",loc1);
  gJ = (gJ + loc1);
}

// Loads 4 into gL
inline action_25() {
  unsigned loc1 : 7 ;
  loc1 = 4;
  printf("Action 25 loaded %d\n",loc1);
  gL = loc1;
}

// Increment gL by 4
inline action_26() {
  unsigned loc1 : 7 ;
  loc1 = 4;
  printf("Action 26 loaded %d\n",loc1);
  gL = (gL + loc1);
}

// Increment gM by 6
inline action_31() {
  unsigned loc1 : 7 ;
  loc1 = 6;
  printf("Action 31 loaded %d\n",loc1);
  gM = (gM + loc1);
}

// Loads 4 into gI
inline action_32() {
  unsigned loc1 : 7 ;
  loc1 = 4;
  printf("Action 32 loaded %d\n",loc1);
  gI = loc1;
}

// Loads 6 into gP
inline action_33() {
  unsigned loc1 : 7 ;
  loc1 = 6;
  printf("Action 33 loaded %d\n",loc1);
  gP = loc1;
}

// Modifies gP * 2
// Modifies gI + 10
inline action_34() {
  unsigned loc1 : 7 ;
  loc1 = 2;
  printf("Action 34 loaded %d\n",loc1);
  gP = (gU * loc1);
  //puff();
  unsigned loc2 : 7 ;
  loc2 = 10;
  printf("Action 34 loaded %d\n",loc2);
  gI = (gM + loc2);
}

// Increment gG by 5
inline action_35() {
  unsigned loc1 : 7 ;
  loc1 = 5;
  printf("Action 35 loaded %d\n",loc1);
  gG = (gG + loc1);
}

// Loads 3 into gJ
inline action_41() {
  unsigned loc1 : 7 ;
  loc1 = 3;
  printf("Action 41 loaded %d\n",loc1);
  gJ = loc1;
}

// Increment gI by 8
inline action_42() {
  unsigned loc1 : 7 ;
  loc1 = 8;
  printf("Action 42 loaded %d\n",loc1);
  gI = (gI + loc1);
}

// Set gG to either 1 or 6
inline action_43() {
  unsigned loc1 : 7 ;
  if
  ::  loc1 = 1;
  ::  loc1 = 6;
  fi

  printf("Action 43 chose (n.d.) %d\n",loc1);
  gG = loc1;
}

// Modifies gM + 5
// Modifies gQ * 6
inline action_44() {
  unsigned loc1 : 7 ;
  loc1 = 5;
  printf("Action 44 loaded %d\n",loc1);
  gM = (gM + loc1);
  //puff();
  unsigned loc2 : 7 ;
  loc2 = 6;
  printf("Action 44 loaded %d\n",loc2);
  gQ = (gQ * loc2);
}

// Loads 1 into gU
inline action_45() {
  unsigned loc1 : 7 ;
  loc1 = 1;
  printf("Action 45 loaded %d\n",loc1);
  gU = loc1;
}

// Increment gQ by 10
inline action_51() {
  unsigned loc1 : 7 ;
  loc1 = 10;
  printf("Action 51 loaded %d\n",loc1);
  gQ = (gQ + loc1);
}

// Loads 3 into gI
inline action_52() {
  unsigned loc1 : 7 ;
  loc1 = 3;
  printf("Action 52 loaded %d\n",loc1);
  gI = loc1;
}

// Loads 5 into gU
inline action_53() {
  unsigned loc1 : 7 ;
  loc1 = 5;
  printf("Action 53 loaded %d\n",loc1);
  gU = loc1;
}

// Increment gM by 6
inline action_54() {
  unsigned loc1 : 7 ;
  loc1 = 6;
  printf("Action 54 loaded %d\n",loc1);
  gM = (gM + loc1);
}

// Decrement gG by 5
inline action_55() {
  unsigned loc1 : 7 ;
  loc1 = 5;
  printf("Action 55 loaded %d\n",loc1);
  gG = (gG - loc1);
}


