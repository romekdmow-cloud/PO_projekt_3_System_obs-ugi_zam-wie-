#include "../PLIKI.h/ZamowienieDostawa.h"

ZamowienieDostawa::ZamowienieDostawa(int id, std::string data, std::string adres, std::string tel)
    : Zamowienie(id, data), adresDostawy(adres), telefonKlienta(tel) {
}

void ZamowienieDostawa::setAdres(std::string adres) {
    adresDostawy = adres;
}

std::string ZamowienieDostawa::getAdres() const {
    return adresDostawy;
}

std::string ZamowienieDostawa::getTelefonKlienta() const {
    return telefonKlienta;
}

double ZamowienieDostawa::policzKosztDostawy() const {
    if (policzSumeBazowa() >= 80.0) {
        return 0.0;
    }

    return 8.0;
}

double ZamowienieDostawa::obliczCalkowitaWartosc() {
    double suma = policzSumeBazowa();

    if (wybranaStrategia != nullptr) {
        suma = wybranaStrategia->zastosujPromocje(suma);
        suma = wybranaStrategia->zastosujSet(suma);
    }

    return suma + policzKosztDostawy();
}

std::string ZamowienieDostawa::getOpis() const {
    return "Dostawa, adres: " + adresDostawy + ", tel: " + telefonKlienta;
}
