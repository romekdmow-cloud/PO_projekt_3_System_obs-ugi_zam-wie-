#include "../PLIKI.h/CenaPromocyjna.h"

bool CenaPromocyjna::sprawdzKryteriaPromocji(double sumaBazowa) {
    return sumaBazowa >= 50.0;
}

double CenaPromocyjna::zastosujPromocje(double sumaBazowa) {
    if (sprawdzKryteriaPromocji(sumaBazowa)) {
        return sumaBazowa * 0.80;
    }

    return sumaBazowa;
}

double CenaPromocyjna::zastosujSet(double sumaBazowa) {
    if (sumaBazowa >= 200.0) {
        return sumaBazowa - 20.0;
    }

    return sumaBazowa;
}
