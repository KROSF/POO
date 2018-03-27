#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchersInternal.h"
#include "clang/ASTMatchers/ASTMatchersMacros.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/Support/CommandLine.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CompilationDatabase.h"

#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "execute.h"


#ifndef CHECKCODE_H
#define CHECKCODE_H

class checkCode : public MatchFinder::MatchCallback{

  public:
	checkCode(int argc, const char **argv, string fileClass = "", string help = "Comando: ./main (*).cpp -- -std=c++11");

/****************************************************************************
**
** PUBLIC FUNCTIONS
**
****************************************************************************/

/*
Método: void setCorrectMessage(string message)
Utilidad: Método para establecer el mensaje que se mostrará al final de la comprobación en caso de que todo esté correcto.
Parámetros:
---string message: cadena de caracteres con el mensaje a mostrar.
*/
	void setCorrectMessage(string message);

/*
Método: void setIncorrectMessage(string message)
Utilidad: Método para establecer el mensaje que se mostrará al final de la comprobación en caso de que alguna verificación haya fallado.
Parámetros:
---string message: cadena de caracteres con el mensaje a mostrar.
*/
	void setIncorrectMessage(string message);

/*
Método: void check()
Utilidad: Método que ha de ser llamado tras un conjunto de comprobaciones para ejecutarlas.
*/
	void check();




/****************************************************************************
**
** PARÁMETROS COMUNES
** Parámetros que son compartidos para todos o varios de los métodos disponibles.
**
****************************************************************************/	
/*
---string message: Opcionalmente, se puede incluir un parámetro que incluya el mensaje a mostrar en pantalla en caso
   de que la verificación no sea satisfactoria. Si no se introduce este parámetro, se proporcionará un mensaje por defecto.

---bool exist: Por defecto, una comprobación se realiza para validar que existe un elemento en el código. Con este parámetro se puede revertir el comportamiento, pasando a validar que un elemento no exista en el código. Por defecto, el parámetro está a "true".

---string constant: Para indicar si un método es constante (en ese caso, "const") o si no lo es (en ese caso, "noconst");

*/



/****************************************************************************
**
** CONSIDERACIONES COMUNES
**
****************************************************************************/	
/*
- Un método particular se identifica por la clase a la que pertenece, su nombre y la lista de los tipos de sus parámetros (y no por sus nombres).

- Si el método es un operador sobrecargado, el nombre será la palabra "operator" junto con el operador sin espacio entre medio. Por ejemplo "operator+=". 

- Respecto a los tipos, se ha de indicar el tipo original, es decir, sin tener en cuenta typedef o alias. Por ejemplo:
typedef unsigned long miTipo;
...
int metodo (miTipo parametro);

El tipo de "parametro" es "unsigned long" y no "miTipo".

- En ciertas comprobaciones y parámetros dentro de esas comprobaciones, se permite introducir "?" (interrogación) para indicar que el parámetro no se tenga en cuenta, principalmente cuando no es conocido o puede variar. Por ejemplo, si introducimos "{"?"}" como lista de parámetros de un método, la comprobación no se ceñirá a un método en particular, sino que tratará de equiparar la comprobación a todos los métodos de la clase indicada que tengan el nombre indicado. 
En las descripciones de las comprobaciones en este fichero, esta propiedad se indica con un (*) en aquellos parámetros que lo permitan. 
Nota: En el caso de que se introduzca "*" como argumento de un parámetro en el que se indica la lista de parámetros de un método, no se tendrá en cuenta la lista de parámetros. Hay que distinguir esta situación del método que no tiene parámetros, en cuyo caso se indicará con la lista vacía {}.  

*/
/****************************************************************************
**
** CLASES
** Métodos dedicados a la búsquedas de clases.
**
****************************************************************************/
/*
- Método: bool findClass(string className)

- Utilidad: Método para comprobar si una clase existe. 
			Devuelve "true" si se encontró, en caso contrario devolverá "false". 
			La comprobación se ejecuta individualmente cuando es invocada, 
			de forma que se puede usar como primera condición antes de pasar a realizar 
			otras verificaciones sobre la misma clase.

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar. 

- Ejemplo: 
	if (findClass("Fecha")){
		comprobación1();
		comprobación2();
		...
	}
	else{
		cout << "Clase no encontrada."<<endl;
	}
*/
      bool findClass(string className);

/*
- Método: void classWithName(string className, string message)

- Utilidad: Método para comprobar si una clase existe. A diferencia de la comprobación "findClass", esta comprobación se ejecuta cuando se invoca el método "check". 

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar.

- Ejemplo:
  
  class A { ... };

  classWithName(A); 	No daría error
  classWithName(B); 	Daría error
*/
	void classWithName(string className, string message = "CLASS NOT FOUND.");






/****************************************************************************
**
** CONSTRUCTORES
** Métodos dedicados a la comprobación de diferentes constructores en una clase.
**
****************************************************************************/

/*
- Método: void defaultConstructor(string className, bool exist, string message)

- Utilidad: Método para verificar que una clase tiene constructor por defecto definido.

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar. 

- Ejemplo:

  class A { A() { ... } };
  class B { B() = default; };
  class C { C(Tipo)  { ... } }; 

  defaultConstructor("A"); No daría error
  defaultConstructor("B"); No daría error
  defaultConstructor("C"); Daría error

*/
	void defaultConstructor(string className, bool exist = true, string message = "DEFAULT CONSTRUCTOR NOT FOUND.");


/*
- Método: void listInitializerConstructor(string className, vector<string> parameters, vector<string> initializers, string message)

- Utilidad: Método para verificar que una clase tiene un constructor que haga uso de una lista de inicialización.

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar. 
---vector<string> parameters (?): vector de cadena de caracteres con los tipos de los parámetros del constructor.
---vector<string> initializers (?): vector de cadenas de caracteres con los tipos de los atributos a los que se inicializa en la lista de inicialización del constructor.

- Ejemplo:

  class A { 
  	int x; 
  	A() { x = 0; } 
  };
  class B { 
  	int x; 
  	B() : x(0) { ... } 
  };  

  listInitializerConstructor("A", {"?"}, {"int"});		Daría error
  listInitializerConstructor("B", {"?"}, {"int"}); 		No daría error

*/
	void listInitializerConstructor(string className, vector<string> parameters, vector<string> initializers, string message = "CLASS WITH THE SPECIFIED CONSTRUCTOR AND INITIALIZATION LIST NOT FOUND.");


/*
- Método: void copyConstructor(string className, bool exist, string message)

- Utilidad: Método para verificar que una clase tiene constructor de copia.

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar. 

- Ejemplo:

  class A { ...
  	A(const A&) { ... }
  ... };					
  class B { ...
  	B(const B&) = default;
  ... };					

  class C { ...
  	C(const C&) = delete;
  ... };					

  copyConstructor("A"); 	No daría error
  copyConstructor("B"); 	No daría error
  copyConstructor("C"); 	Daría error

*/
	void copyConstructor(string className, bool exist = true, string message = "COPY CONSTRUCTOR NOT FOUND.");


/*
- Método: void moveConstructor(string className, string message)

- Utilidad: Método para verificar que una clase tiene constructor de movimiento.

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar. 

- Ejemplo:

  class A { ...
  	A(const A&&) { ... }	
  ... };					
  class B { ...
  	B(const B&&) = default;	
  ... };					

  class C { ...
  	C(const C&&) = delete;	
  ... };


  moveConstructor("A"); No daría error
  moveConstructor("B"); No daría error
  moveConstructor("C"); Daría error
*/
	void moveConstructor(string className, bool exist = true, string message = "MOVE CONSTRUCTOR NOT FOUND.");


/*
- Método: void destructor(string className, string message)

- Utilidad: Método para verificar que una clase tiene destructor explícito.

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar. 

- Ejemplo:

  class A { ...
  	~A() { ... }
  ... };					
  class B { ...
  	~B() = default;
  ... };					
  class C { ...
   	...
  ... };

  destructor("A");  No daría error
  destructor("B");  No daría error
  destructor("C");  Daría error
*/
	void destructor(string className, bool exist = true, string message = "DESTRUCTOR NOT FOUND.");


/*
- Método: void destructorDeleteMember(string className, vector<string> memberNames, string message)

- Utilidad: Método para verificar que en el destructor de una clase se procede a hacer "delete" a un miembro.

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar.
---vector<string> memberName (*): vector de cadena de caracteres con los nombres de los miembros a los que se le aplica el operador delete.

- Ejemplo:

	class A{				
		Tipo* a;
		A(): a{new Tipo()}{ ... }
		~A() {delete a;}
	};

	class B{				
		Tipo* b;
		B(): b{new Tipo()} { ... }
		~B() { //no hace delete }
	};

	destructorDeleteMember("B", {"b"}); 	Daría error
	destructorDeleteMember("A", {"a"}); 	No daría error

*/
      void destructorDeleteMember(string className, vector<string> memberNames, string message = "MEMBER VARIABLE NOT DELETED IN DESTRUCTOR.");

/*
- Método: void numberOfConstructors(string className, unsigned int constructors, bool lenient, string message)

- Utilidad: Método para comprobar si una clase tiene un número determinado de constructores.

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar.
---unsigned int constructors: entero que indica el número de constructors en la clase.
---lenient: booleano que indica si se permite un número igual o mayor de constructores (true) o solo un número igual de constructores (false). Por defecto a "false".

- Ejemplo:

	class A{
		A() { ... }
		A(const A&) { ... }
	};

	numberOfConstructors("A", 2, false);	No daría error 
	numberOfConstructors("A", 1, true);	No daría error
	numberOfConstructors("A", 1, false);	Daría error
*/
      void numberOfConstructors(string className, unsigned int constructors, bool lenient = false, string message = "INCORRECT NUMBER OF CONSTRUCTORS.");


/*
- Método: void explicitSpecifiedConstructor(string className, vector<string> parameters, string message);

- Utilidad: Método para comprobar si un determinado constructor está marcado "Explicit Specified".

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar. 
---vector<string> parameters (?): vector de cadena de caracteres con los tipos de los parámetros del constructor.

- Ejemplo:

	class A{
		A(Tipo1 a) { ... }
		explicit A(Tipo2 a) { ... }
	};

	explicitSpecifiedConstructor("A", {"Tipo1"}); Daría error
	explicitSpecifiedConstructor("A", {"Tipo2"}); No daría error 

*/
      void explicitSpecifiedConstructor(string className, vector<string> parameters, string message = "EXPLICIT CONSTRUCTOR NOT FOUND.");





/****************************************************************************
**
** EXCEPCIONES
** Métodos dedicados a la comprobación sobre excepciones.
**
****************************************************************************/

/*
- Método: void takeException(vector<string> exception, string message);

- Utilidad: Método para comprobar si se lanza una excepción.

- Parámetros:
---vector<string> className: vector de cadena de caracteres con el nombre de la excepción.

- Ejemplo:

	class A{
		void metodo1() { ... }
	}

	void A::metodo1() {
		...
		throw Excepcion1(...);
	}

	takeException({"Excepcion1"});	No daría error
	takeException({"Excepcion2"});	Daría error porque no se lanza la excepción "Excepcion2"

*/
      void takeException(vector<string> exceptionClasses, string message = "EXCEPTION NOT FOUND");






/****************************************************************************
**
** VARIABLES
** Métodos dedicados a la verificación de miembros de una clase.
**
****************************************************************************/

/*
- Método: void memberVariable(string className, vector<string> memberNames, vector<string> constant, string message)

- Utilidad: Método para verificar si una clase tiene un determinado atributo.

Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar. 
---vector<string> memberName (*): vector de cadena de caracteres que contendrá la lista de nombres de atributos que deseamos buscar.
---vector<string> constant (?): vector de cadena de caracteres para indicar si las variables a buscar son constantes o no.

- Ejemplo:

	class A{
		Tipo a
		const Tipo b;
	};

	memberVariable("A", {"a", "b"}, {"noconst", "const"}); 	No daría error
	memberVariable("A", {"a", "b"}, {"?", "?"});		No daría error
	memberVariable("A", {"c"}, {"?"}); 			Daría error
	memberVariable("A", {"a"}, {"const"});			Daría error

*/
	void memberVariable(string className, vector<string> memberNames, vector<string> constant, string message = "MEMBER VARIABLE NOT FOUND.");


/*
- Método: void staticVariable(string className, vector<string> variableNames, string message)

- Utilidad: Método para verificar la existencia de una variable "static const" en una clase.

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar. 
---vector<string> variableNames (*): vector de cadena de caracteres que contendrá la lista de nombres de las variables "static const" que deseamos buscar.
---vector<string> constant (?): vector de cadena de caracteres para indicar si las variables a buscar son constantes o no.

- Ejemplo:

	class A{
		static const Tipo a
		static Tipo b;
		const Tipo c;
	};

	staticVariable("A", {"a", "b"}, {"const", "noconst"}); 		No daría error
	staticVariable("A", {"a", "b"}, {"const", "const"}); 		Daría error
	staticVariable("A", {"a", "b"}, {"noconst", "noconst"}); 	Daría error
	staticVariable("A", {"c"}, {"const"}); 				Daría error
	staticVariable("A", {"a", "c"}, {"const", "const"}); 		Daría error
*/
	void staticVariable(string className, vector<string> variableNames, vector<string> constant, string message = "STATIC CONST VARIABLE NOT FOUND.");


/*
- Método: void releaseVariable(string message)

- Utilidad: Método para comprobar que todas las variables que se crean con "new", posteriormente son eliminadas con "delete".

- Ejemplo:

	{
		Tipo* a = new Tipo();
		...
		delete a;
	}
	
	En este caso no daría error porque se liberan las variables.

	{
		Tipo* a = new Tipo();
		...
	}
	
	En este caso sí daría error porque no se liberan las variables.

*/
	void releaseVariable(string message = "NOT ALL VARIABLES RELEASED.");


/*
- Método: void memberVariableAccessLevel(string className, vector<string> memberNames, string level, string message)

- Utilidad: Método para verificar si una clase tiene un determinado atributo con un nivel de acceso específico.

Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar. 
---vector<string> memberName (*): vector de cadena de caracteres que contendrá la lista de nombres de atributos que deseamos buscar.
---string level: cadena de caracteres que indica el nivel de acceso (public, protected or private).

- Ejemplo

	class A{
	public:
		Tipo1 a1;
		Tipo2 a2;
	private:
		Tipo3 b;	
	};

	class B{
	private:
		Tipo1 a1;
		Tipo2 a2;
		Tipo3 a3;	
	};

	memberVariableAccessLevel("A", {"a1", "a2", "b"}, "public"); 	Daría error
	memberVariableAccessLevel("A", {"a.*"}, "public"); 				No daría error
	memberVariableAccessLevel("A", {"b"}, "private"); 				No daría error
	memberVariableAccessLevel("A", {".*"}, "private"); 				Daría error
	memberVariableAccessLevel("B", {".*"}, "private"); 				No daría error

*/
        void memberVariableAccessLevel(string className, vector<string> memberNames, string level, string message = "MEMBER VARIABLE WITH THAT ACCESS LEVEL NOT FOUND.");





/*****************************************************************************
**
** MÉTODOS
** Métodos dedicados a la búsqueda de diferentes métodos en una clase.
**
*****************************************************************************/

/*
- Método: void method(vector<string> methodNames, vector<vector<string> > parameters, string className, vector<string> constant,string message)

- Utilidad: Método para validar si un método existe.

- Parámetros:
---vector<string> methodNames (*): vector de cadena de caracteres con los nombres de los métodos a buscar.
---vector<vector<string> > parameters (?): vector de vector de cadena de caracteres con los tipos de los parámetros de los métodos a buscar.  
---string className (*): cadena de caracteres con el nombre de la clase donde debemos buscar los métodos deseados. 
---vector<string> constant (?): vector de cadena de caracteres para indicar si los métodos a buscar son constantes o no.

- Ejemplo:

	class A{
		void metodo1(Tipo1, Tipo2) { ... }
		void metodo2() { ... }
	};

	method({"metodo1", "metodo2"}, {{"Tipo1", "Tipo2"}, {}}, "A", {"?", "?"}); 	No daría error
	method({"metodo3"}, {{"?"}}, "A"); 						Daría error

*/
	void method(vector<string> methodNames, vector<vector<string> > parameters, string className, vector<string> constant = {"?"}, string message="METHOD NOT FOUND.");


/*
- Método: void methodWithNameReferenceVariable(vector<string> methodNames, vector<vector<string> > parameters, string className, vector<string> constant, string usedVariable, string message)

- Utilidad: Método para validar si un método hace referencia a una determinada variable.

- Parámetros:
---vector<string> methodNames (*): vector de cadena de caracteres con los nombres de los métodos a buscar. 
---vector<vector<string> > parameters (?): vector de vector de cadena de caracteres con los tipos de los parámetros de los métodos a buscar.
---string className (*): cadena de caracteres con el nombre de la clase donde debemos buscar los métodos deseados. 
---vector<string> constant (?): vector de cadena de caracteres para indicar si los métodos a buscar son constantes o no.
---vector<string> usedVariables: vector de cadena de caracteres con los nombres de los atributos que queremos buscar referenciados en los métodos.

- Ejemplo:

	class A{
		int a;
		void metodo1() { 
			...
			a = 3;
			...
		}
		void metodo2(){
			...
			// No usa 'a'
		}
	};

	methodWithReferencedVariable({"metodo1"}, {{"?"}}, "A", {"?"}, {"a"}); No daría error
	methodWithReferencedVariable({"metodo2"}, {{"?"}}, "A", {"?"}, {"a"}); Daría error

*/
	void methodWithReferencedVariable(vector<string> methodNames, vector<vector<string> > parameters, string className, vector<string> constant, vector<string> usedVariables, string message = "METHOD WITH REFERENCED MEMBER VARIABLE NOT FOUND.");


/*
- Método: void methodWithReferencedMethod(tor<string> mainMethodNames, vector<vector<string> > mainMethodparameters, string mainClassName, vector<string> mainConstant, vector<string> usedMethodName, vector<vector<string> > usedMethodparameters, string usedClassName, vector<string> usedConstant, string message);

- Utilidad: Método para validar si un método hace referencia a un determinado método.

- Parámetros:
---vector<string> mainMethodNames (*): vector de cadena de caracteres con el nombre de los métodos principales a buscar. 
---vector<vector<string> > mainMethodparameters (?): vector de vector de cadena de caracteres con los tipos de los parámetros de los métodos principales a buscar. 
---string mainClassName (*): cadena de caracteres con el nombre de la clase donde debemos buscar los métodos principales. 
---vector<string> constant (?): vector de cadena de caracteres para indicar si los métodos principales a buscar son constantes o no.
---vector<string> usedMethodNames (*): cadena de caracteres con el nombre de los métodos que queremos buscar referenciados en el método principal.
---vector<vector<string> > usedMethodparameters (?): vector de vector de cadena de caracteres con los tipos de los parámetros de los métodos referenciados a buscar. 
---string usedClassName (*): cadena de caracteres con el nombre de la clase donde debemos buscar los métodos referenciados. 
---vector<string> constant (?): vector de cadena de caracteres para indicar si los métodos referenciados a buscar son constantes o no.

- Ejemplo:

	class A{
		void metodo3() { ... }
		void metodo1(){ 
			...
			metodo3();
			...
		}
		void metodo2(){
			...
			// No usa 'metodo3'
		}
	};

	methodWithReferencedMethod({"metodo1"}, {{"?"}}, "A", {"?"}, {"metodo3"}, {{"?"}}, "A", {"?"}); No daría error
	methodWithReferencedMethod({"metodo2"}, {{"?"}}, "A", {"?"}, {"metodo3"}, {{"?"}}, "A", {"?"}); Daría error

*/
	void methodWithReferencedMethod(vector<string> mainMethodNames, vector<vector<string> > mainMethodparameters, string mainClassName, vector<string> mainConstant, vector<string> usedMethodName, vector<vector<string> > usedMethodparameters, string usedClassName, vector<string> usedConstant, string message = "METHOD WITH REFERENCED METHOD NOT FOUND.");


/*
- Método:  void noExceptMethod(vector<string> methodNames, vector<vector<string> > parameters, string className, vector<string> constant, string message)

- Utilidad: Método para determinar si un determinado método está marcado como "noexcept".

- Parámetros:
---vector<string> methodNames (*): vector de cadena de caracteres con los nombres de los métodos a buscar. 
---vector<string> parameters (?): vector de vector de cadena de caracteres con los tipos de los parámetros de los métodos a buscar.
---string className (*): cadena de caracteres con el nombre de la clase donde debemos buscar los métodos deseados.
---vector<string> constant (?): vector de cadena de caracteres para indicar si los métodos a buscar son constantes o no.

- Ejemplo:

	class A{
		void metodo1() { ... }
		void metodo2() noexcept { ... }
	};

	noExceptMethod({"metodo1"}, {{"?"}}, "A", {"?"}); Daría error
	noExceptMethod({"metodo2"}, {{"?"}}, "A", {"?"}); No daría error

*/
      void noExceptMethod(vector<string> methodNames, vector<vector<string> > parameters, string className, vector<string> constant = {"?"}, string message = "METHOD NO EXCEPT NOT FOUND.");


/*
- Método:  void inlineMethod(vector<string> methodNames, vector<vector<string> > parameters, string className, vector<string> constant, string message)

- Utilidad: Método para comprobar que un método de una clase está marcado como "inline"

- Parámetros:
---vector<string> methodNames (*): vector de cadena de caracteres con los nombres de los métodos a buscar. 
---vector<string> parameters (?): vector de vector de cadena de caracteres con los tipos de los parámetros de los métodos a buscar.
---string className (*): cadena de caracteres con el nombre de la clase donde debemos buscar los métodos deseados.
---vector<string> constant (?): vector de cadena de caracteres para indicar si los métodos a buscar son constantes o no.

- Ejemplo:

	class A{
		void metodo1(){...}
		void metodo2();
		void metodo3();
	};

	void A::metodo2(){...}
	inline void A::metodo3(){...}

	inlineMethod({"metodo1"}, {{"?"}}, "A", {"?"}); 			No daría error
	inlineMethod({"metodo1", metodo2"}, {{"?"}, {"?"}}, "A", {"?","?"}); 	No daría error
	inlineMethod({"metodo3"}, {{"?"}}, "A", {"?"});		 		Daría error
	inlineMethod({"metodo2", metodo3"}, {{"?"}, {"?"}}, "A", {"?","?"}); 	Daría error

*/
      void inlineMethod(vector<string> methodNames, vector<vector<string> > parameters, string className, vector<string> constant = {"?"}, string message = "INLINE METHODS NOT FOUND.");


/*
- Método:  void defaultArgumentsInMethod(vector<string> methodNames, vector<vector<string> > parameters, string className, vector<string> constant, vector<unsigned int> defaultArgs, string message)

- Utilidad: Método para comprobar que un método de una clase tiene un número determinado de argumentos por defecto

- Parámetros:
---vector<string> methodNames (*): vector de cadena de caracteres con los nombres de los métodos a buscar. 
---vector<string> parameters (?): vector de vector de cadena de caracteres con los tipos de los parámetros de los métodos a buscar.
---string className (*): cadena de caracteres con el nombre de la clase donde debemos buscar los métodos deseados.
---vector<string> constant (?): vector de cadena de caracteres para indicar si los métodos a buscar son constantes o no.
---vector<unsigned int> defaultArgs: vector de enteros para indicar el número de argumentos por defecto que los métodos a buscar deben tener.

- Ejemplo:

	class A{
		void metodo1(Tipo1 a=0) { ... }
		void metodo2(Tipo1, Tipo2=0) { ... }
	};

	defaultArgumentsInMethod({"metodo1"}, {{"Tipo1"}}, "A", {"?"}, {1});		No daría error
	defaultArgumentsInMethod({"metodo2"}, {{"Tipo1", "Tipo2"}}, "A", {"?"}, {2});	Daría error

*/
      void defaultArgumentsInMethod(vector<string> methodNames, vector<vector<string> > parameters, string className, vector<string> constant = {"?"}, 
				    vector<unsigned int> numDefaultArgs = {0},  vector<vector<string> > defaultArgs={{""}}, string message = "INLINE METHODS NOT FOUND.");


/*
- Método:  void deletedMethod(vector<string> methodNames, vector<vector<string>> parameters, string className, vector<string> constant, string message);

- Utilidad: Método para comprobar que los métodos de una clase están marcado como "= delete"

- Parámetros:
---vector<string> methodNames (*): vector de cadena de caracteres con los nombres de los métodos a buscar. 
---vector<string> parameters (?): vector de vector de cadena de caracteres con los tipos de los parámetros de los métodos a buscar.
---string className (*): cadena de caracteres con el nombre de la clase donde debemos buscar los métodos deseados.
---vector<string> constant (?): vector de cadena de caracteres para indicar si los métodos a buscar son constantes o no.

- Ejemplo:

	class A{
		A() { ... }
		A(const A &) = delete
	};

	deletedMethod({"A"}, {{"?"}}, "A", {"?"});			Daría error
	deletedMethod({"A"}, {{"const class A &"}}, "A", {"?"});	No daría error

*/

