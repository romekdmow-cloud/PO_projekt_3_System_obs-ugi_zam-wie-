#ifndef ZAMOWIENIE_H
#define ZAMOWIENIE_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "ObliczenieCeny.h"
#include "Produkt.h"
#include "Typy.h"

class Zamowienie {
private:
    int idZamowienia;
    std::string dataZlozenia;
    StatusZamowienia status;

protected:
    std::vector<Produkt> produkty;
    std::shared_ptr<ObliczenieCeny> wybranaStrategia;

public:
    Zamowienie(int id, std::string data);
    virtual ~Zamowienie() = default;

    int getId() const;
    void setID(int id);
    std::string getData() const;
    void setData(std::string data);
    StatusZamowienia getStatus() const;
    void setStatus(StatusZamowienia s);

    void dodajProdukt(const Produkt& produkt);
    bool usunProdukt(int numerProduktu);
    const std::vector<Produkt>& getProdukty() const;
    void setStrategia(std::shared_ptr<ObliczenieCeny> strategia);

    double policzSumeBazowa() const;
    void wyswietlProdukty() const;

    virtual double obliczCalkowitaWartosc() = 0;
    virtual std::string getOpis() const = 0;
};

std::ostream& operator<<(std::ostream& os, Zamowienie& zamowienie);

#endif
