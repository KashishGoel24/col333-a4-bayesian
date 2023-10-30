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
    // vector<float> sum_cpt_initial;
    // int data_points_initial;
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
        // for(int i=0;i<Children.size();i++)
        // {
        //     if(Children[i]==new_child_index)
        //         return 0;
        // }
        Children.push_back(new_child_index);
        return 1;
    }
    void update_sum_cpt(int index_change, float val_change  ){
        if (comments) what_is(sum_cpt.size()) ;
        if (comments) what_is(index_change) ;

        sum_cpt[index_change]+=val_change ;
        if (comments) check_here("all gud ?")
        // n_const += val_change ; 
    }

    void rewrite_cpt(){
        // 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8
        int prod_except_me = CPT.size()/nvalues ; 
        vector<float> vector_norm  ; 
        // what_is(prod_except_me);
        for (int i=0; i < prod_except_me ; i++){
            float norm_const  = 0 ; 
            for (int j=0 ; j < nvalues ; j++){
                norm_const += sum_cpt[j*prod_except_me + i] ;
            }
            vector_norm.push_back(norm_const) ;
        }
        // vec_print(sum_cpt);
        // vec_print(vector_norm);
        // if (comments) cout<<Node_Name<<" hi\n";
        // if (comments) vec_print(CPT);
        // if (comments) vec_print(sum_cpt);
        // if (comments) vec_print(vector_norm);
        for (int i=0 ; i < sum_cpt.size() ; i++) CPT[i] = sum_cpt[i]/vector_norm[i%prod_except_me] ;
        if (comments) vec_print(CPT);
        if (comments) vec_print(sum_cpt);
    }
};
 // The whole network represted as a list of nodes
class network{

	// list <Graph_Node> Pres_Graph;
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
        // Graph_Node listIt;
        // int count=0;
        // for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        // {
        //     if(listIt->get_name().compare(val_name)==0)
        //         return count;
        //     count++;
        // }
        // return -1;
        // for (int i = 0; i < Pres_Graph.size(); i++){
        //     if (Pres_Graph[i].get_name() == val_name){
        //         return i;
        //     }
        // }
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
     				
