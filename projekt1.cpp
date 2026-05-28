#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

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

class Produkt {
private:
    string nazwa;
    double cenaBazowa;
    string typ;

public:
    Produkt(string n, double c, string t) 
        : nazwa(n), cenaBazowa(c), typ(t) {}

    double getCenaBazowa() const { return cenaBazowa; }
    string getNazwa() const { return nazwa; }
};

class Zamowienie; 

class ObliczenieCeny {
protected:
    Zamowienie* powiazaneZamowienie; 

public:
    virtual ~ObliczenieCeny() = default;
    virtual double zastosujPromocje(double sumaBazowa) = 0;
    virtual double zastosujSet(double sumaBazowa) = 0;
};

class Zamowienie {
private:
    int idZamowienia;
    string dataZlozenia; 
    StatusZamowienia status;

protected:
    vector<Produkt> produkty; 
    shared_ptr<ObliczenieCeny> wybranaStrategia; 

public:
    Zamowienie(int id, string data) 
        : idZamowienia(id), dataZlozenia(data), status(StatusZamowienia::NOWE) {}
        
    virtual ~Zamowienie() = default; 

    int getId() const { return idZamowienia; }
    void setStatus(StatusZamowienia s) { status = s; }
    
    void dodajProdukt(const Produkt& p) {
        produkty.push_back(p);
    }

    void setStrategia(shared_ptr<ObliczenieCeny> strategia) {
        wybranaStrategia = strategia;
    }

    virtual double obliczCalkowitaWartosc() = 0; 
};

class ZamowienieLokal : public Zamowienie {
private:
    int numerStolika;
    Pracownik obsluga;

public:
    ZamowienieLokal(int id, string data, int stolik, Pracownik pr)
        : Zamowienie(id, data), numerStolika(stolik), obsluga(pr) {}

    double obliczCalkowitaWartosc() override {
        double suma = 0;
        for (const auto& prod : produkty) {
            suma += prod.getCenaBazowa();
        }
        
        //tu bedzie jakas metoda liczaca promocje w zamowieniu
        return suma;
    }
};

class ZamowienieDostawa : public Zamowienie {
private:
    string adresDostawy;
    string telefonKlienta;

public:
    ZamowienieDostawa(int id, string data, string adres, string tel)
        : Zamowienie(id, data), adresDostawy(adres), telefonKlienta(tel) {}

    double obliczCalkowitaWartosc() override {
        //tu bedzie petla podliczajaca produkty i liczaca koszt dostawy
        return 0.0; 
    }
};

class CenaPodstawowa : public ObliczenieCeny {
public:
    double zastosujPromocje(double sumaBazowa) override {
        return sumaBazowa; 
    }
    double zastosujSet(double sumaBazowa) override {
        return sumaBazowa;
    }
};

class CenaPromocyjna : public ObliczenieCeny {
public:
    double zastosujPromocje(double sumaBazowa) override {
        //cos typu algorytm na hapyy hours -20%
        return sumaBazowa; 
    }
    double zastosujSet(double sumaBazowa) override {
        //tu bedzie logika naliczania ceny po znizce za zestaw combo np
        return sumaBazowa;
    }
};