#include <cstring>
#include <stdexcept>
#include "cadena.hpp"

Cadena::Cadena(size_t tam, const char c) : s_(new char[tam+1]),tam_(tam) {
        for (unsigned i = 0; i < tam; ++i)
            s_[i]=c;
        s_[tam_]='\0';
}

Cadena::Cadena(const Cadena &cad ) : s_(new char[cad.tam_+1]),tam_(cad.tam_) {
    std::strcpy(s_, cad.s_);
}

Cadena::Cadena(const char * cc) : s_(new char[strlen(cc)+1]),tam_(strlen(cc)) {
    std::strcpy(s_, cc);
}

Cadena::Cadena(Cadena&& cad ):s_(cad.s_),tam_(cad.tam_){
    cad.tam_ = 0;
    cad.s_= nullptr;
}

Cadena::Cadena(const Cadena & ori, size_t begindex, size_t len) : s_(new char[len + 1]),tam_(len) {
        size_t i,j;
        for(i = 0,j = begindex; i < len; i++,j++)
            s_[i]=ori[j];
        s_[tam_] = '\0';
}

Cadena::~Cadena() {
        delete[] s_;
}

inline size_t Cadena::length() const noexcept { return tam_; }

char Cadena::operator [] (size_t i) const noexcept { return s_[i]; }

char& Cadena::operator [] (size_t i) noexcept { return s_[i]; }

Cadena& Cadena::operator = (Cadena&& cad)
{
    delete[] s_;
    tam_ = cad.tam_;
    s_= cad.s_;
    cad.s_ = nullptr;
    cad.tam_ = 0;
    return *this;
}

Cadena& Cadena::operator = (const Cadena & cad ) {
        if(this != &cad)
        {
            delete [] s_;
            tam_=cad.tam_;
            s_=new char[tam_+1];
            std::strcpy(s_,cad.s_);
        }
        return *this;
}

Cadena &Cadena::operator=(const char *cad) {
        delete[] s_;
        tam_ = strlen(cad);
        s_ = new char[tam_ + 1];
        std::strcpy(s_, cad);
        return *this;
}

char Cadena::at(size_t i) const {
        if (i >= tam_) throw std::out_of_range("Fuera de rango");
        return s_[i];
}

char& Cadena::at(size_t i) {
        if (i >= tam_) throw std::out_of_range("Fuera de rango");
        return s_[i];
}

Cadena &Cadena::operator+=(const Cadena &cad) {
        Cadena t = *this;
        tam_ = t.tam_+cad.tam_;
        delete [] s_;
        s_=new char[tam_+1];
        std::strcpy(s_,t.s_);
        std::strcat(s_,cad.s_);
        return *this;
}

Cadena operator + (const Cadena &cad1, const Cadena &cad2) {
        return Cadena(cad1) += cad2;
}

bool operator == (const Cadena &cad1, const Cadena &cad2) noexcept {
    return (std::strcmp(cad1.c_str(), cad2.c_str()) == 0);
}

bool operator!=(const Cadena &cad1, const Cadena &cad2) noexcept {
        return !(cad1==cad2);
}
bool operator > (const Cadena &cad1, const Cadena &cad2) noexcept {
    return (std::strcmp(cad1.c_str(),cad2.c_str()) > 0 );
}

bool operator < (const Cadena &cad1, const Cadena &cad2) noexcept {
        return !(cad1 == cad2) && !(cad1 > cad2);
}

bool operator <= (const Cadena &cad1, const Cadena &cad2) noexcept {
        return !(cad1 > cad2);
}

bool operator >= (const Cadena &cad1, const Cadena &cad2) noexcept {
        return (cad1 == cad2) || (cad1 > cad2);
}

Cadena Cadena::substr(size_t begindex, size_t len) const {

        size_t fin = begindex + len;
        if (begindex >= this->length() or fin > this->length() or fin < begindex)
                throw std::out_of_range("Error de rango");
        return Cadena(*this, begindex, len);
}

inline const char * Cadena::c_str() const { return s_; }

ostream& operator << (std::ostream& os, const Cadena& cad)
{
	os << cad.c_str();
	return os;
}

std::istream &operator>>(std::istream& input,Cadena& A)
{
  int i = 0;
  char* s=new char[33];
  while(isspace(input.get()) && i<32){i++;}
  input.unget();
  if(i<32)
  {
  input.width(33);
  input>>s;
  }
  A=Cadena(s);
  return input;
}
