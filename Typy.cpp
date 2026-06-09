#include "../PLIKI.h/Typy.h"

std::string statusNaTekst(StatusZamowienia status) {
    switch (status) {
    case StatusZamowienia::NOWE:
        return "Nowe";
    case StatusZamowienia::PRZYGOTOWYWANE:
        return "Przygotowywane";
    case StatusZamowienia::GOTOWE:
        return "Gotowe";
    case StatusZamowienia::ANULOWANE:
        return "Anulowane";
    default:
        return "Nieznany";
    }
}

std::string pracownikNaTekst(Pracownik pracownik) {
    switch (pracownik) {
    case Pracownik::KELNER:
        return "Kelner";
    case Pracownik::BARMAN:
        return "Barman";
    default:
        return "Nieznany";
    }
}
