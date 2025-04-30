#include "BookingSystem.h"
#include <iostream>
#include <iomanip>

using namespace std;

BookingSystem::BookingSystem(){
    name = "Trinity Laptop Bookings";
    totalWindowsLaptops= 20;
    totalMacBooks= 20;
    availableWindowsLaptops = totalWindowsLaptops;
    availableMacBooks = totalMacBooks;
}

BookingSystem::BookingSystem(string name, int totalWindowsLaptops, int totalMacBooks){
    if (name.size() > 64) //name should not exceed 64 char
        this->name = name.substr(0,64);
    else
        this->name = name;

    if (totalWindowsLaptops > 0) //no negative values
        this-> totalWindowsLaptops = totalWindowsLaptops;
    else
        this->totalWindowsLaptops = 0;
    availableWindowsLaptops = this -> totalWindowsLaptops;

    if (totalMacBooks > 0) //no negative values
        this -> totalMacBooks = totalMacBooks;
    else
        this -> totalMacBooks = 0;
    availableMacBooks = this -> totalMacBooks;
}

bool BookingSystem::RentWindowsLaptop(){
    if (availableWindowsLaptops > 0) {
        availableWindowsLaptops--;
        return true;
    }
    else
        return false;
}

bool BookingSystem::RentMacBook(){
    if (availableMacBooks > 0) {
        availableMacBooks--;
        return true;
    }
    else
        return false;
}

void BookingSystem::ReturnWindowsLaptop(){
    if (availableWindowsLaptops < totalWindowsLaptops)
        availableWindowsLaptops++;
}

void BookingSystem::ReturnMacBook(){
    if (availableMacBooks < totalMacBooks)
        availableMacBooks++;
}

void BookingSystem::PrintReport() {
    cout << "---------------------" << endl << "Laptop Booking System" << endl << "---------------------" << endl;
    cout << "Name" << setw(24) << right << " : " << getName() << endl;
    cout << "Total Windows Laptops" << setw(7) << right << " : " << getTotalWindowsLaptops() << endl;
    cout << "Total MacBooks" << setw(14) << right << " : " << getTotalMacBooks() << endl;
    cout << "Rented Windows Laptops" << setw(6) << right << " : " << getRentedWindowsLaptops() << endl;
    cout << "Rented MacBooks" << setw(13) << right << " : " << getRentedMacBooks() << endl;
    cout << "Rented Laptops" << setw(14) << right << " : " << getRentedLaptops() << endl;
    cout << "Available Windows Laptops" << right << " : " << getAvailableWindowsLaptops() << endl;
    cout << "Available MacBooks" << setw(10) << right << "  : " << getAvailableMacBooks() << endl;
    cout << "Available Laptops" << setw(11) << right << ": " << getAvailableLaptops();
}

string BookingSystem::getName(){
    return name;
}

unsigned int BookingSystem::getTotalWindowsLaptops(){
    return totalWindowsLaptops;
}

unsigned int BookingSystem::getTotalMacBooks(){
    return totalMacBooks;
}

unsigned int BookingSystem::getAvailableWindowsLaptops(){
    return availableWindowsLaptops;
}

unsigned int BookingSystem::getAvailableMacBooks(){
    return availableMacBooks;
}

unsigned int BookingSystem::getAvailableLaptops(){
    return getAvailableWindowsLaptops() + getAvailableMacBooks();
}

unsigned int BookingSystem::getRentedWindowsLaptops(){
    return totalWindowsLaptops - availableWindowsLaptops;
}

unsigned int BookingSystem::getRentedMacBooks(){
    return totalMacBooks - availableMacBooks;
}

unsigned int BookingSystem::getRentedLaptops(){
    return getRentedWindowsLaptops() + getRentedMacBooks();
}

void BookingSystem::addWindowsLaptops(unsigned int additionalWindowsLaptops){
    if ((availableWindowsLaptops + additionalWindowsLaptops) < totalWindowsLaptops)
        availableWindowsLaptops += additionalWindowsLaptops;
    else
        availableWindowsLaptops = totalWindowsLaptops;
}

void BookingSystem::addMacBooks(unsigned int additionalMacBooks){
    if ((availableMacBooks + additionalMacBooks) < totalMacBooks)
        availableMacBooks += additionalMacBooks;
    else
        availableMacBooks = totalMacBooks;
}

void BookingSystem::removeWindowsLaptops(unsigned int removedWindowsLaptops){
    if ((availableWindowsLaptops - removedWindowsLaptops) > 0)
        availableWindowsLaptops -= removedWindowsLaptops;
    else
        availableWindowsLaptops = 0;
}

void BookingSystem::removeMacBooks(unsigned int removedMacBooks){
    if ((availableMacBooks - removedMacBooks) > 0)
        availableMacBooks -= removedMacBooks;
    else
        availableMacBooks = 0;
}
