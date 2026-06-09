#ifndef OBLICZENIE_CENY_H
#define OBLICZENIE_CENY_H

class Zamowienie;

class ObliczenieCeny {
protected:
    Zamowienie* powiazaneZamowienie;

public:
    ObliczenieCeny();
    virtual ~ObliczenieCeny() = default;

    virtual double zastosujPromocje(double sumaBazowa) = 0;
    virtual double zastosujSet(double sumaBazowa) = 0;
};

#endif
