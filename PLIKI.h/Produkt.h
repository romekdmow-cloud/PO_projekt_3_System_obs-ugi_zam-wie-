#ifndef PRODUKT_H
#define PRODUKT_H

#include <iostream>
#include <string>

class Produkt {
private:
    std::string nazwa;
    double cenaBazowa;
    std::string typ;

public:
    Produkt(std::string n, double c, std::string t);

    double getCenaBazowa() const;
    std::string getNazwa() const;
    std::string getTyp() const;
};

std::ostream& operator<<(std::ostream& os, const Produkt& produkt);

#endif
