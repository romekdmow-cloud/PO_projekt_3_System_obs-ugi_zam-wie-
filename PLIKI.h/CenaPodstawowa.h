#ifndef CENA_PODSTAWOWA_H
#define CENA_PODSTAWOWA_H

#include "ObliczenieCeny.h"

class CenaPodstawowa : public ObliczenieCeny {
public:
    double zastosujPromocje(double sumaBazowa) override;
    double zastosujSet(double sumaBazowa) override;
};

#endif
