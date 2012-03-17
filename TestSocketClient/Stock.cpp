/*
 * Stock class
 */

#include "Stock.h"

using namespace std;

float Stock::getEMA() {
    return rand() % 100;
}

float Stock::getMACD() {
    return rand() % 100;
}

string Stock::getTick() {
    return "tick";
}
