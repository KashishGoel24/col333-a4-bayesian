#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unordered_map> 
#include <ctime>
#include <chrono>
#include <algorithm>
#include <random>
using namespace std::chrono;
// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;
#define vec_print(v) {cout<<#v<<" : " ;for(int rnd_i = 0 ; rnd_i < v.size() ; rnd_i ++) {cout<<v[rnd_i]<<" " ;} cout<<endl  ;}
#define vec_print_no_name(v) {for(int rnd_i = 0 ; rnd_i < v.size() ; rnd_i ++) {cout<<v[rnd_i]<<" " ;} cout<<endl  ;}
#define what_is(x) {cout<<#x<<" is "<<x<<endl ;}
#define check_here(x) {cout<<"Here it works "<<x<<endl ;}

bool comments = false ;
bool display = true ; 
bool testing  = false ;
bool useless_comments = true;

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

private:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning
    vector<float> sum_cpt ;
    float n_const ;

public:
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
	{   
		Node_Name=name;
		nvalues=n;
		values=vals;
	}

    Graph_Node* getPointer() {
        return this;
    }

    void set_sum_cpt(){
        n_const = CPT.size() ;
        for (int i=0 ; i<CPT.size() ; i++) sum_cpt.push_back(0.001) ;
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
        Children.push_back(new_child_index);
        return 1;
    }
    void update_sum_cpt(int index_change, float val_change  ){
        sum_cpt[index_change]+=val_change ;
    }

    void rewrite_cpt(){
        int prod_except_me = CPT.size()/nvalues ; 
        vector<float> vector_norm  ; 
        for (int i=0; i < prod_except_me ; i++){
            float norm_const  = 0 ; 
            for (int j=0 ; j < nvalues ; j++){
                norm_const += sum_cpt[j*prod_except_me + i] ;
            }
            vector_norm.push_back(norm_const) ;
        }
        for (int i=0 ; i < sum_cpt.size() ; i++) CPT[i] = sum_cpt[i]/vector_norm[i%prod_except_me] ;
    }
};
 // The whole network represted as a list of nodes

class network{

    vector<Graph_Node> Pres_Graph;

public:
    unordered_map<string,int> name_to_index; // 37 
    vector<unordered_map<string,int> > value_to_index_for_nodes; // vector ka size 37 hai 
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
        return name_to_index[val_name];
    }
