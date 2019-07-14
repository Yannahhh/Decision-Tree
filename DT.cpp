#include<iostream>
#include<string>
#include <map>
#include <cstring>
#include <fstream>
#include <sstream>
#include<cmath>
#include<vector>
#include<queue>
using namespace std;
#define LENTH 10
#define PUNISHMENT 0.6
#define choose_way 0 
//int TRUE[10][10000] ={0};
map<int,int> mMap[LENTH-1];
map<int,int> True[9];


fstream  result1("E:\\files\\SMIE\\third_up\\ai\\lab4_Decision_Tree\\lab4_Decision_Tree\\result.csv",ios::out);
/**********************
�����洢��ֵ��
	�ý����ʲô���ԣ�attribute
	���ڵ�����ô����������ģ�arrive_value
	�����еĺ��ӷ�֧��vector <node*> childs
*********************/
struct node{
	int attribute;
	int arrive_value;
	int most_label;
	int level;
	int father_attribute;
	vector <node*> childs;
	node(){
		attribute = -1;
		arrive_value = -1;
		level = 0;
		most_label = 0;
		father_attribute = -1;
	}
};

/****************************
ȫ�ֵĳ�ʼ��������
	���ڵ㣺root��
	ÿһ�е�ֵ��line_values;
	һ���д���������ݣ�instance��
	���е����Ե�id��attribute_id
	ÿһ��������ʲô��֧��map_attribute_to_branch
********************************/
node* root;
vector<vector<int> > instance;
vector <int> attribute_id;
vector <int> branches[LENTH-1];
int TP = 0,TN = 0,FP=0,FN=0; 
/*******************************************************
���ݼ��㹫ʽ�������ÿ�����Ե�C45���������� 
*******************************************************/ 
int C4_5_or_ID3(vector <vector<int> > remain_instance, vector <int> remain_attribute,bool c45){
	double max = -1;
	int result = 10086;
    //cout<<"remain inst "<<remain_instance.size()<<endl;
   // cout<<"remain attr "<<remain_attribute.size()<<endl;
    if(remain_attribute.size()==1) return remain_attribute[0];
	for(int k = 0;k<remain_attribute.size();k++){
		int attr_id = remain_attribute[k];
		vector<int> branch = branches[attr_id];
    	double H_D_attr = 0;
    	double SplitInfo = 0;
    	int All_positive = 0;
    	double gain = 0;
    	for(int i = 0;i<branch.size();i++){
    		int Positive = 0;
    		int All = 0;
    		/*��ǰ��֧�µ�1�ĸ����м���*/
    		for(int j = 0;j<remain_instance.size();j++){
    			if(remain_instance[j][attr_id]==branch[i]){
    				if(remain_instance[j][LENTH-1] == 1){
    					Positive++;
    					All_positive++;
    				} 
    				All++;
    			}
    		}
    		if(All == 0) continue;
    		double 	p = (double)All/(double)remain_instance.size();
    		SplitInfo += -p*log2(p);
			p = (double)Positive/(double)All;
    		if(p == 1 || p ==0) continue;
    		H_D_attr += All*(-p*log2(p)-(1-p)*log2(1-p));
    	
    	}
    
    	double H_D_p = (double)All_positive/(double)remain_instance.size();
    	double H_D =  -H_D_p*log2(H_D_p)-(1-H_D_p)*log2(1-H_D_p);
    	
    	gain =H_D - H_D_attr/(double)remain_instance.size();
    	if(c45){
    		if(SplitInfo==0)
    			continue;
    		gain = gain/SplitInfo;
    	}
    		
    	if(gain > max){
    		max = gain;
    		result = attr_id;
    	}
	}
    return result;
}
/*******************************************************
����ʣ���ʵ����ʣ������ԣ��ҳ�����ʵ����ԣ�1-gini���
*******************************************************/ 
int Gini(vector <vector<int> > remain_instance, vector <int> remain_attribute){
	double max = 0;
	int result = 10086;
	for(int k = 0;k<remain_attribute.size();k++){
		int attr_id = remain_attribute[k];
		vector<int> branch = branches[attr_id];
    	double gini = 0;
    	for(int i = 0;i<branch.size();i++){
    		int Positive = 0;
    		int All = 0;
    		/*��ǰ��֧�µ�1�ĸ����м���*/
    		for(int j = 0;j<remain_instance.size();j++){
    			if(remain_instance[j][attr_id]==branch[i]){
    				if(remain_instance[j][LENTH-1] == 1) Positive++;
    				All++;
    			}
    		}
    		if(All == 0) continue;
    		double p = (double)Positive/(double)All;
    		gini += All*(1-p*p-(1-p)*(1-p));
    	}
    	//cout<<"���� "<< attr_id<<" ��gini "<<(double)gini/double(remain_instance.size())<<endl; 
    	gini = 1-(double)gini/double(remain_instance.size());
    	if(gini > max){
    		max = gini;
    		result = attr_id;
    	}
	}
    return result;

}