	  void deletedMethod(vector<string> methodNames, vector<vector<string>> parameters, string className, vector<string> constant = {"?"}, string message = "DELETED METHODS NOT FOUND.");


/*
- Método:  void defaultedMethod(vector<string> methodNames, vector<vector<string>> parameters, string className, vector<string> constant, string message);

- Utilidad: Método para comprobar que los métodos de una clase están marcado como "= default"

- Parámetros:
---vector<string> methodNames (*): vector de cadena de caracteres con los nombres de los métodos a buscar. 
---vector<string> parameters (?): vector de vector de cadena de caracteres con los tipos de los parámetros de los métodos a buscar.
---string className (*): cadena de caracteres con el nombre de la clase donde debemos buscar los métodos deseados.
---vector<string> constant (?): vector de cadena de caracteres para indicar si los métodos a buscar son constantes o no.

- Ejemplo:

	class A{
		A() { ... }
		A(const A &) = default
	};

	defaultedMethod({"A"}, {{"?"}}, "A", {"?"});			Daría error
	defaultedMethod({"A"}, {{"const class A &"}}, "A", {"?"}); 	No daría error

*/
	void defaultedMethod(vector<string> methodNames, vector<vector<string>> parameters, string className, vector<string> constant = {"?"}, string message = "DEFAULT METHODS NOT FOUND.");


/****************************************************************************
**
** OPERADORES
** Métodos dedicados a la búsqueda de diferentes operadores en una clase.
**
****************************************************************************/

/*
- Método: void copyAssignmentOperator(string className, string message)

- Utilidad: Método para buscar clases con operador de asignación de copia.

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar. 
---bool exist: Indica si existe o no el operador de asignación

-Ejemplo:

	class A{
		A& operator=(const A&) { ... }
	};

	class B{

	};

	copyAssignmentOperator("A", true);	No daría error
	copyAssignmentOperator("A", false); 	Daría error

	copyAssignmentOperator("B", false); 	No daría error
	copyAssignmentOperator("B", true); 	Daría error

*/
	void copyAssignmentOperator(string className, bool exist, string message = "COPY ASSIGNMENT OPERATOR NOT FOUND.");


/*
- Método: void moveAssignmentOperator(string className, string message)

- Utilidad: Método para buscar clases con operador de asignación de movimiento.

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar.

-Ejemplo:

	class A{
		A& operator=(const A&&) { ... }
	};

	class B{

	};

	moveAssignmentOperator("A", true);	No daría error
	moveAssignmentOperator("A", false); 	Daría error
	moveAssignmentOperator("B", false); 	No daría error
	moveAssignmentOperator("B", true); 	Daría error
*/
	void moveAssignmentOperator(string className, bool exist = true, string message = "MOVE ASSIGNMENT OPERATOR NOT FOUND.");


/*
- Método:  void function(vector<string> functionNames, vector<vector<string> > parameters, string message)

- Utilidad: Método para determinar si una determinada función existe.

- Parámetros:
---vector<string> functionNames (*): vector de cadena de caracteres con el nombre de las funciones a buscar.
---vector<string> parameters (?): vector de vector de cadena de caracteres con los tipos de los parámetros de las funciones a buscar.  

- Ejemplo:

	{
		void funcion1(Tipo1 parametro){
			...
		}
	}

	funcion({"funcion1"}, {{"Tipo1"}}, {"?"});  	No daría error
	funcion({"funcion1"}, {{"?"}}, {"?"});		No daría error
	funcion({"funcion2"}, {{"?"}}, {"?"});		Daría error (no existe la función 'funcion2')

*/
	void function(vector<string> functionNames, vector<vector<string> > parameters, string message = "FUNCTION NOT FOUND.");


/*
- Método: void functionWithReferencedFunction(vector<string> mainFunctionNames, vector<vector<string> > mainFunctionParameters, vector<string> usedFunctionNames, vector<vector<string> > usedFunctionParameters, string message)

- Utilidad: Método para validar si una determinada función está haciendo referencia a otra función.

- Parámetros:
---vector<string> mainFunctionNames (*): vector de cadena de caracteres con los nombres de las funciones principales que deseamos buscar.
---vector<vector<string> > mainFunctionParameters (?): vector de vector de cadena de caracteres con los tipos de los parámetros de las funciones principales a buscar. 
---vector<string> usedFunctionNames (*): vector de cadena de caracteres con los nombres de las funciones que queremos buscar referenciadas en las funciones principales.
---vector<vector<string> > usedFunctionParameters (?): vector de cadena de caracteres con los tipos de los parámetros de la funciones referenciadas a buscar.

- Ejemplo:

	
		void funcion3() { ... }
		void funcion1(){ 
			...
			funcion3();
			...
		}
		void funcion2(){
			...
			// No usa 'funcion3'
		}
	

	functionWithReferencedFunction({"funcion1"}, {{"?"}}, {"?"}, {"funcion3"}, {{"?"}}, {"?"}); No daría error
	functionWithReferencedFunction({"funcion2"}, {{"?"}}, {"?"}, {"funcion3"}, {{"?"}}, {"?"}); Daría error


*/
	void functionWithReferencedFunction(vector<string> mainFunctionNames, vector<vector<string> > mainFunctionParameters, vector<string> usedFunctionNames, vector<vector<string> > usedFunctionParameters, string message = "FUNCTION WITH REFERENCED FUNCTION NOT FOUND.");





/****************************************************************************
**
** CABECERAS
** Métodos dedicados a la verificación de la inclusión de cabeceras correctas.
**
****************************************************************************/
/*
- Método:  void includedHeader(vector<string> functionNames, string headerName, string message = "HEADER THAT MUST BE INCLUDED NOT FOUND.");

- Utilidad: Método para determinar si, en caso de usar ciertas funciones, se han tomado de la cabecera correcta.

- Parámetros:
---vector<string> functionNames: vector de cadena de caracteres con el nombre de las funciones a buscar.
---string headerName: cadena de caracteres con el nombre de la cabecera correcta.
---bool exist: booleano que permite indicar si deseamos comprobar que las funciones están siendo tomadas de la cabecera (true) o, si por el contrario, en caso de usar funciones incluidas en esa cabecera, comprobar que no se están tomando de esa cabecera (false). 

- Ejemplo:


	cabecera.h:
		void funcion1();
		void funcion2();
	codigo.cpp:
		#include "cabecera.h"
		...
		funcion1();
		...
		funcion2();
		...
		funcion3();

	includedHeader({"funcion1", "funcion2"}, "cabecera.h");	No daría error
	includedHeader({"funcion3"}, "cabecera.h"); 		Daría error
	includedHeader({"funcion2", "funcion3"}, "cabecera.h"); Daría error

*/
	void includedHeader(vector<string> functionNames, string headerName, bool exist = true, string message = "A PROBLEM WITH THE INCLUDED HEADERS DETECTED.");


/*
- Método:  void fileIncludedHeader(string headerName, vector<string> fileNames, string message = "HEADER IS NOT INCLUDED IN ANY OF THE FILES");

- Utilidad: Método para determinar si se ha incluido una cierta cabecera en alguno de los ficheros indicados.

- Parámetros:
---string headerName: cadena de caracteres con el nombre de la cabecera a buscar.
---vector<string> fileNames: vector de cadena de caracteres con el nombre de los ficheros donde buscar.

- Ejemplo:

	fichero1.cpp:
		#include "cabecera1.h"
	fichero2.cpp:
		#include "cabecera2.h"
	fichero3.cpp:
		#include "cabecera1.h"
	
	fileIncludedHeader("cabecera1.h", {"fichero1.cpp", "fichero3.cpp"}); 	No daría error
	fileIncludedHeader("cabecera2.h", {"fichero1.cpp"});			Daría error
	fileIncludedHeader("cabecera2.h", {"fichero1.cpp", "fichero3.cpp"}); 	Daría error

*/
      void fileIncludedHeader(string headerName, vector<string> fileNames, string message = "HEADER IS NOT INCLUDED IN ANY OF THE FILES");


/*
- Método: void guardClauses(string fileName, string message="THE GUARD CLAUSE IS NOT FOUND IN THE FILE")

- Utilidad: Método para determinar si se ha incluido la cláusula de guarda en un fichero.

- Parámetros:
---string fileName: cadena de caracteres con el nombre del fichero donde tiene que estar la cláusula de guarda.

- Ejemplo:

	codigo1.h:
	...

	codigo2.h:
	#ifndef GUARDA
	#define GUARDA
	...
	#endif

	guardClauses("codigo1.h"); Daría error
	guardClauses("codigo2.h"); No daría error

*/
      void guardClauses(string fileName, string message="THE GUARD CLAUSE IS NOT FOUND IN THE FILE");



/****************************************************************************
**
** MIEMBROS FRIEND
** Método dedicado a la comprobación sobre la existencia de miembros declarados
** como "friend" dentro de una determinada clase.
**
****************************************************************************/
/*
- Método:  void notFriendMember(string className, string message = "FRIEND MEMBER FOUND.");

- Utilidad: Método para verificar que no existen miembros "friend" en una clase.

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase a buscar

- Ejemplo:

	class A{
		...
	};

	class B{
		friend void metodo();
	};

	class C{
		friend A;
	};

	class D{
		friend A;
		friend void metodo();
	}

	notFriendMember("A"); No daría error
	notFriendMember("B"); Daría error
	notFriendMember("C"); Daría error
	notFriendMember("D"); Daría error

*/
	void notFriendMember(string className, string message = "FRIEND MEMBER FOUND.");


/*
- Método: void friendFunction(vector<string> functionNames, vector<vector<string> > parameters, string className, string message)

- Utilidad: Método para validar que una función ha sido marcada como amiga de una clase.

- Parámetros:
---vector<string> functionNames (*): vector de cadena de caracteres con los nombres de las funciones a buscar.
---vector<vector<string> > parameters (?): vector de vector de cadena de caracteres con los tipos de los parámetros de las funciones a buscar.  
---string className (*): cadena de caracteres con el nombre de la clase donde debemos buscar las funciones deseadas. 


-Ejemplo:


	void funcion();

	clase A{
		void metodo();
	};

	clase B{
		friend void funcion();
	};

	friendFunction({"funcion"}, {{"?"}}, "A", {"?"}); Daría error
	friendFunction({"funcion"}, {{"?"}}, "B", {"?"}); No daría error

*/
	void friendFunction(vector<string> functionNames, vector<vector<string> > parameters, string className, string message="FRIEND FUNCTION NOT FOUND.");

   
/*
- Método: void friendClass(string className, vector<string> friendClassesNames, string message)

- Utilidad: Método para validar que una clase ha sido marcada como amiga de otra clase.

- Parámetros:
---string className (*): cadena de caracteres con el nombre de la clase donde debemos buscar las clases amigas.
---vector<string> friendClassesNames (*): vector de cadena de caracteres con los nombres de las clases amigas a buscar.

- Ejemplo:

	clase A{
		...
	};

	clase B{
		friend class A;
	};

	friendClass("B", {"A"}); No daría error
	friendClass("A", {"B"}); Daría error

*/

