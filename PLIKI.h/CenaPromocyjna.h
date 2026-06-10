#ifndef CENA_PROMOCYJNA_H
#define CENA_PROMOCYJNA_H

#include "ObliczenieCeny.h"

class CenaPromocyjna : public ObliczenieCeny {
public:
    double zastosujPromocje(double sumaBazowa) override;
    double zastosujSet(double sumaBazowa) override;
    bool sprawdzKryteriaPromocji(double sumaBazowa);
};

#endif
