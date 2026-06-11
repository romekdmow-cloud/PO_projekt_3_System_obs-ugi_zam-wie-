#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../PLIKI.h/CenaPodstawowa.h"
#include "../PLIKI.h/CenaPromocyjna.h"
#include "../PLIKI.h/Produkt.h"
#include "../PLIKI.h/ZamowienieDostawa.h"
#include "../PLIKI.h/ZamowienieLokal.h"
#include "../PLIKI.h/Wyjatki.h"

using namespace std;

int wczytajInt(string komunikat, int minimum, int maksimum) {
    int liczba;

    while (true) {
        cout << komunikat;
        cin >> liczba;

        if (!cin.fail() && liczba >= minimum && liczba <= maksimum) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return liczba;
        }

        cout << "Bledna wartosc. Sprobuj ponownie.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string wczytajTekst(string komunikat) {
    string tekst;

    cout << komunikat;
    getline(cin, tekst);
    return tekst;
}

string dzisiejszaData() {
    time_t teraz = time(nullptr);
    tm* czas = localtime(&teraz);
    char bufor[11];

    strftime(bufor, sizeof(bufor), "%Y-%m-%d", czas);
    return bufor;
}

vector<Produkt> wczytajOferteZPliku(string nazwaPliku) {
    vector<Produkt> oferta;
    ifstream plik(nazwaPliku);

    if (!plik.is_open()) {
        cout << "Nie udalo sie otworzyc pliku z menu: " << nazwaPliku << "\n";
        return oferta;
    }

    string linia;

    while (getline(plik, linia)) {
        string nazwa;
        string cenaTekst;
        string typ;
        stringstream strumien(linia);

        getline(strumien, nazwa, ';');
        getline(strumien, cenaTekst, ';');
        getline(strumien, typ, ';');

        if (nazwa.empty() || cenaTekst.empty() || typ.empty()) {
            continue;
        }

        double cena;
        stringstream cenaStrumien(cenaTekst);

        if (!(cenaStrumien >> cena)) {
            continue;
        }

        oferta.push_back(Produkt(nazwa, cena, typ));
    }

    return oferta;
}

void wyswietlOferte(const vector<Produkt>& oferta) {
    cout << "\n--- Oferta ---\n";
    for (int i = 0; i < static_cast<int>(oferta.size()); i++) {
        cout << i + 1 << ". " << oferta[i] << "\n";
    }
}

shared_ptr<Zamowienie> znajdzZamowienie(vector<shared_ptr<Zamowienie>>& zamowienia, int id) {
    for (shared_ptr<Zamowienie>& zamowienie : zamowienia) {
        if (zamowienie->getId() == id) {
            return zamowienie;
        }
    }

    return nullptr;
}

void dodajProduktyDoZamowienia(shared_ptr<Zamowienie> zamowienie, const vector<Produkt>& oferta) {
    while (true) {
        wyswietlOferte(oferta);
        int wybor = wczytajInt("Wybierz produkt (0 konczy dodawanie): ", 0, oferta.size());

        if (wybor == 0) {
            break;
        }

        zamowienie->dodajProdukt(oferta[wybor - 1]);
        cout << "Dodano produkt.\n";   
    }
}

void wybierzStrategie(shared_ptr<Zamowienie> zamowienie) {
    cout << "\n1. Cena podstawowa\n";
    cout << "2. Cena promocyjna (-20% od 50 zl)\n";

    int wybor = wczytajInt("Wybierz sposob liczenia ceny: ", 1, 2);

    if (wybor == 1) {
        zamowienie->setStrategia(make_shared<CenaPodstawowa>());
    } else {
        zamowienie->setStrategia(make_shared<CenaPromocyjna>());
    }
}

bool czyStolikZajety(vector<shared_ptr<Zamowienie>>& zamowienia, int numerStolika) {
    for (shared_ptr<Zamowienie>& zamowienie : zamowienia) {
        ZamowienieLokal* lokal = dynamic_cast<ZamowienieLokal*>(zamowienie.get());
        
        if (lokal != nullptr && lokal->getNumerStolika() == numerStolika) {
            if (zamowienie->getStatus() == StatusZamowienia::NOWE || 
                zamowienie->getStatus() == StatusZamowienia::PRZYGOTOWYWANE) {
                return true;
            }
        }
    }
    return false;
}

