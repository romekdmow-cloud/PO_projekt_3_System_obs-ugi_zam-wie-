# System Obsługi Zamówień Restauracji

Projekt realizujący obiektowy system zarządzania zamówieniami w lokalu gastronomicznym oraz w dostawie.

---

## 1. Opis celu projektu

## Problem i przeznaczenie
Program rozwiązuje problem ręcznego i niespójnego zarządzania zamówieniami w małych i średnich punktach gastronomicznych. Zastępuje tradycyjne papierowe notatki cyfrowym systemem. System skierowany jest do obsługi restauracji.

## Główne funkcjonalności
- Zarządzanie ofertą: Wczytywanie aktualnego menu (produktów) z pliku zewnętrznego (`DANE/menu.txt`).

- Tworzenie zamówień (CREATE): Obsługa dwóch niezależnych kanałów sprzedaży:

- Zamówienia na miejscu (Lokal): powiązane z numerem stolika oraz konkretnym pracownikiem obsługującym (Kelner/Barman). Zapobiega rezerwacji zajętego stolika.

- Zamówienia z dostawą (Dostawa): przechowujące dane adresowe oraz numer telefonu klienta.

- Wybór strategii naliczania cen: Elastyczne stosowanie różnych polityk cenowych (cena standardowa vs cena promocyjna z progami rabatowymi).

- Przegląd zamówień (READ): Wyświetlanie listy aktywnych zamówień wraz z ich szczegółami, produktami oraz dynamicznie obliczaną wartością końcową.

- Modyfikacja zamówień (UPDATE):

- Zmiana statusu zamówienia (Nowe, Przygotowywane, Gotowe, Anulowane).

- Edycja zawartości zamówienia (dodawanie i usuwanie produktów).

- Usuwanie zamówień (DELETE): Wycofanie zamówienia z systemu.

- Automatyczne zapisywanie bazy zamówień do pliku tekstowego (`DANE/zamowienia.txt`) przy wyjściu i ich wczytywanie przy starcie programu (dane nie giną między sesjami).

---

## 2. Architektura i struktura obiektowa

## Opis najważniejszych klas
- Produkt: Reprezentuje pojedynczy artykuł z menu. Przechowuje nazwę, cenę bazową oraz typ produktu (danie, napój, dodatek).
- Zamowienie (Klasa Abstrakcyjna): Główny szablon zamówienia. Definiuje wspólny interfejs, przechowuje unikalne ID, datę, status, listę produktów oraz wskaźnik do strategii obliczania ceny. Zawiera czysto wirtualną metodę `obliczCene()` oraz przeciążony polimorficzny operator `<<`.
- ZamowienieLokal (dziedziczy po `Zamowienie`): Rozszerza zamówienie o numer stolika (1-20) i typ obsługi (Kelner/Barman).
- ZamowienieDostawa (dziedziczy po `Zamowienie`): Rozszerza zamówienie o dane adresowe i numer telefonu.
- ObliczenieCeny (Interfejs / Klasa Abstrakcyjna): Baza dla wzorca projektowego *Strategia*. Definiuje metodę `oblicz()` przyjmującą listę produktów.
- CenaPodstawowa (dziedziczy po `ObliczenieCeny`): Strategia zwracająca standardową sumę cen produktów.
- CenaPromocyjna (dziedziczy po `ObliczenieCeny`): Strategia naliczająca rabat (-20% przy zamówieniu powyżej 50 zł, oraz dodatkowe -10 zł przy zamówieniu powyżej 100 zł).
- Wyjątki (`WyjatekProjektu`, `StoliktZajety`, `ZamowienieNieZnalezione`): Własna hierarchia klas wyjątków dziedzicząca po `std::exception` służąca do bezpiecznej obsługi błędów wykonania programu.