	void friendClass(string className, vector<string> friendClassesName, string message="FRIEND CASS NOT FOUND");



/****************************************************************************
**
** FUNCIONES PRIVADAS
**
****************************************************************************/
   private:

      //Auxiliares
      void apply_clase(const MatchFinder::MatchResult &Result);
      list<string> getListSources(int argc, const char **argv);
      bool foundClass();
      string getHelper();
      string setFunctions(vector<string> functionNames);
      void setHelper(string help);


      //Clang
      virtual void run(const MatchFinder::MatchResult &Result);
      MatchFinder getFinder();
      


/****************************************************************************
**
** MIEMBROS PRIVADOS
**
****************************************************************************/

	//Argumentos
	vector<string> argvs;
	int argc1, argc2;
	const char **argv1;
	const char **argv2;

	//Clang
	MatchFinder Finder;
	ASTContext *Context;
	Rewriter Rewrite;
	
	//Objeto execute para ejecución
	execute e;    

	//Auxiliares
	string helper, classMessage;
	bool clase_encontrada = false;
	string fileClass = "";
	list<string> listSources;
	list<string>::iterator it;
	vector<string> p;

};




/****************************************************************************
**
** FUNCION ADICIONAL
**
****************************************************************************/
 list<string> cabeceras_leidas(string file_name, string orden);

#endif //CONTROL_H
