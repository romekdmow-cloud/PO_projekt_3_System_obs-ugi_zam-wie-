
#include "../PLIKI.h/Wyjatki.h"

WyjatekProjektu::WyjatekProjektu(const std::string& msg)
	: wiadomosc(msg) {}

WyjatekProjektu::~WyjatekProjektu() = default;

const char* WyjatekProjektu::what() const noexcept {
	return wiadomosc.c_str();
}

StoliktZajety::StoliktZajety(int numerStolika)
	: WyjatekProjektu("BLAD: Stolik nr " + std::to_string(numerStolika) + " jest juz zajety!") {}

ZamowienieNieZnalezione::ZamowienieNieZnalezione(int id)
	: WyjatekProjektu("BLAD: Zamowienie o ID " + std::to_string(id) + " nie istnieje!") {}