                    // list<Graph_Node>::iterator listIt;
                    // list<Graph_Node>::iterator listIt1;
                    // Graph_Node listIt;
                    // Graph_Node listIt1;
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
                    // cout<<"printing the size of the cpt set "<<Alarm.Pres_Graph[index].get_CPT().size()<<endl;
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
        // int ind = -1;
        // for (int i = 0; i < Alarm->get_nth_node(count)->get_nvalues(); i++){
        //     if (s == Alarm->get_nth_node(count)->get_values()[i]) {ind = i; break;}
        // }
        // if (ind == -1){
        //     temp_vect.push_back(s);
        //     // cout<<s<<" "<<Alarm->get_nth_node(count)->get_name()<<endl;
        // }
        // else{
        //     temp_vect.push_back(to_string(ind));
        // }
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
    if (comments) cout<<"Here it works"<<endl;
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
        // vector<string> all_possible_values = parent_node->get_values() ; 
        // int ind = -1 ;
        // for (int ii=0 ; ii < all_possible_values.size() ; ii++){
        //     if (val_parent == all_possible_values[ii]) {
        //         ind = ii  ;
        //         break ; }
        // }
        // what_is(val_parent);
        // check_here("nhi yeh waala ");
        parents_index.push_back(val_parent) ;
        nvalues_parents.push_back(parent_node->get_nvalues()) ;
    
    }
    if (comments) cout<<"Here it works 98"<<endl;

    int index_in_cpt = 0 ; // this is the index in cpt that is to be used
    int running_product = 1 ; 
    for (int i= parents_size-1 ; i >=0 ; i--){
        index_in_cpt += parents_index[i]*running_product ; 
        running_product *= nvalues_parents[i] ;
    }
    if (comments) cout<<"Here it works 34"<<endl;
    if (comments) what_is(X->get_name());
    if (comments) what_is(X->get_Parents().size());
    vector<int> children_index = X->get_children() ; // this is the indices of the children
    int number_childrens  = children_index.size() ; 
    if (comments) what_is(number_childrens);

    int nvalues_x = X->get_nvalues() ;
    string x_name = X->get_name() ;   
    if (comments) cout<<"Here it works 17"<<endl;

    for (int value_x=0 ; value_x < nvalues_x ; value_x++){
        // this is for a given value of x  
        float prob_x_spec = cpt_X[index_in_cpt +running_product* value_x] ; // this is all the data from the parents 
        // cout<<prob_x_spec
        if (comments) cout<<"Here it works 21"<<endl;

        for (int i=0 ; i < number_childrens ; i++){
            // this is for a given child
            // if (useless_comments) cout<<i<<endl;
            Graph_Node* child = Alarm->get_nth_node(children_index[i]) ; // this gives the correct linked list node 
            vector<string> childs_parents =  child->get_Parents() ; // this is the vector of parents of the given child
            int child_parents_size = childs_parents.size() ; // this is the number of parents of a child
            vector<int> child_parent_value_indices  ; 
            vector<int> nvalues_child_parents ;
            if (comments) cout<<"Here it works 7"<<endl;
            
            for (int j=0 ; j < child_parents_size ; j++){
                // this is just for filling the indices
                if (childs_parents[j]==x_name) {
                    child_parent_value_indices.push_back(value_x) ;
                    nvalues_child_parents.push_back(nvalues_x) ;
                    }
                else { 
                    string child_parent = childs_parents[j] ;
                    int val_child_parent = given_values[child_parent] ;
                    
                    Graph_Node* child_parent_node =  Alarm->get_nth_node(Alarm->get_index(child_parent)) ;
                    // vector<string> all_possible_values_child_parent = child_parent_node->get_values() ;
                    // int ind = -1 ;
                    // for (int rnd_cnt=0 ; rnd_cnt < all_possible_values_child_parent.size() ; rnd_cnt++){
                    //     if (val_child_parent == all_possible_values_child_parent[rnd_cnt]) {
                    //         ind = rnd_cnt;
                    //         break ; }
                    // }
                    // child_parent_value_indices.push_back(ind) ; 
                    // what_is(val_child_parent);
                    child_parent_value_indices.push_back(val_child_parent) ; 
                    nvalues_child_parents.push_back(child_parent_node->get_nvalues()) ;

                }
            }
            if (comments) cout<<"Here it works 8"<<endl;

            // now we find which value from the child cpt to take  
            // the index to be used is as such 
            int running_stuff_mult = 1 ; 
            int index_in_child_cpt = -1  ;
            string name_child = child->get_name() ; 
            int val_child = given_values[name_child] ;
            vector<string> value_children = child->get_values() ;
            if (comments) cout<<name_child<<endl ;
            if (comments) cout<<"Here it works 9"<<endl;
            if (comments) cout<<"The value to be found is :"<<val_child<<endl ;
            // for (int it = 0 ; it < value_children.size() ; it++){
            //     if (comments) cout<<value_children[it]<<endl ;
            //     if (val_child == value_children[it]){
            //         index_in_child_cpt = it ; 
            //         break ; 
            //     }
            // }
            // what_is(val_child);
            index_in_child_cpt = val_child;
            if (comments) cout<<"index_in_child_cpt is "<<index_in_child_cpt<<endl ;
            if (comments) cout<<"Here it works 10"<<endl;
            if (comments) cout<<"parents size is "<<child_parents_size<<endl ;
            // cout<<
            int index_random  = 0 ;
            if (comments) vec_print(nvalues_child_parents) ;
            if (comments) vec_print(child_parent_value_indices);
            for (int cnt_1  = child_parents_size-1 ; cnt_1>=0 ;cnt_1-- ){
                index_random += running_stuff_mult*child_parent_value_indices[cnt_1] ;
                running_stuff_mult*= nvalues_child_parents[cnt_1] ;
            }
            if (comments) what_is(running_stuff_mult) ;

            if (comments) cout<<"Here it works 11"<<endl;

            vector<float> child_cpt = child->get_CPT() ;
            if (comments) what_is(index_in_child_cpt)
            if (comments) what_is(index_random);
            prob_x_spec *= child_cpt[index_in_child_cpt*running_stuff_mult +index_random] ;

            

            


        }

        x_prob_table.push_back(prob_x_spec) ;
    }

    // now normalize the prob table 
    if (comments) vec_print(x_prob_table) ;
    float norm_cnst = 0 ; 
    for (int i=0 ; i< x_prob_table.size() ; i++) norm_cnst+= x_prob_table[i] ;
    for (int i=0 ; i<x_prob_table.size() ; i++) x_prob_table[i] = x_prob_table[i]/norm_cnst ;
    if (comments) vec_print(x_prob_table) ;
    return x_prob_table ;
    // cpt_X[inde]
    // <1,2,0,3>
    // X.get_CPT();
}

