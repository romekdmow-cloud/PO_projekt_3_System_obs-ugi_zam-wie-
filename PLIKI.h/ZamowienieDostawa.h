#ifndef ZAMOWIENIE_DOSTAWA_H
#define ZAMOWIENIE_DOSTAWA_H

#include "Zamowienie.h"

class ZamowienieDostawa : public Zamowienie {
private:
    std::string adresDostawy;
    std::string telefonKlienta;

public:
    ZamowienieDostawa(int id, std::string data, std::string adres, std::string tel);

    void setAdres(std::string adres);
    std::string getAdres() const;
    std::string getTelefonKlienta() const;
    double policzKosztDostawy() const;

    double obliczCalkowitaWartosc() override;
    std::string getOpis() const override;
};

#endif
