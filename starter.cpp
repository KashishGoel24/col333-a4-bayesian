#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>


// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

private:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

	vector<float> sample_amt ; 
	float total_samples ;
	// vector<float> learn_cpt ; 

public:
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals )
	{
		Node_Name=name;
		nvalues=n;
		values=vals;
		total_samples = n ; 
		vector<int> sample_vect ; 
		for (int i=0 ; i< n ; i++) sample_vect.push_back(1) ; 
		sample_amt = sample_vect ; 

	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<float> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	void set_CPT(vector<float> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }
	void increment_index(int ind){
		sample_amt[ind]++ ; 
		total_samples++ ;

	}
	vector<float> get_sample_CPT()
	{
		vector<float> learn_cpt ; 
		for (int i=0 ; i < nvalues ; i++) learn_cpt.push_back((float)sample_amt[i]/total_samples) ;
		return learn_cpt;
	}




};


 // The whole network represted as a list of nodes
class network{

	list <Graph_Node> Pres_Graph;

public:
	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}
    
    
	int netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int get_index(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return count;
            count++;
        }
        return -1;
    }
// get the node at nth index
    list<Graph_Node>::iterator get_nth_node(int n)
    {
       list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(count==n)
                return listIt;
            count++;
        }
        return listIt; 
    }
    //get the iterator of a node with a given name
    list<Graph_Node>::iterator search_node(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return listIt;
        }
    
            cout<<"node not found\n";
        return listIt;
    }
	

};

network read_network()
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile("alarm.bif"); 
  	string temp;
  	string name;
  	vector<string> values;
  	
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		
      		ss.str(line);
     		ss>>temp;
     		
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				int pos=Alarm.addNode(new_node);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;
     				
                    list<Graph_Node>::iterator listIt;
                    list<Graph_Node>::iterator listIt1;
     				listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        listIt1=Alarm.search_node(temp);
                        listIt1->add_child(index);
     					values.push_back(temp);
     					
     					ss>>temp;

    				}
                    listIt->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
     				vector<float> curr_CPT;
                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{
                        
     					curr_CPT.push_back(atof(temp.c_str()));
     					
     					ss2>>temp;
                       
                        

    				}
                    
                    listIt->set_CPT(curr_CPT);


     		}
            else
            {
                
            }
     		
     		

    		
    		
    	}
    	
    	if(find==1)
    	myfile.close();
  	}
  	
  	return Alarm;
}

vector<vector<string>> parse_data(string filename){
	
	vector<vector<string>> parsed_data ; 

	string myText;
	// Read from the text file
	ifstream MyReadFile(filename);
	// Use a while loop together with the getline() function to read the file line by line
	while (getline (MyReadFile, myText)) {
	stringstream ss(myText);
	vector<string> temp_vect ;
	string s ;
	while (getline(ss, s, ' ')) {
 
        // store token string in the vector
        temp_vect.push_back(s);
    }
	parsed_data.push_back(temp_vect) ;
	// Output the text from the file
	
	}

	// Close the file
	MyReadFile.close(); 
	return parsed_data ;
}

int main()
{
	network Alarm;
	Alarm=read_network();

	// cout<<data_vector[1][2]<<endl ;
    // int size_of_network = Alarm.netSize() ;
	// for (int i=0 ; i< size_of_network ; i++){
	// int len_values = Alarm.get_nth_node(i)->get_nvalues() ;
	// for (int j=0 ; j < len_values ; j++) cout<<Alarm.get_nth_node(i)->get_values()[j]<<" " ;  
	// cout<<endl ;
	// }

	
    // long long pow_2[31] ;
    // pow_2[0]=1ll ;  
    // for (int i=1 ; i< size_of_network ; i++){
    //     pow_2[i] = pow_2[i-1]*2 ;
    // }
	// cout<<size_of_network<<endl  ;
    // for (int i=0 ; i< size_of_network ;i++){
    //     // cout<<"size of parents "<<Alarm.get_nth_node(i)->get_Parents().size()<<"\n" ; 
    //     int predicted_size_cpt = 1  ; 
    //     for (int i =0 ; i< Alarm.get_nth_node(i)->get_Parents().size() ; i++){
    //         predicted_size_cpt *=Alarm.search_node(Alarm.get_nth_node(i)->get_Parents()[i])->get_nvalues() ;
    //     }
    //     predicted_size_cpt *= Alarm.get_nth_node(i)->get_nvalues() ;
    //     cout<<(predicted_size_cpt==Alarm.get_nth_node(i)->get_CPT().size())<<endl;
    //     cout<<"size of cpt "<<Alarm.get_nth_node(i)->get_CPT().size()<<"\n" ;
    //     cout<<"size of n_values "<<Alarm.get_nth_node(i)->get_nvalues()<<"\n" ;

    //     // cout<<(pow_2[Alarm.get_nth_node(i)->get_Parents().size()] == Alarm.get_nth_node(i)->get_CPT().size())<<endl ;
    // }


	// cout<<Alarm.get_nth_node(0)->get_CPT()[0]<<endl ;
	// while()
	// starting the code from here 
	vector<vector<string>> data_vector = parse_data("records.dat") ;
	// data_vector[0]
	// cout<<Alarm.get_nth_node(0)->get_name() ;
	// cout<<Alarm.get_index(Alarm.get_nth_node(0)->get_name()) ;

    
    
// Example: to do something
    // cout<<Alarm.netSize()<<"\n" ;
	cout<<"Perfect! Hurrah! \n";
	
}


