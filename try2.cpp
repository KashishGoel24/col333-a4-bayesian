#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unordered_map> 

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

public:
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
	{
		Node_Name=name;
	
		nvalues=n;
		values=vals;
		

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

network read_network(string filename)
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile(filename); 
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
					// std::cout<<name<<std::endl;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
					// std::cout<<"pritning\n";
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
						
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
						// std::cout<<temp<<std::endl;
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

vector<float> find_probability_given_all(list<Graph_Node>::iterator X , unordered_map<string,string> given_values , network* Alarm  ){ // given all ?
    // this is just the product probability of X given parents and parents of its children given parents  
    cout<<"Here it works"<<endl;
    vector<string>  parents_X = X->get_Parents() ; // this is all the parents
    int parents_size = parents_X.size() ; 
    vector<int> parents_index ; // stores what indices values do the parents value are
    vector<int> nvalues_parents ; // total number of n values of parents 
    vector<float> cpt_X = X->get_CPT()  ; 
    vector<float> x_prob_table ; // this is to be ultimately returned

    for (int i=0 ; i < parents_size ; i++){
        // this loop fills in the parents_index and nvalues_parents 
        string parent = parents_X[i] ; 
        string val_parent   = given_values[parent]  ; // this is the given value of parent
        list<Graph_Node>::iterator parent_node =  Alarm->get_nth_node(Alarm->get_index(parent)) ; // this is the parent node
        vector<string> all_possible_values = parent_node->get_values() ; 
        int ind = -1 ;
        for (int i=0 ; i < all_possible_values.size() ; i++){
            if (val_parent == all_possible_values[i]) {
                ind = i  ;
                break ; }
        }
        parents_index.push_back(ind) ;
        nvalues_parents.push_back(parent_node->get_nvalues()) ;
    
    }
    cout<<"Here it works 98"<<endl;

    int index_in_cpt = 0 ; // this is the index in cpt that is to be used
    int running_product = 1 ; 
    for (int i= parents_size-1 ; i >=0 ; i--){
        index_in_cpt += parents_index[i]*running_product ; 
        running_product *= nvalues_parents[i] ;
    }
    cout<<"Here it works 34"<<endl;

    vector<int> children_index = X->get_children() ; // this is the indices of the children
    int number_childrens  = children_index.size() ; 


    int nvalues_x = X->get_nvalues() ;
    string x_name = X->get_name() ;   
    cout<<"Here it works 17"<<endl;

    for (int value_x=0 ; value_x < nvalues_x ; value_x++){
        // this is for a given value of x  
        float prob_x_spec = cpt_X[index_in_cpt*nvalues_x + value_x] ; // this is all the data from the parents 
        // cout<<prob_x_spec
        cout<<"Here it works 21"<<endl;

        for (int i=0 ; i < number_childrens ; i++){
            // this is for a given child
            list<Graph_Node>::iterator child = Alarm->get_nth_node(children_index[i]) ; // this gives the correct linked list node 
            vector<string> childs_parents =  child->get_Parents() ; // this is the vector of parents of the given child
            int child_parents_size = childs_parents.size() ; // this is the number of parents of a child
            vector<int> child_parent_value_indices  ; 
            vector<int> nvalues_child_parents ;
            cout<<"Here it works 7"<<endl;
            
            for (int j=0 ; j < child_parents_size ; j++){
                // this is just for filling the indices
                if (childs_parents[j]==x_name) {
                    child_parent_value_indices.push_back(i) ;
                    nvalues_child_parents.push_back(nvalues_x) ;
                    }
                else { 
                    string child_parent = childs_parents[j] ;
                    string val_child_parent = given_values[child_parent] ;
                    
                    list<Graph_Node>::iterator child_parent_node =  Alarm->get_nth_node(Alarm->get_index(child_parent)) ;
                    vector<string> all_possible_values_child_parent = child_parent_node->get_values() ;
                    int ind = -1 ;
                    for (int rnd_cnt=0 ; rnd_cnt < all_possible_values_child_parent.size() ; rnd_cnt++){
                        if (val_child_parent == all_possible_values_child_parent[rnd_cnt]) {
                            ind = rnd_cnt;
                            break ; }
                    }
                    child_parent_value_indices.push_back(ind) ; 
                    nvalues_child_parents.push_back(child_parent_node->get_nvalues()) ;

                }
            }
            cout<<"Here it works 8"<<endl;

            // now we find which value from the child cpt to take  
            // the index to be used is as such 
            int running_stuff_mult = 1 ; 
            int index_in_child_cpt = -1  ;
            string name_child = child->get_name() ; 
            string val_child = given_values[name_child] ;
            vector<string> value_children = child->get_values() ;
            cout<<"Here it works 9"<<endl;
            cout<<"The value to be found is :"<<val_child<<endl ;
            for (int it = 0 ; it < value_children.size() ; it++){
                cout<<value_children[it]<<endl ;
                if (val_child == value_children[it]){
                    index_in_child_cpt = it ; 
                    break ; 
                }
            }
            cout<<index_in_child_cpt<<endl ;
            cout<<"Here it works 10"<<endl;

            for (int cnt_1  = child_parents_size ; child_parents_size>=0 ;cnt_1-- ){
                index_in_child_cpt+= running_stuff_mult*child_parent_value_indices[cnt_1] ;
                running_stuff_mult*= nvalues_child_parents[cnt_1] ;
            }
            vector<float> child_cpt = child->get_CPT() ;
            prob_x_spec *= child_cpt[index_in_child_cpt] ;
            cout<<"Here it works 11"<<endl;

            

            


        }

        x_prob_table.push_back(prob_x_spec) ;
    }

    // now normalize the prob table 
    float norm_cnst = 0 ; 
    for (int i=0 ; i< x_prob_table.size() ; i++) norm_cnst+= x_prob_table[i] ;
    for (int i=0 ; i<x_prob_table.size() ; i++) x_prob_table[i] = x_prob_table[i]/norm_cnst ;
    return x_prob_table ;
    // cpt_X[inde]
    // <1,2,0,3>
    // X.get_CPT();
}

int main()
{
	network Alarm;
	Alarm=read_network("small_alarm.bif");
    vector<vector<string>> data  = parse_data("small_records.dat"); 


    //isme data[0] ki jagah data[i] kardena
    vector<string> data_row = data[0]; 
    unordered_map<string,string> value_row;
    list<Graph_Node>::iterator X_node = Alarm.get_nth_node(0); 
    for (int i=0 ; i < data_row.size() ; i ++){
        cout<<Alarm.get_nth_node(i)->get_name()<<" "<<data_row[i]<<endl;
        value_row[Alarm.get_nth_node(i)->get_name()] = data_row[i]; 
        if (data_row[i]=="\"?\"") X_node = Alarm.get_nth_node(i); 
    }
    cout<<X_node->get_name() ;
    // cout<<value_row["\"A\""]<<endl ;
    vector<float> prob_table_x = find_probability_given_all(X_node,value_row, &Alarm) ;
    cout<<prob_table_x[0]<<" "<<prob_table_x[1]<<endl ;
    cout<<data_row[1]<<endl  ;
	cout<<"Okay, that's all! \n";

}