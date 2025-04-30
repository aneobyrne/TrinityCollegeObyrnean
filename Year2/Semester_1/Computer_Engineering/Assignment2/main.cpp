#include <iostream>
#include "BookingSystem.h"
using namespace std;

int main() {

    BookingSystem bookingSystemC("Confused Shopppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp", 10, 5); for(int
                                                                      i=0;i<20;i++) {
        bookingSystemC.RentWindowsLaptop();
        bookingSystemC.RentMacBook();
    }
    bookingSystemC.ReturnWindowsLaptop();
    bookingSystemC.ReturnMacBook(); bookingSystemC.PrintReport();


    return 0;
}