void utworzZamowienie(vector<shared_ptr<Zamowienie>>& zamowienia, const vector<Produkt>& oferta, int& nastepneId) {
    cout << "\n--- Nowe zamowienie ---\n";
    cout << "1. Zamowienie w lokalu\n";
    cout << "2. Zamowienie z dostawa\n";

    int typ = wczytajInt("Wybierz typ zamowienia: ", 1, 2);
    shared_ptr<Zamowienie> noweZamowienie;

    if (typ == 1) {
        int stolik;
        do {
            stolik = wczytajInt("Podaj numer stolika (1-20): ", 1, 20);
            if (czyStolikZajety(zamowienia, stolik)) {
                throw StoliktZajety(stolik);
            }
        } while (czyStolikZajety(zamowienia, stolik));
        
        cout << "1. Kelner\n";
        cout << "2. Barman\n";
        int pracownik = wczytajInt("Wybierz obsluge: ", 1, 2);

        Pracownik obsluga = Pracownik::KELNER;
        if (pracownik == 2) {
            obsluga = Pracownik::BARMAN;
        }

        noweZamowienie = make_shared<ZamowienieLokal>(nastepneId, dzisiejszaData(), stolik, obsluga);
    } else {
        string adres = wczytajTekst("Podaj adres dostawy: ");
        string telefon = wczytajTekst("Podaj telefon klienta: ");

        noweZamowienie = make_shared<ZamowienieDostawa>(nastepneId, dzisiejszaData(), adres, telefon);
    }

    dodajProduktyDoZamowienia(noweZamowienie, oferta);

    if (noweZamowienie->getProdukty().empty()) {
        cout << "Zamowienie bez produktow nie zostalo dodane.\n";
        return;
    }

    wybierzStrategie(noweZamowienie);
    zamowienia.push_back(noweZamowienie);
    nastepneId++;

    cout << "Dodano zamowienie.\n";
}

void wyswietlZamowienia(vector<shared_ptr<Zamowienie>>& zamowienia) {
    cout << "\n--- Zamowienia ---\n";

    if (zamowienia.empty()) {
        cout << "Brak zamowien.\n";
        return;
    }

    for (shared_ptr<Zamowienie>& zamowienie : zamowienia) {
        cout << *zamowienie << "\n";
        zamowienie->wyswietlProdukty();
    }
}

void zmienStatus(vector<shared_ptr<Zamowienie>>& zamowienia) {
    int id = wczytajInt("Podaj ID zamowienia: ", 1, 100000);
    shared_ptr<Zamowienie> zamowienie = znajdzZamowienie(zamowienia, id);

    if (zamowienie == nullptr) {
        throw ZamowienieNieZnalezione(id);
    }

    cout << "1. Nowe\n";
    cout << "2. Przygotowywane\n";
    cout << "3. Gotowe\n";
    cout << "4. Anulowane\n";

    int wybor = wczytajInt("Wybierz nowy status: ", 1, 4);

    if (wybor == 1) {
        zamowienie->setStatus(StatusZamowienia::NOWE);
    } else if (wybor == 2) {
        zamowienie->setStatus(StatusZamowienia::PRZYGOTOWYWANE);
    } else if (wybor == 3) {
        zamowienie->setStatus(StatusZamowienia::GOTOWE);
    } else {
        zamowienie->setStatus(StatusZamowienia::ANULOWANE);
    }

    cout << "Zmieniono status.\n";
}

void edytujProdukty(vector<shared_ptr<Zamowienie>>& zamowienia, const vector<Produkt>& oferta) {
    int id = wczytajInt("Podaj ID zamowienia: ", 1, 100000);
    shared_ptr<Zamowienie> zamowienie = znajdzZamowienie(zamowienia, id);

    if (zamowienie == nullptr) {
        throw ZamowienieNieZnalezione(id);
    }

    cout << "1. Dodaj produkt\n";
    cout << "2. Usun produkt\n";

    int wybor = wczytajInt("Wybierz akcje: ", 1, 2);

    if (wybor == 1) {
        dodajProduktyDoZamowienia(zamowienie, oferta);
    } else {
        if (zamowienie->getProdukty().empty()) {
            cout << "To zamowienie nie ma produktow do usuniecia.\n";
            return;
        }

        zamowienie->wyswietlProdukty();
        int numer = wczytajInt("Podaj numer produktu do usuniecia: ", 1, zamowienie->getProdukty().size());

        if (zamowienie->usunProdukt(numer)) {
            cout << "Usunieto produkt.\n";
        } else {
            cout << "Nie udalo sie usunac produktu.\n";
        }
    }
}

void usunZamowienie(vector<shared_ptr<Zamowienie>>& zamowienia) {
    int id = wczytajInt("Podaj ID zamowienia do usuniecia: ", 1, 100000);

    for (auto it = zamowienia.begin(); it != zamowienia.end(); ++it) {
        if ((*it)->getId() == id) {
            zamowienia.erase(it);
            cout << "Usunieto zamowienie.\n";
            return;
        }
    }

    throw ZamowienieNieZnalezione(id);
}


