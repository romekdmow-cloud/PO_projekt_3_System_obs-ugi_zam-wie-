#ifndef WYJATKI_H
#define WYJATKI_H

#include <exception>
#include <string>

class WyjatekProjektu : public std::exception {
protected:
    std::string wiadomosc;

public:
    explicit WyjatekProjektu(const std::string& msg);
    virtual ~WyjatekProjektu();

    virtual const char* what() const noexcept override;
};

class StoliktZajety : public WyjatekProjektu {
public:
    explicit StoliktZajety(int numerStolika);
};

class ZamowienieNieZnalezione : public WyjatekProjektu {
public:
    explicit ZamowienieNieZnalezione(int id);
};

#endif

