#ifndef ZAMOWIENIE_LOKAL_H
#define ZAMOWIENIE_LOKAL_H

#include "Zamowienie.h"

class ZamowienieLokal : public Zamowienie {
private:
    int numerStolika;
    Pracownik obsluga;

public:
    ZamowienieLokal(int id, std::string data, int stolik, Pracownik pr);

    void setPracownik(Pracownik pr);
    int getNumerStolika() const;
    Pracownik getPracownik() const;

    double obliczCalkowitaWartosc() override;
    std::string getOpis() const override;
};

#endif