void zapiszZamowienia(const vector<shared_ptr<Zamowienie>>& zamowienia, const string& nazwaPliku) {
    ofstream plik(nazwaPliku);

    if (!plik.is_open()) {
        cout << "Nie udalo sie otworzyc pliku do zapisu: " << nazwaPliku << "\n";
        return;
    }

    for (const shared_ptr<Zamowienie>& zam : zamowienia) {
        ZamowienieLokal* lokal = dynamic_cast<ZamowienieLokal*>(zam.get());
        ZamowienieDostawa* dostawa = dynamic_cast<ZamowienieDostawa*>(zam.get());

        if (lokal != nullptr) {
            plik << "LOKAL";
            plik << ";" << lokal->getId();
            plik << ";" << lokal->getData();
            plik << ";" << static_cast<int>(lokal->getStatus());
            plik << ";" << lokal->getNumerStolika();
            plik << ";" << static_cast<int>(lokal->getPracownik());
        } else if (dostawa != nullptr) {
            plik << "DOSTAWA";
            plik << ";" << dostawa->getId();
            plik << ";" << dostawa->getData();
            plik << ";" << static_cast<int>(dostawa->getStatus());
            plik << ";" << dostawa->getAdres();
            plik << ";" << dostawa->getTelefonKlienta();
        } else {
            continue;
        }

        plik << ";";
        const vector<Produkt>& produkty = zam->getProdukty();
        for (int i = 0; i < static_cast<int>(produkty.size()); i++) {
            if (i > 0) plik << "|";
            plik << produkty[i].getNazwa()
                 << ":" << produkty[i].getCenaBazowa()
                 << ":" << produkty[i].getTyp();
        }
        plik << "\n";
    }

    cout << "Zamowienia zapisane do pliku: " << nazwaPliku << "\n";
}

vector<string> podziel(const string& tekst, char znak) {
    vector<string> wynik;
    string token = "";
    for (char c : tekst) {
        if (c == znak) {
            wynik.push_back(token);
            token = "";
        } else {
            token += c;
        }
    }
    wynik.push_back(token);
    return wynik;
}

void wczytajZamowienia(vector<shared_ptr<Zamowienie>>& zamowienia, const string& nazwaPliku, int& nastepneId) {
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) return;

    string linia;
    while (getline(plik, linia)) {
        if (linia.empty()) continue;

        vector<string> pola = podziel(linia, ';');
        if (pola.size() < 7) continue;

        string typ = pola[0];
        int id = stoi(pola[1]);
        string data = pola[2];
        StatusZamowienia status = static_cast<StatusZamowienia>(stoi(pola[3]));

        shared_ptr<Zamowienie> zam;
        if (typ == "LOKAL") {
            int stolik = stoi(pola[4]);
            Pracownik prac = static_cast<Pracownik>(stoi(pola[5]));
            zam = make_shared<ZamowienieLokal>(id, data, stolik, prac);
        } else if (typ == "DOSTAWA") {
            zam = make_shared<ZamowienieDostawa>(id, data, pola[4], pola[5]);
        } else {
            continue;
        }

        zam->setStatus(status);

        string produktyTekst = pola[6];
        if (!produktyTekst.empty()) {
            vector<string> prodLista = podziel(produktyTekst, '|');
            for (const string& pTekst : prodLista) {
                vector<string> pPola = podziel(pTekst, ':');
                if (pPola.size() >= 3) {
                    zam->dodajProdukt(Produkt(pPola[0], stod(pPola[1]), pPola[2]));
                }
            }
        }

        zamowienia.push_back(zam);

        if (id >= nastepneId) {
            nastepneId = id + 1;
        }
    }

    if (!zamowienia.empty()) {
        cout << "Wczytano " << zamowienia.size() << " zamowien z pliku.\n";
    }
}

int main() {
    vector<Produkt> oferta = wczytajOferteZPliku("DANE/menu.txt");

    if (oferta.empty()) {
        cout << "Brak produktow w menu. Program konczy dzialanie.\n";
        return 0;
    }

    vector<shared_ptr<Zamowienie>> zamowienia;
    int nastepneId = 1;

    wczytajZamowienia(zamowienia, "DANE/zamowienia.txt", nastepneId);

    while (true) {
        try {
            cout << "\n===== System obslugi zamowien =====\n";
            cout << "1. Pokaz oferte\n";
            cout << "2. Dodaj zamowienie (CREATE)\n";
            cout << "3. Pokaz zamowienia (READ)\n";
            cout << "4. Zmien status zamowienia (UPDATE)\n";
            cout << "5. Edytuj produkty w zamowieniu (UPDATE)\n";
            cout << "6. Usun zamowienie (DELETE)\n";
            cout << "0. Koniec\n";

            int wybor = wczytajInt("Wybierz opcje: ", 0, 6);

            if (wybor == 1) {
                wyswietlOferte(oferta);
            } else if (wybor == 2) {
                utworzZamowienie(zamowienia, oferta, nastepneId);
            } else if (wybor == 3) {
                wyswietlZamowienia(zamowienia);
            } else if (wybor == 4) {
                zmienStatus(zamowienia);
            } else if (wybor == 5) {
                edytujProdukty(zamowienia, oferta);
            } else if (wybor == 6) {
                usunZamowienie(zamowienia);
            } else {
                // Zapisz zamowienia przed wyjsciem
                zapiszZamowienia(zamowienia, "DANE/zamowienia.txt");
                cout << "Koniec programu.\n";
                break;
            }
        }
        catch (StoliktZajety& e) {
            cout << "\n*** " << e.what() << " Wybierz inny stolik. ***\n";
        }
        catch (ZamowienieNieZnalezione& e) {
            cout << "\n*** " << e.what() << " ***\n";
        }
        catch (WyjatekProjektu& e) {
            cout << "\n*** NIEOCZEKIWANY BLAD: " << e.what() << " ***\n";
        }
    }

    cout << "\nProgram zakonczony pomyslnie.\n";
    return 0;
}
