#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <queue>
#include <utility> 
#include <stack>
#include <utility> 
#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include <limits>
#include <ilcplex/ilocplex.h>



 
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


class Network{
	public:
    int n ; // taille du réseau : nombre de noeuds

    int d ; // nombre de flots

   int maxdemand ;
   
   int maxcapacity ;
   
   int maxcost ;
   
   int maxfixedcost ;
   
   int sumdemand ;
   
   int sumarc ;
   
   int sumcapacity ;

	int ** link   ; // link[i][j]=1 s'il existe un lien entre les noeuds i et j et 0 sinon
	
	int ** capacity  ;
	
	int ** cost  ;
	
	int ** fixedcost ;
	
    int ** demand ;
    
    static int compteur;


	Network();
	Network(int n0,float alpha, float beta,int maxdemand0,int maxcapacity0,int maxcost0,int maxfixedcost0); // genere une instance aleatoire de n sommets , d demandes 
	void toCplex() ; // méthode qui écrit l'instance dans un fichier lisible par CPLEX
	~Network();

	
};




#endif