## Relacje między klasami
- Dziedziczenie:
  - `ZamowienieLokal` i `ZamowienieDostawa` dziedziczą po `Zamowienie`.
  - `CenaPodstawowa` i `CenaPromocyjna` dziedziczą po `ObliczenieCeny`.
  - `StoliktZajety` i `ZamowienieNieZnalezione` dziedziczą po `WyjatekProjektu` (dziedziczącym bezpośrednio po `std::exception`).
- Kompozycja i Agregacja :
  - Klasa `Zamowienie` posiada wewnątrz wektor obiektów `Produkt` (kompozycja).
  - Klasa `Zamowienie` agreguje obiekt strategii obliczania ceny poprzez inteligentny wskaźnik `std::shared_ptr<ObliczenieCeny>`.

## Uzasadnienie decyzji projektowych
- Wzorzec Strategia (Strategy Pattern): Umożliwia dynamiczną zmianę algorytmu naliczania rabatu w trakcie działania programu. Klasa `Zamowienie` nie musi wiedzieć, *jak* liczony jest rabat – deleguje to zadanie do obiektu strategii. Pozwala to na łatwe dodawanie nowych promocji (zasada Open/Closed).
- Polimorfizm: Metoda `obliczCene()` oraz przeciążony operator `<<` pozwalają na jednolite traktowanie obiektów `ZamowienieLokal` oraz `ZamowienieDostawa` w kontenerze `std::vector<std::shared_ptr<Zamowienie>>`.
- Inteligentne wskaźniki (`shared_ptr`): Zastosowane w celu automatycznego zarządzania pamięcią dynamiczną dla obiektów polimorficznych. Zapobiegają wyciekom pamięci (Memory Leaks).
- Hierarchia wyjątków: Pozwala na separację logiki biznesowej od obsługi sytuacji awaryjnych (np. próba zajęcia zajętego stolika, edycja nieistniejącego zamówienia) w czytelny sposób za pomocą bloków `try-catch`.

---

## 3. Krótka instrukcja uruchomienia i użycia

## Kompilacja projektu
Aby skompilować projekt, otwórz terminal w folderze głównym i uruchom polecenie:
```bash
g++ -I PLIKI.h PLIKI.cpp/*.cpp -o projekt.exe
```

## Uruchomienie programu
```bash
./projekt.exe
```

## Podstawowy scenariusz użycia
1. Po włączeniu program automatycznie wczyta listę produktów z `DANE/menu.txt` oraz poprzednio zapisane zamówienia z `DANE/zamowienia.txt`.
2. Wybierz `2`, aby dodać nowe zamówienie. Podaj jego szczegóły (stolik/adres), dodaj produkty z menu (wpisanie `0` kończy dodawanie) oraz wybierz sposób naliczenia ceny.
3. Wybierz `3`, aby wyświetlić aktualny spis zamówień.
4. Wybierz `0` (Koniec) – program automatycznie zapisze zaktualizowaną listę zamówień do pliku i zakończy działanie.

---

## 4. Opis podziału pracy w zespole

- Tymon Czarniawski (model podstawowy):
  - Pliki: `Produkt.h`/`Produkt.cpp`, `Typy.h`/`Typy.cpp`, `Zamowienie.h`/`Zamowienie.cpp`.

- Michał Walczyk (dziedziczenie i liczenie ceny):
  - Pliki: `ObliczenieCeny.h`/`ObliczenieCeny.cpp`, `CenaPodstawowa.h`/`CenaPodstawowa.cpp`, `CenaPromocyjna.h`/`CenaPromocyjna.cpp`, `ZamowienieLokal.h`/`ZamowienieLokal.cpp`, `ZamowienieDostawa.h`/`ZamowienieDostawa.cpp`.

- Mateusz Szarkowski (main, CRUD i pliki tekstowe):
  - Pliki: `projekt1.cpp`, `DANE/menu.txt`, `README.md` (oraz nowo dodana trwałość danych w `DANE/zamowienia.txt` i obsługa wyjątków `Wyjatki.h`/`Wyjatki.cpp`).
