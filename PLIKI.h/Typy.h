#ifndef TYPY_H
#define TYPY_H

#include <string>

enum class StatusZamowienia {
    NOWE,
    PRZYGOTOWYWANE,
    GOTOWE,
    ANULOWANE
};

enum class Pracownik {
    KELNER,
    BARMAN
};

std::string statusNaTekst(StatusZamowienia status);
std::string pracownikNaTekst(Pracownik pracownik);

#endif