/*************************
�ж�����������label�Ƿ���ͬ�� 
********************************/
int All_is_same(vector <vector<int> > remain_instance){
	int cnt = 0;
	for(int i = 0;i<remain_instance.size();i++){
		if(remain_instance[i][LENTH-1] == 1) cnt++;
	}
	//cout<<"***  "<<remain_instance.size()<<" "<<cnt<<endl;
	if(cnt == remain_instance.size()) return 1;
	if(cnt == 0) return -1;
	return 0; 

}

int Most_Label(vector <vector<int> > remain_instance){
	int cnt = 0;
	for(int i = 0;i<remain_instance.size();i++){
		if(remain_instance[i][LENTH-1] == 1) cnt++;
	}
	if(cnt*2 > remain_instance.size()) return 1;
	return -1;
}
bool All_attri_same(vector <vector<int> > remain_instance, vector <int> remain_attribute){
	 for(int i = 0;i<remain_attribute.size();i++){
		 	int id = remain_attribute[i];
		 	int tmp = remain_instance[0][id];
			for(int j = 0;j<remain_instance.size();j++){
				if(remain_instance[j][id]!=tmp) return false;
			}
	 }
	 return true;
}
int count_most(vector <vector<int> > remain_instance){
	int Pos = 0;
	int Neg = 0;
	for(int i = 0;i<remain_instance.size();i++){
		if(remain_instance[i][LENTH-1] == 1) Pos++;
		else Neg++;
	}
	return (Pos<Neg)?Pos:Neg;
}
double BuildDecisionTree(node* current_node,vector <vector<int> > remain_instance, vector <int> remain_attribute,int level,int father_attribute,int way){
	int less_number = count_most(remain_instance);
	if(current_node == NULL) current_node = new node();
	
	current_node -> most_label = Most_Label(remain_instance);
	current_node -> level = level;
	current_node -> father_attribute = father_attribute;
	
	double tmp = less_number+PUNISHMENT;
		//�ݹ���ֹ���� 
	if(All_is_same(remain_instance)==1){
		current_node->attribute = 1;
		return tmp;
	}
	if(All_is_same(remain_instance)==-1){
		current_node->attribute = -1;
		return less_number+PUNISHMENT;
	}
	if(!remain_attribute.size()||All_attri_same(remain_instance,remain_attribute)){
		current_node -> attribute = Most_Label(remain_instance);
		return less_number+PUNISHMENT;
	}

	//������Ӧ�������Ӧ�ĸ��ڵ������
	int choose_attribute;
	if(way == 2)	//GIni
		choose_attribute = Gini(remain_instance,remain_attribute);
	if(way == 1)	//C45
		choose_attribute = C4_5_or_ID3(remain_instance,remain_attribute,true);
	if(way == 0) //ID3 
		choose_attribute = C4_5_or_ID3(remain_instance,remain_attribute,false);
	
	cout<<"ѡ������  "<< choose_attribute<<endl;
	current_node -> attribute = choose_attribute;
	//����ʣ�������
	vector <int> new_attribute;  
    vector <vector <int> > new_instance;  
    
    for(int i = 0;i<remain_attribute.size();i++){
    	if(remain_attribute[i]!=choose_attribute) new_attribute.push_back(remain_attribute[i]);
    }
    //���ݸ��׽ڵ�����Եķ�֧���������ӵķָ�,ÿ����֧�ݹ齨��
    vector<int> branch = branches[choose_attribute];
    
    double sum_of_childs_false = 0;
    for(int j = 0;j<branch.size();j++){
    	//cout<<"branch  "<<branch[j]<<endl;
    	for(int i = 0;i<remain_instance.size();i++){
    		if(remain_instance[i][choose_attribute]==branch[j]){
    			new_instance.push_back(remain_instance[i]);
    		}
    	}
    
    	//ĳ����֧û���˶�Ӧ�����ӣ��б�Ϊ�����ֵ�label ������������  ����Ϊû�����֧ 
    	if(new_instance.size() == 0){
    		continue;
    	}
    	else{
			node* new_node = new node();
	    	new_node -> father_attribute = current_node->attribute;
	    	new_node -> arrive_value = branch[j];
	    	sum_of_childs_false += BuildDecisionTree(new_node,new_instance,new_attribute,level+1,current_node->attribute,way);
    		current_node -> childs.push_back(new_node);
    		new_instance.erase(new_instance.begin(),new_instance.end());
		} 	
    }
    //��֧������ʸ� 
    if(sum_of_childs_false>=less_number+PUNISHMENT){
    	current_node->childs.erase(current_node->childs.begin(),current_node->childs.end());
    	sum_of_childs_false = less_number+PUNISHMENT;
    	current_node->attribute = current_node -> most_label;
    	cout<<"**************cut****************** "<<endl;
    }
    return sum_of_childs_false;
}
/******************************
�ж�ĳһ��������ֵ 
*************************************/ 
void Judge(node* p,vector<int> line_value){
	int attribute_id = p->attribute;
	int arrive_value = line_value[attribute_id];
	int result = 0;
	int i = 0;
	for(i = 0;i<p->childs.size();i++){
		int value = (p->childs[i])->arrive_value;
		if(value == arrive_value){
			Judge(p->childs[i],line_value);
			break;
		}
	}
	if(p->childs.size()==0) result = p->attribute;
	else if(i==p->childs.size())   result = p->most_label;
	else return;
	result1<<result<<endl;
	//if(result == 1 && line_value[LENTH-1] == 1) TP++;
    //else if(result == 1 && line_value[LENTH-1] == -1) FP++;
    //else if(result == -1 && line_value[LENTH-1]== 1) FN++;
    //else TN++;
} 
void FreeTree(node *p){  
    if (p == NULL)  
        return;  
    for (vector<node*>::iterator it = p->childs.begin(); it != p->childs.end(); it++){  
        FreeTree(*it);  
    }  
    delete p; 
} 


