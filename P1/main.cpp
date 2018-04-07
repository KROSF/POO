#include "cadena.cpp"
#include <iostream>
int main()
{
  using namespace std;
  Cadena c;
  cin>>c;
  cout<<"Palabra 1: "<<c<<" "<<c.length()
      <<"\nCter. actual: '"<<static_cast<char>(cin.peek())<<"'"<<endl;
  cin>>c;
  cout<<"Palabra 2: "<<c<<" "<<c.length()<<endl;

  return 0;
}