void update_cpt(float sample_weight , network* Alarm, unordered_map<string,int> given_values){
    int size_net = Alarm->netSize() ;
    if (comments) what_is(size_net) ;
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
        if (comments) check_here(3)  ;

        int index_in_cpt = 0 ; // this is the index in cpt that is to be used
        int running_product = 1 ; 
        for (int i= parents_size-1 ; i >=0 ; i--){
            index_in_cpt += parents_index[i]*running_product ; 
            running_product *= nvalues_parents[i] ;
        }
        int final_index  = index_in_cpt + running_product * index_val ; 
        if (comments) check_here(4)  ;
        int size_cpt = X_update->get_CPT().size() ;
        if (comments) what_is(size_cpt) ;
        if (comments) what_is(final_index) ;
        X_update->update_sum_cpt(final_index, sample_weight);
        if (comments) check_here(5)  ;
        X_update->rewrite_cpt() ;
        if (comments) check_here("all nice ?")
        if (comments) vec_print(X_update->get_CPT());

    }
    if (comments) check_here(" am bahar ab ");
    if (comments) vec_print(Alarm->get_nth_node(1)->get_CPT());
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
    if (not testing) data  = parse_data("records.dat", &Alarm); 
    if (not testing) Gold_Alarm = read_network("gold_alarm.bif") ;

    //isme data[0] ki jagah data[i] kardena
    // auto start_time = std::chrono::system_clock::now();
    // auto end_time = std::chrono::system_clock::now();
    int number_iterations = 2 ;
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
        if (testing && comments) vec_print(data_row) ; 
        if (testing && comments) what_is(Alarm.netSize()) ;
        unordered_map<string,int> value_row;
        Graph_Node* X_node = Alarm.get_nth_node(0); 
        for (int i=0 ; i < data_row.size() ; i ++){
            if (data_row[i]==-1) {
                X_node = Alarm.get_nth_node(i);
            }
            value_row[Alarm.get_nth_node(i)->get_name()] = data_row[i];
        }
        if (comments) what_is(X_node->get_name()) ;
        vector<float> prob_table_x = find_probability_given_all(X_node,value_row, &Alarm) ;
        if (comments) check_here(1) ;
        int nvals_x= X_node->get_nvalues() ;
        for (int i=0  ; i < nvals_x ; i++){
            value_row[X_node->get_name()] = i ;
            update_cpt(prob_table_x[i],&Alarm , value_row) ;
            if (comments) check_here("all fine ? ")

        }
        if (comments) check_here("aur kuchch ? ") ;
    }
    if (comments) check_here("sab sahi ?")
    }
    if (comments) check_here("ab to rukja") ;

    // cout<<prob_table_x[0]<<" "<<prob_table_x[1]<<endl ;
    // vec_print(X_node->get_CPT()) ;
    // if (display) vec_print(prob_table_x) ;
    // if (comments) cout<<data_row[1]<<endl  ;
    float score;
    score = compute_score(&Gold_Alarm,&Alarm) ;
    if (comments) check_here("Ye khatam hai");
    what_is(score) ;
    int net_size = Alarm.netSize() ;
    // {for (int node_num  =0 ; node_num < net_size; node_num++){
    //     // vec_print(Alarm.get_nth_node(node_num)->get_CPT()) ;
    //     cout<<Alarm.get_nth_node(node_num)->get_name()<<" " ;
    //     vec_print_no_name(Alarm.get_nth_node(node_num)->get_CPT()) ;
    // }
	// if (comments) cout<<"Okay, that's all! \n";}
    
}
