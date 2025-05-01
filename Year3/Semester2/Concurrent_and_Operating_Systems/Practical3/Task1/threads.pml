//Practical 3 Threads
//
// Tasks:
//
//  1. Modify thread code below *where necessary*
//     to ensure action-functions run atomically.
// 
//     Do not modify the order in which action-functions are called.
//     Do not add or modify action-function calls.
//
//  2. Modify the `assert` statements inside inline `endassertions`
//     to check for the correct final values of the global variables.


proctype Thread1() {
  printf( "Thread1 Running\n" );

  action_11();              // Loads 6 into gJ
  action_12();              // Loads 2 into gH
  atomic { action_13(); }   // Modifies gU + 1 and Modifies gI * 2
  atomic { action_14(); }   // Increment gP by 4
  action_15();              // Loads 2 into gL

  printf("Waiting for (gL > 3)\n");
  ((gL > 3)) ;

  atomic { action_16(); }   // Decrement gL by 4

}


proctype Thread2() {
  printf( "Thread2 Running\n" );

  atomic { action_21(); }  // Modifies gI + 3 and Modifies gM * 4
  action_22();             // Loads 5 into gP
  action_23();             // Loads 8 into gN
  atomic { action_24(); }  // Increment gJ by 10
  action_25();             // Loads 4 into gL

  printf("Waiting for (gL < 3)\n");
  ((gL < 3)) ;

  atomic { action_26(); }  // Increment gL by 4

}


proctype Thread3() {
  printf( "Thread3 Running\n" );

  atomic { action_31(); } // Increment gM by 6
  action_32();            // Loads 4 into gI
  action_33();            // Loads 6 into gP
  atomic { action_34(); } // Modifies gU * 2 and Modifies gM + 10
  
  printf("Waiting for (gG == 1)\n");
  ((gG == 1)) ;

  atomic { action_35(); } // Increment gG by 5

}


proctype Thread4() {
  printf( "Thread4 Running\n" );

  action_41();            // Loads 3 into gJ
  atomic { action_42(); } // Increment gI by 8
  action_43();            // Set gG to either 1 or 6
  atomic { action_44(); } // Modifies gM + 5 and Modifies gQ * 6
  action_45();            // Loads 1 into gU
  
}


proctype Thread5() {
  printf( "Thread5 Running\n" );

  atomic { action_51(); } // Increment gQ by 10
  action_52();            // Loads 3 into gI
  action_53();            // Loads 5 into gU
  atomic { action_54(); } // Increment gM by 6
  
  printf("Waiting for (gG == 6)\n");
  ((gG == 6)) ;

  atomic { action_55(); } // Decrement gG by 5

}


inline endassertions(){
  printf("End Assertions\n")
  assert(gL == 2 || gL == 4 || gL == 0 || gL == 6); //Action 15,16,25,26
  assert(gH == 2); //Action 12
  assert(gN == 8); //Action 23
  assert(gJ == 6 || gJ == 3 || gJ == 16 || gJ == 13); //Action 11,24,41
  assert(gP == 5 || gP == 6 || gP == 2 || gP == 10 || gP == 4 || gP == 12 || gP == 9 || gP == 14 || gP == 16); //Action 14,22,33,34
  assert(gU == 1 || gU == 5 || gU == 2 || gU == 6); //Action 13,45,53
  assert(gI == 4 || gI == 9|| gI == 27 || gI == 32 || gI == 34 || gI == 78 || gI == 45 || gI == 60 || gI == 63 || gI == 42 || gI == 3 || 
         gI == 8 || gI == 54 || gI == 57 || gI == 24 || gI == 156 || gI == 159|| gI == 90 || gI == 120 || gI == 126 || gI == 84 || gI == 6 ||
         gI == 7 || gI == 30 || gI == 81 || gI == 87 || gI == 48 || gI == 66 || gI == 12 || gI == 93 || gI == 35 || gI == 25 || gI == 136 ||
         gI == 86 || gI == 53 || gI == 68 || gI == 71 || gI == 50 || gI == 62 || gI == 11 || gI == 15 || gI == 38 || gI == 96 || gI == 164 ||
         gI == 89 || gI == 56 || gI == 74 || gI == 16 || gI == 14 || gI == 76 || gI == 178 || gI == 112 || gI == 123 || gI == 142 || gI == 132 ||
         gI == 148 || gI == 106 || gI == 28 || gI == 19 || gI == 65 || gI == 167 || gI == 162 || gI == 101 || gI == 129 || gI == 131 ||
         gI == 137 || gI == 95 || gI == 17 || gI == 22 || gI == 170 || gI == 172 || gI == 70 || gI == 104 || gI == 134 || gI == 103 || gI == 92 ||
         gI == 140 || gI == 98 || gI == 20 || gI == 175 || gI == 109 || gI == 100 || gI == 21 || gI == 139 || gI == 145 || gI == 128); //Action 13,21,32,34,42,52
  assert(gM == 17 || gM == 68 || gM == 35 || gM = 50 || gM = 53 || gM = 32); //Action 21,31,44,54
  assert(gQ == 10 || gQ == 60); //Action 44,51
  assert(gG == 1 || gG == 6); //Action 35,43,55
}