void PrintTree(node *p, int depth){  
    queue <node*> q; 
	if(p) q.push(p);
	depth = q.front()->level;
	while(!q.empty()){
		if(depth != q.front()->level){
			cout<<endl;
			depth = q.front()->level;
		}
		char ttmp = q.front()->father_attribute+'a';
		cout<<ttmp<<":";
		char tmp = q.front()->attribute+'a';
		if(!q.front()->childs.size()) cout<< q.front()->attribute<<"   ";
		else  cout<<tmp<<"   ";
		
		for(int i = 0;i<q.front()->childs.size();i++){
			q.push(q.front()->childs[i]);
		} 
		q.pop();
	}
	 
}

int main(){
	fstream  train("E:\\files\\SMIE\\third_up\\ai\\lab4_Decision_Tree\\lab4_Decision_Tree\\train.csv");
	fstream  valid("E:\\files\\SMIE\\third_up\\ai\\lab4_Decision_Tree\\lab4_Decision_Tree\\valid.csv");
	fstream  test("E:\\files\\SMIE\\third_up\\ai\\lab4_Decision_Tree\\lab4_Decision_Tree\\test.csv");
	int rows = 0;
	int cnt = 0;
	int All_True = 0;
	map <int,int>::iterator it;
	string a;
	//9���������θ����0-8
	
	for(int i = 0;i<LENTH-1;i++){
		attribute_id.push_back(i);
	}

	/***********************************************
	�������ݣ�
	mMap�����Ӧ9������
	mMap[i][tmp[i]] = 1����i�������г��ֹ���tmp[i]�����֧
	branches�洢��ÿһ�����Զ�Ӧ�ķ�֧��ֵ
	line_values:��ǰ��һ�е����ݣ�
	instance���洢�˵�row�е����ݾ���line_values -�������������Ƕ�ά����
	map_attribute_to_branch��ĳ������������Щ��֧�� -�������������Ƕ�ά����

	*********************************************************/ 
	while(getline(train,a)){	
		vector <int> line_values;
		rows++;
		stringstream ss;
		ss<<a;
		int tmp[10] = {0};
		char ttmp;
		for(int i=0; i<LENTH;i++){
			ss>>tmp[i];
			ss>>ttmp;
			line_values.push_back(tmp[i]);
		}
		instance.push_back(line_values) ;
		
		for(int i = 0;i<LENTH-1;i++){
			it = mMap[i].find(tmp[i]);
			if(it == mMap[i].end()){//��һ�γ���,�ӵ���֧��
				mMap[i][tmp[i]] = 1;
				branches[i].push_back(tmp[i]);
			}
		}
	} 
	cout<<"finish"<<endl;
	cout<<"finish"<<endl;
	root = new node();
	BuildDecisionTree(root,instance,attribute_id,0,-1,choose_way);
	PrintTree(root,0);
	cout<<"Build Tree finished"<<endl;
	
	while(getline(test,a)){	
		vector <int> line_values;
		stringstream ss;
		ss<<a;
		int tmp=0;
		char ttmp;
		for(int i=0; i<LENTH;i++){
			ss>>tmp;
			ss>>ttmp;
			line_values.push_back(tmp);
		}
		Judge(root,line_values);
	} 	
	double Accuracy = (double)(TP+TN)/(double)(TP+TN+FP+FN);
	double Recall = (double)(TP)/(double)(TP+FN);
	double Precision = (double)(TP)/(double)(TP+FP);
	double F1 = 2* Precision* Recall/(Recall+Precision) ;
	cout<<"�Ż�2�� "<<endl; 
	cout<<"TP: "<<TP<<endl;
	cout<<"FN: "<<FN<<endl;
	cout<<"TN: "<<TN<<endl;
	cout<<"FP: "<<FP<<endl;
	cout<<"Accuracy: "<<Accuracy<<endl;
	cout<<"Recall: "<<Recall<<endl;
	cout<<"Precision: "<<Precision<<endl;
	cout<<"F1: "<<F1<<endl;
	FreeTree(root);
	return 0;
}
