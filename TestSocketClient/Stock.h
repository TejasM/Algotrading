/*
 * Stock class
 */

#ifndef STOCK_H
#define STOCK_H

#include<string>

// Dummy stock class
class Stock{
    std::string tick;
    public:
       // make a constructor and pass in tick / etc. eventually
       // make a destructor
       float getEMA();
       float getMACD();
       std::string getTick();
};

#endif
