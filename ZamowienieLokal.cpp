#include "../PLIKI.h/ZamowienieLokal.h"

ZamowienieLokal::ZamowienieLokal(int id, std::string data, int stolik, Pracownik pr)
    : Zamowienie(id, data), numerStolika(stolik), obsluga(pr) {
}

void ZamowienieLokal::setPracownik(Pracownik pr) {
    obsluga = pr;
}

int ZamowienieLokal::getNumerStolika() const {
    return numerStolika;
}

Pracownik ZamowienieLokal::getPracownik() const {
    return obsluga;
}

double ZamowienieLokal::obliczCalkowitaWartosc() {
    double suma = policzSumeBazowa();

    if (wybranaStrategia != nullptr) {
        suma = wybranaStrategia->zastosujPromocje(suma);
        suma = wybranaStrategia->zastosujSet(suma);
    }

    return suma;
}

std::string ZamowienieLokal::getOpis() const {
    return "Lokal, stolik " + std::to_string(numerStolika) + ", " + pracownikNaTekst(obsluga);
}