// get the node at nth index
    Graph_Node* get_nth_node(int n)
    {
       return Pres_Graph[n].getPointer(); 
    }
    //get the iterator of a node with a given name
    Graph_Node* search_node(string val_name)
    {
        int index = get_index(val_name);
        return Pres_Graph[index].getPointer();
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
        int count = 0;
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
                    int count_values = 0;
                    unordered_map<string,int> value_mapping_to_index;
     				while(temp.compare("};")!=0)
     				{
						// std::cout<<temp<<std::endl;
     					values.push_back(temp);
     					value_mapping_to_index[temp] = count_values; 
     					ss2>>temp;
                        count_values += 1;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				int pos=Alarm.addNode(new_node);
                    Alarm.name_to_index[name] = count;
                    count += 1;
                    Alarm.value_to_index_for_nodes.push_back(value_mapping_to_index);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;

     				Graph_Node* listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        Graph_Node* listIt1=Alarm.search_node(temp);
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
                    listIt->set_sum_cpt() ;


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

vector<vector<int> > parse_data(string filename, network* Alarm ){
	
	vector<vector<int> > parsed_data ; 

	string myText;
	// Read from the text file
	ifstream MyReadFile(filename);
	// Use a while loop together with the getline() function to read the file line by line
	while (getline (MyReadFile, myText)) {
	stringstream ss(myText);
	vector<int> temp_vect ;
	string s ;
    int count = 0;
	while (getline(ss, s, ' ')) {
 
        // store token string in the vector
        if (s == "\"?\""){
            temp_vect.push_back(-1);
        }
        else{
            temp_vect.push_back(Alarm->value_to_index_for_nodes[count][s]);
        }
        count += 1;
    }
	parsed_data.push_back(temp_vect) ;
	// Output the text from the file
	
	}

	// Close the file
	MyReadFile.close(); 
	return parsed_data ;
}

vector<float> find_probability_given_all(Graph_Node* X , unordered_map<string,int> given_values , network* Alarm  ){ // given all ?
    // this is just the product probability of X given parents and parents of its children given parents  
    vector<string>  parents_X = X->get_Parents() ; // this is all the parents
    int parents_size = parents_X.size() ; 
    vector<int> parents_index ; // stores what indices values do the parents value are
    vector<int> nvalues_parents ; // total number of n values of parents 
    vector<float> cpt_X = X->get_CPT()  ; 
    vector<float> x_prob_table ; // this is to be ultimately returned
    for (int i=0 ; i < parents_size ; i++){
        // this loop fills in the parents_index and nvalues_parents 
        string parent = parents_X[i] ; 
        int val_parent   = given_values[parent]  ; // this is the given value of parent
        Graph_Node* parent_node =  Alarm->get_nth_node(Alarm->get_index(parent)) ; // this is the parent node
        parents_index.push_back(val_parent) ;
        nvalues_parents.push_back(parent_node->get_nvalues()) ;
    }
    int index_in_cpt = 0 ; // this is the index in cpt that is to be used
    int running_product = 1 ; 
    for (int i= parents_size-1 ; i >=0 ; i--){
        index_in_cpt += parents_index[i]*running_product ; 
        running_product *= nvalues_parents[i] ;
    }
    vector<int> children_index = X->get_children() ; // this is the indices of the children
    int number_childrens  = children_index.size() ; 
    int nvalues_x = X->get_nvalues() ;
    string x_name = X->get_name() ;   

    for (int value_x=0 ; value_x < nvalues_x ; value_x++){
        // this is for a given value of x  
        float prob_x_spec = cpt_X[index_in_cpt +running_product* value_x] ; // this is all the data from the parents 
        for (int i=0 ; i < number_childrens ; i++){
            Graph_Node* child = Alarm->get_nth_node(children_index[i]) ; // this gives the correct linked list node 
            vector<string> childs_parents =  child->get_Parents() ; // this is the vector of parents of the given child
            int child_parents_size = childs_parents.size() ; // this is the number of parents of a child
            vector<int> child_parent_value_indices  ; 
            vector<int> nvalues_child_parents ;
            
            for (int j=0 ; j < child_parents_size ; j++){
                if (childs_parents[j]==x_name) {
                    child_parent_value_indices.push_back(value_x) ;
                    nvalues_child_parents.push_back(nvalues_x) ;
                    }
                else { 
                    string child_parent = childs_parents[j] ;
                    int val_child_parent = given_values[child_parent] ;
                    Graph_Node* child_parent_node =  Alarm->get_nth_node(Alarm->get_index(child_parent)) ;
                    child_parent_value_indices.push_back(val_child_parent) ; 
                    nvalues_child_parents.push_back(child_parent_node->get_nvalues()) ;

                }
            }
            int running_stuff_mult = 1 ; 
            int index_in_child_cpt = -1  ;
            string name_child = child->get_name() ; 
            int val_child = given_values[name_child] ;
            vector<string> value_children = child->get_values() ;
            index_in_child_cpt = val_child;
            int index_random  = 0 ;
            for (int cnt_1  = child_parents_size-1 ; cnt_1>=0 ;cnt_1-- ){
                index_random += running_stuff_mult*child_parent_value_indices[cnt_1] ;
                running_stuff_mult*= nvalues_child_parents[cnt_1] ;
            }

            vector<float> child_cpt = child->get_CPT() ;
            prob_x_spec *= child_cpt[index_in_child_cpt*running_stuff_mult +index_random] ;

        }

        x_prob_table.push_back(prob_x_spec) ;
    }

    // now normalize the prob table 
    float norm_cnst = 0 ; 
    for (int i=0 ; i< x_prob_table.size() ; i++) norm_cnst+= x_prob_table[i] ;
    for (int i=0 ; i<x_prob_table.size() ; i++) x_prob_table[i] = x_prob_table[i]/norm_cnst ;
    return x_prob_table ;
}

void update_cpt(float sample_weight , network* Alarm, unordered_map<string,int> given_values , bool rewrite_now){
    int size_net = Alarm->netSize() ;
    for (int node_number =0 ; node_number < size_net ; node_number++){
        Graph_Node* X_update = Alarm->get_nth_node(node_number);
        int number_n_values_x_update = X_update->get_nvalues();
        vector<string> n_values_x_update = X_update->get_values();
        int index_val = -1;
        index_val = given_values[X_update->get_name()];
        vector<string> parents_x = X_update->get_Parents() ;

        int parents_size = parents_x.size() ; 
        vector<int> parents_index ; // stores what indices values do the parents value are
        vector<int> nvalues_parents ; // total number of n values of parents 
        for (int i=0 ; i < parents_size ; i++){
            // this loop fills in the parents_index and nvalues_parents 
            string parent = parents_x[i] ; 
            int val_parent   = given_values[parent]  ; // this is the given value of parent
            Graph_Node* parent_node =  Alarm->get_nth_node(Alarm->get_index(parent)) ; // this is the parent node
            // what_is(val_parent);
            // check_here("yeh waala val p[arent]");
            parents_index.push_back(val_parent) ;
            nvalues_parents.push_back(parent_node->get_nvalues()) ;
        
        }

        int index_in_cpt = 0 ; // this is the index in cpt that is to be used
        int running_product = 1 ; 
        for (int i= parents_size-1 ; i >=0 ; i--){
            index_in_cpt += parents_index[i]*running_product ; 
            running_product *= nvalues_parents[i] ;
        }
        int final_index  = index_in_cpt + running_product * index_val ; 
        int size_cpt = X_update->get_CPT().size() ;
        X_update->update_sum_cpt(final_index, sample_weight);
        if (rewrite_now) X_update->rewrite_cpt() ;

    }
}

void write_to_solved(string filename_read, network *Alarm, string filename_write){
    ofstream MyFile_write(filename_write);

    // Write to the file

    // Close the file
  	ifstream myfile_read(filename_read); 
    string line ;
    bool prev_probab ;
    int cnt=0 ;
    int net_size = Alarm->netSize() ;
    while(!myfile_read.eof()){
        getline (myfile_read,line) ;
        if (not prev_probab) MyFile_write<<line<<endl ;
        else {
            Graph_Node* node_cpt = Alarm->get_nth_node(cnt) ; 
            vector<float> intended_cpt = node_cpt->get_CPT();
            MyFile_write<<"\ttable " ;
            for (int i=0 ; i < intended_cpt.size() ; i++) MyFile_write<<intended_cpt[i]<<" " ;
            MyFile_write<<endl; 

            cnt++ ;
        }
        if (line.substr(0,11).compare("probability")==0) prev_probab = true;
        else prev_probab = false; 
    }
    MyFile_write.close();
}


float compute_score(network* Gold_Alarm, network* Alarm){
    int size_net =Alarm->netSize() ;
    float score =0 ;
    for (int i=0 ; i < size_net  ; i++){
        Graph_Node* alarm_node = Alarm->get_nth_node(i) ;
        Graph_Node* gold_node = Gold_Alarm->get_nth_node(i) ;
        vector<float> v_our = alarm_node->get_CPT() ;
        vector<float> v_gold = gold_node->get_CPT() ;
        for (int i=0 ; i < v_our.size() ; i++){
            score+= abs(v_our[i]-v_gold[i])  ;
        }

    }
    return score ;

}

int main()
{
	network Alarm;
	network Gold_Alarm;
    vector<vector<int> > data ;
	if (testing) Alarm=read_network("small_testcase.bif");
    if (testing)  data = parse_data("small_testcase.dat", &Alarm); 
    if (not testing)  Alarm=read_network("alarm.bif");
    if (not testing) data  = parse_data("records_no_q_mark.dat", &Alarm); 
    if (not testing) Gold_Alarm = read_network("gold_alarm.bif") ;

    //isme data[0] ki jagah data[i] kardena
    // auto start_time = std::chrono::system_clock::now();
    // auto end_time = std::chrono::system_clock::now();
    int number_iterations = 1 ;
    int total_cnt = 0 ;
    
    while(total_cnt < number_iterations){
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(data), std::end(data), rng);
    // data.random()
    total_cnt++;
    cout<<"iteration number going on is "<<total_cnt<<endl;
    for (int all_data = 0 ; all_data < data.size() ; all_data++){
        // cout<<all_data<<endl;
        vector<int> data_row = data[all_data];
        unordered_map<string,int> value_row;
        Graph_Node* X_node = Alarm.get_nth_node(0); 
        bool question_mark = false  ;
        for (int i=0 ; i < data_row.size() ; i ++){
            if (data_row[i]==-1) {
                X_node = Alarm.get_nth_node(i);
                question_mark = true  ; 
            }
            value_row[Alarm.get_nth_node(i)->get_name()] = data_row[i];
        }
        if (not question_mark){
            update_cpt(1,&Alarm,value_row, (all_data < 0.1*data.size() || (rand()%10 == 1))) ;
        }
        else{
        
        vector<float> prob_table_x = find_probability_given_all(X_node,value_row, &Alarm) ;
        int nvals_x= X_node->get_nvalues() ;

        for (int i=0  ; i < nvals_x ; i++){
            value_row[X_node->get_name()] = i ;
            // bool rewrite = ((rand() % ((total_cnt-1)*data.size() + all_data)) < sqrt((total_cnt)*data.size() + all_data) + 10) ;
            update_cpt(prob_table_x[i],&Alarm , value_row , (all_data < 0.1*data.size() || (rand()%10 == 1))) ;
            // update_cpt(prob_table_x[i],&Alarm , value_row , true) ;

        }
    }
    }
    }
    float score;
    score = compute_score(&Gold_Alarm,&Alarm) ;
    what_is(score) ;
    int net_size = Alarm.netSize() ;
    write_to_solved("alarm.bif", &Alarm, "solved_alarm.bif") ;
    
}
