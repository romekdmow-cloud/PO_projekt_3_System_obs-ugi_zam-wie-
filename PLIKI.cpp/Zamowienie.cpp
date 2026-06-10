#include "../PLIKI.h/Zamowienie.h"

Zamowienie::Zamowienie(int id, std::string data)
    : idZamowienia(id), dataZlozenia(data), status(StatusZamowienia::NOWE) {
}

int Zamowienie::getId() const {
    return idZamowienia;
}

void Zamowienie::setID(int id) {
    idZamowienia = id;
}

std::string Zamowienie::getData() const {
    return dataZlozenia;
}

void Zamowienie::setData(std::string data) {
    dataZlozenia = data;
}

StatusZamowienia Zamowienie::getStatus() const {
    return status;
}

void Zamowienie::setStatus(StatusZamowienia s) {
    status = s;
}

void Zamowienie::dodajProdukt(const Produkt& produkt) {
    produkty.push_back(produkt);
}

bool Zamowienie::usunProdukt(int numerProduktu) {
    if (numerProduktu < 1 || numerProduktu > static_cast<int>(produkty.size())) {
        return false;
    }

    produkty.erase(produkty.begin() + numerProduktu - 1);
    return true;
}

const std::vector<Produkt>& Zamowienie::getProdukty() const {
    return produkty;
}

void Zamowienie::setStrategia(std::shared_ptr<ObliczenieCeny> strategia) {
    wybranaStrategia = strategia;
}

double Zamowienie::policzSumeBazowa() const {
    double suma = 0.0;

    for (const Produkt& produkt : produkty) {
        suma += produkt.getCenaBazowa();
    }

    return suma;
}

void Zamowienie::wyswietlProdukty() const {
    if (produkty.empty()) {
        std::cout << "  Brak produktow.\n";
        return;
    }

    for (int i = 0; i < static_cast<int>(produkty.size()); i++) {
        std::cout << "  " << i + 1 << ". " << produkty[i] << "\n";
    }
}

std::ostream& operator<<(std::ostream& os, Zamowienie& zamowienie) {
    os << "ID: " << zamowienie.getId()
       << " | data: " << zamowienie.getData()
       << " | status: " << statusNaTekst(zamowienie.getStatus())
       << " | typ: " << zamowienie.getOpis()
       << " | wartosc: " << zamowienie.obliczCalkowitaWartosc() << " zl";
    return os;
}
