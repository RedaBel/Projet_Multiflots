#include "INSTANCE.hpp"
using namespace std;

int Network::compteur = 0;

Network::Network(int n0,float alpha, float beta,int maxdemand0,int maxcapacity0,int maxcost0){
	n = n0 ;
	d = alpha*n*n ;
	sumdemand 	= 0;
	sumcapacity = 0;
	sumarc		= 0;
    maxdemand = maxdemand0 ;
    maxcapacity = maxcapacity0;
    maxcost = maxcost0;
	int	max_arc		= beta*n*n,
		i			= 0,
		j			= 0;

	//----initialisation al�atoire de la matrice des liens----//
	
	
	link = new  int* [n] ;

	
	
	for(int i = 0 ; i<n ; i++){
		link[i] = new int[n] ;
		} 

	
	
	for (int i = 0 ; i < n ; i++) {
		for(int j = 0 ; j < n ; j++){
			link[i][j] = 0 ;	
			
		}
	}

	while(sumarc != max_arc){
		i = rand()%n ;
		j = rand()%n ;
		if(!link[i][j])
		{
			link[i][j] = 1;
			sumarc += 1;
		}
	}
		
		

	//----initialisation al�atoire de la matrice des demandes---//


	    demand = new int* [d] ;
    
    for(int i = 0 ; i<d ; i++){
        demand[i] = new int[3] ;
        }
    
    for (int i = 0 ; i < d ; i++) {
        demand[i][0] = 0 ;
        demand[i][1] = 0 ;
        demand[i][2] = 0 ;
        }
    
    	for (int i = 0 ; i < d ; i++) {
		do{
			demand[i][0] = rand() % n ; //source
			demand[i][1] = rand() % n ; //destination
          }while(!link[demand[i][0],demand[i][1]]);
		demand[i][2] = 1 + rand() % maxdemand ;
		sumdemand = sumdemand + demand[i][2];
		
		while (demand[i][0] == demand[i][1]) {
			
			demand[i][0] = rand() % n ; 
			
			}
		
		}


	//----initialisation al�atoire de la matrice des capacites---//
		
	capacity = new  int* [n] ;

	
	
	for(int i = 0 ; i<n ; i++){
		capacity[i] = new int[n] ;
		} 
	
	for (int i = 0 ; i < n ; i++) {
		for(int j = 0 ; j < n ; j++){
			capacity[i][j] = 0 ;
			
			
			
		}
	}
		for (int i = 0 ; i < n ; i++) {
		for(int j = 0 ; j < n ; j++){
			if (link[i][j]) {
			capacity[i][j] = 1  + rand()%maxcapacity;
			sumcapacity = sumcapacity + capacity[i][j];
			
		}
		}
	}
	
	//----initialisation al�atoire de la matrice des co�ts---//
	
	cost = new  int* [n] ;
	
	for(int i = 0 ; i<n ; i++){
		cost[i] = new int[n] ;
		} 
	
	for (int i = 0 ; i < n ; i++) {
		for(int j = 0 ; j < n ; j++){
			cost[i][j] = 0 ;		
			
		}
	}
	
	
	for (int i = 0 ; i < n ; i++) {
		for(int j = 0 ; j < n ; j++){
			if (link[i][j]) {
			cost[i][j] = 1+rand()%maxcost ;
		}
		}
	}
	
	cout<< "Le nombre d'arcs est " << sumarc << endl;
	
	cout<< "La capacit� est de " << sumcapacity << endl;
	
	cout<< "Les demandes sont de " << sumdemand << endl;
	
	if(sumcapacity >= sumdemand)
		cout << "Capacit� sup�rieure aux demandes."<< endl;
	else
		cout << "Capacit� inf�rieure aux demandes."<<endl;
	
}

 


void Network::toCplex() {
	
	std::string result;
	std::stringstream sstm;
	sstm << "Data/TEST" << compteur << ".lp";
	result = sstm.str();
    std::ofstream file (result.c_str());
    
    if (file.is_open())
    {
        file << "Problem name: INSTANCE" << compteur << ".lp\n";
        // Fct obj
        file << "Min\n";
        file << "obj: " ;
        std::string chaine = "";
       
      
      int z = 0 ;
      
      for (int k = 0 ; k < d ; k++) {
      
        for(int i=0;i<n;++i)
        {
			for(int j=0;j<n;++j)
			{
				if(link[i][j]) {
					
					
					chaine.append(patch::to_string(cost[i][j]) + " x"+patch::to_string(z)+"+");
					
					z++ ;
					}
        }}}
       
        chaine = chaine.substr(0, chaine.size()-1);
        file << chaine ;
        chaine = "" ;
   
        // (Constraints)
        file << "\nSubject to\n";
        
        
          //(contraintes de capacit�s)
        
      z = 0 ;
  
      
for(int a=0;a<n;++a)
{
	for(int b=0;b<n;++b)
	{
				if(link[a][b]) {
							
		z = 0 ;
					
					
		for (int k = 0 ; k < d ; k++) {
      
        for(int i=0;i<n;++i)
        {
			for(int j=0;j<n;++j)
			{
				if(link[i][j]) {
				  
				  if(( i == a )&&(j == b)) {
					  
					  chaine.append("x"+patch::to_string(z)+"+");
  
					  }			
					z++ ;
					}
        }}}
       
            chaine = chaine.substr(0, chaine.size()-1);
            file << chaine ;
            file << "<=" +patch::to_string(capacity[a][b]) +"\n" ;
            chaine = "\0" ;           			
		  }
        }
        
      }
        
              //contrainte de flot
              
   for(int kb=0; kb<d;++kb){
	for(int a =0; a<n;a++)
				 {
					 
					 			  
       z = 0 ;
      
      for (int k = 0 ; k < d ; k++) {
      
        for(int i=0;i<n;++i)
        {
			for(int j=0;j<n;++j)
			{
				if(link[i][j]) {
					
					if(( i == a )&&(kb == k)) {
					  
					  chaine.append("+x"+patch::to_string(z));
					  
					  
					  }	
					if(( j == a )&&(kb == k)) {
					  
					  chaine.append("-x"+patch::to_string(z));
		  
					  }	

					z++ ;
					}
        }}}
					 			 
        file << chaine ;
        
		if (a == demand[kb][0] && !chaine.empty()) {
					       file << "=" + patch::to_string(demand[kb][2])+ "\n"; 
				 }
		else if (a == demand[kb][1] && !chaine.empty()) {
					       file << "= -"+patch::to_string(demand[kb][2])+"\n";
				 }						 
        
         else if ((a != demand[kb][0] ) && (a != demand[kb][1] ) && !chaine.empty())
           file << "= 0\n";
        
        chaine = "\0" ;	 
				 }}
				
        //(2d)
        
        
         z = 0 ;
      
      for (int k = 0 ; k < d ; k++) {
      
        for(int i=0;i<n;++i)
        {
			for(int j=0;j<n;++j)
			{
				if(link[i][j]) {
					
					
					chaine.append("x"+patch::to_string(z)+">= 0\n");
					file << chaine ;
				    chaine = "" ;
					z++ ;
					}
        }}}
      
        file <<"End" ;
                  
        file.close();
    }
    else std::cout << "Unable to open file";
}

