#include "../PLIKI.h/Produkt.h"

Produkt::Produkt(std::string n, double c, std::string t)
    : nazwa(n), cenaBazowa(c), typ(t) {
}

double Produkt::getCenaBazowa() const {
    return cenaBazowa;
}

std::string Produkt::getNazwa() const {
    return nazwa;
}

std::string Produkt::getTyp() const {
    return typ;
}

std::ostream& operator<<(std::ostream& os, const Produkt& produkt) {
    os << produkt.getNazwa() << " (" << produkt.getTyp() << ") - "
       << produkt.getCenaBazowa() << " zl";
    return os;
}