Network::~Network(){
	
	for( int i = 0 ; i < n ; i++ )
	{
		delete[] link[i];
	}
	delete[] link;
	for( int i = 0 ; i < n ; i++ )
	{
		delete[] capacity[i];
	}
	delete[] capacity;
	for( int i = 0 ; i < n ; i++ )
	{
		delete[] cost[i];
	}
	delete[] cost;
	for( int i = 0 ; i < d ; i++ )
	{
		delete[] demand[i];
	}
	delete[] demand;

}

int main(int argc, char ** argv) {
	int n 	  		= 0,
		maxdemand	= 1,
		maxcapacity = 10,
		maxcost		= 10;
	float 	alpha 		= 0,
			beta  		= 0;
	int boolean	    = 1;
	std::string result;
	std::stringstream sstm;
	std::stringstream sstm2;
	
	cout<< "Entrez le nombre de sommets: " << endl;
	cin >> n;
	
	for(float alpha = 0.1; alpha <= 1; alpha += 0.1){
		for(float beta = 0.1; beta <= 0.5; beta += 0.1){
	srand(time(NULL)) ;
	
	
	
	//cout<< "Entrez le param�tre alpha (total commodit�s): " << endl;
	//cin >> alpha;
	//cout<< "Entrez le param�tre beta (total arcs): " << endl;
	//cin >> beta;
	//cout<< "Entrez la demande max pour un arc: " << endl;
	//cin >> maxdemand;
	//cout<< "Entrez la capacit� max d'un arc: " << endl;
	//cin >> maxcapacity;
	//cout<< "Entrez le co�t max de passage par un arc: " << endl;
	//cin >> maxcost;
	
	IloNum start;
	float diff = 0;
	Network instance (n,alpha,beta,maxdemand,maxcapacity,maxcost);
	sstm << "Data/TEST" << Network::compteur << ".lp";
	sstm2 << "Data/TEST" << Network::compteur << "log.txt";
	
	instance.toCplex();
		
		result = sstm2.str();
		ofstream fout(result.c_str());
		fout << "n =" << n << std::endl ;
		fout << "alpha =" << alpha << std::endl ;
		fout << "beta =" << beta << std::endl ;
		fout << "maxdemand =" << maxdemand << std::endl ;
		fout << "maxcapacity =" << maxcapacity << std::endl ;
		fout << "maxcost =" << maxcost << std::endl ;
		fout << "Sum demands =" << instance.sumdemand << std::endl ;
		fout << "Sum arcs =" << instance.sumarc << std::endl ;
		fout << "Sum capacities =" << instance.sumcapacity << std::endl ;
		
		result.clear();
		result = sstm.str();

		cout << "Instance Generated. Creation of the LP file..." <<endl;
		try{
	    IloEnv env; 
        IloModel model(env) ; 
        IloCplex cplex(model) ;
	    cplex.IloCplex::importModel(model,result.c_str()) ;
		std::cout << "Problem read by CPLEX" << std::endl ;
		cplex.setOut(fout);
		cplex.setWarning(fout);
		cplex.setError(fout);
		start=cplex.getTime();
		if(!cplex.solve()){
				throw(-1);
			}
		else{
			fout << "solution = " << cplex.getObjValue() << std::endl ;
			fout << "solution time = " << cplex.getTime()-start << endl;
			std::cout << "Log File Generated." << std::endl ;
			}
		}
		catch(IloException& e) {
				fout << "Concert exception caught: " << e.getMessage() << endl;
				std::cout << "Log Exception File Generated." << std::endl ;
			}
		catch (...) {
			fout << "Unknown exception caught" << endl;	
			std::cout << "Log Unknown Exception File Generated." << std::endl ;
			}
			fout.close();
		
	sstm.str(std::string());
	sstm2.str(std::string());
	result.clear();
	Network::compteur++;
	continue;
}
}

	return 0;

}

