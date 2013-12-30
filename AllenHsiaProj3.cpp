/*
 * AllenHsiaProj3.cpp
 *
 *  Created on: Mar 5, 2013
 *      Author: allen
 */
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <iterator>
#include <array>
#include <map>
#include <vector>
#include <sstream>
#include <ostream>
#include <fstream>
using namespace std;
int nodeCount = 0;
struct expr
{
	expr* LNode;
	expr* RNode;
	string type;
	string oper;
	//expr(string T, string O):type(T),oper(O){++nodeCount;};
	//expr(string T):type(T),oper(""){++nodeCount;};
	//expr():type(""),oper(""){++nodeCount;};
	expr(string T, string O):LNode(nullptr),RNode(nullptr),type(T),oper(O){++nodeCount;};
	expr(string T):type(T),LNode(nullptr),RNode(nullptr),oper(""){++nodeCount;};
	expr():type(""),LNode(nullptr),RNode(nullptr),oper(""){++nodeCount;};
	~expr(){
		delete RNode;
		delete LNode;
		
	};
};

vector<string> splitting(string tokenString)
{
	vector<string> splittedResult;
	splittedResult.reserve(2);
	int spacePos = tokenString.find(" ");
	int stringSize = tokenString.size();
	//finding if it's an array. if not then continue
	string isArray = tokenString.substr(0,spacePos);
	int found = 0;
	found = isArray.find_first_of("[");
	if(found != string::npos)		//if there is no empty, meaning if there is a [ found.
	{
		//cout << "There is a [ found"<<endl;
		splittedResult.push_back(tokenString.substr(0, found));	//reading the int for int a
		splittedResult.push_back(tokenString.substr(spacePos+1,stringSize));	//reading the a for int a
	}
	//
	splittedResult.push_back(tokenString.substr(0,spacePos));	//reading the int for int a
	splittedResult.push_back(tokenString.substr(spacePos+1,stringSize));	//reading the a for int a
	return splittedResult;
}

string findType(map<string,string> ttypes,string thingToCheck)
{
	string result = "";
	result = ttypes.find(thingToCheck)->second;
	return result;
}
				//map						equ line in a vector, leaf,		head,		counter/index through vector
expr* makingTree(map<string,string> ttypes,vector<string> tokens, expr *leaf, expr *head, int i)
{
	  if(i < tokens.size())
	  {
		if(i + 1 < tokens.size()) //if index is not at the end. e.g. a = b + c , c won't have anything after it
		{
			//cout <<tokens[i]<< " " << ttypes.find(tokens[i])->second<<" hello "<<endl;
			//leaf->LNode = new expr(ttypes.find(tokens[i])->second, tokens[i+1]); //creates left leaf with one type and operand
			leaf->LNode = new expr(findType(ttypes,tokens[i]), tokens[i+1]); //creates left leaf with one type and operand
			expr *e = new expr();
			leaf->RNode = e; //initializes right leaf
			return makingTree(ttypes, tokens, leaf->RNode, head, i+2);
		}
		else
		{
				leaf->type = findType(ttypes,(tokens[i]));
				return head;
		}
	  }
	else
	{
		return head; //should never be reached, just a precaution
	}

}
string compare(string type1, string type2){
    if(type1 == type2)
        return type1;
    else if((type1 == "int" && type2 == "double") || (type1 == "double" && type2 == "int")){
        return "double";
    }
    else{
        return "";
    }
}
string compare2(string type1, string type2){
    if(type1 == type2)
        return type1;
    else if((type1 == "double" && type2 == "int")){
        return "double";
    }
    else{
        return "";
    }
}
string TreeTraversal(expr* head, bool isCond, string prevType)
{
    if(prevType == "")
    {
        prevType = head->LNode->type;
    }
    else{
        string result = compare(prevType, head->LNode->type);
        if(result != ""){
            prevType = result;
        }
        else{
            printf("ERROR 2: Type mismatch in expression");
            return "";
        }
    }
    if(head->LNode->oper == ">" || head->LNode->oper == "<" || head->LNode->oper == ">=" || head->LNode->oper == "<=" || head->LNode->oper == "==")
    {
        isCond = true;
    }
    if(head->RNode!= NULL)
    {
        if(head->RNode->type == "")
            return TreeTraversal(head->RNode, isCond, prevType);
        else
        {
            string result = compare(prevType, head->RNode->type);
            if(result != ""){
                if(isCond){
                    return "bool";
                }
                else
                    return result;
            }

            else{
                printf("ERROR 2: Type mismatch in expression");
                return "";
            }
        }
    }
    else{
            return "";
    }
}



vector<string> parseLine(string tokenString)
{
	vector<string> result;
	unsigned int curPos = 0;
	unsigned int spacePos = 0;
	unsigned int brackPos = 0;
	int temp = 0;
	while(curPos< tokenString.size())		//while current position is less than total size
	{

		spacePos = tokenString.find(" ", curPos);//find first space
		temp = spacePos;
		if(temp != string::npos)		//if there is no empty, meaning if there is a " " found.
		{
			brackPos = tokenString.find("[",curPos);//finding the [
			if(brackPos<spacePos)
			{
				result.push_back(tokenString.substr(curPos,brackPos-curPos));
				//cout << "curPos = " << curPos << " bracketPosition "<< brackPos<<endl;
			}
			else									//did not find a [
			{	result.push_back(tokenString.substr(curPos,spacePos-curPos));
				//cout << "curPos = " << curPos << " spacePos "<< spacePos<<endl;
			}
			curPos = spacePos+1;
		}
		else
		{
			brackPos = tokenString.find("[",curPos);//finding the [
			if(brackPos<spacePos)
			{
				result.push_back(tokenString.substr(curPos,brackPos-curPos));
				//cout << "curPos = " << curPos << " bracketPosition "<< brackPos<< " else"<<endl;
			}
			else									//did not find a [
				{
				result.push_back(tokenString.substr(curPos,tokenString.size()-curPos));
				//cout << "curPos = " << curPos << " tokenString"<< tokenString.size()<< " else" <<endl;
				}
			break;
		}
		//cout << curPos<<endl;
	}
	return result;
}
				//map 						the expression equation
string checkingType (map<string,string> ttypes, string tokenString)
{

	string result;
	vector <string> tokens;
	tokens = parseLine(tokenString);
	cout << "\n";
	for(int i = 0; i<tokens.size(); i++)
	{
		if(ttypes.find(tokens[i])==ttypes.end())
		{
			cout<< "ERROR 1: Undeclared identifier in expression";
			return "";
		}
		i++;
	}
	//vector<string> testVec;
	/*testVec.push_back("a");
	testVec.push_back("=");
	testVec.push_back("b");
	testVec.push_back("-");
	testVec.push_back("allen");
	testVec.push_back("/");
	testVec.push_back("greg");
	testVec.push_back("+");
	testVec.push_back("morethings");*/
	expr *tree = new expr();
	makingTree(ttypes,tokens,tree,tree,0);
	string firstType = tree->LNode->type;
	bool isCond = false;
	if(tree->RNode != NULL){
	    if(tree->RNode->type == "")
	    {
	        result = TreeTraversal(tree->RNode, isCond, "");
	        if(result != ""){
	            result = compare2(firstType, result);
	            if(result != ""){
	                printf("%s", result.data());
	            }
	            else{
	                printf("ERROR 3: Type mismatch in assignment");
	            }
	        }
	    }
	    else{ //tree is just three nodes (e.g   x = y;)
	        result = compare2(firstType, tree->RNode->type);
	        if(result != ""){
	            printf("%s", result.data());
	        }
	        else{
	            //printf("Type mismatch in assignment\n");
	        }
	    }
	}
	else{
	    //Tree not initialized correctly
	}
	//cout << "1"<<endl;
	delete tree;
	//printf("%d", nodeCount);
	//result = TreeTraversal(e,temp);
	//result = EvaluationTree(e,temp);
	//cout << e->LNode->type;
	//cout << e->RNode->LNode->type;
	//cout << e->RNode->RNode->type;
	//result = EvaluationTree(e);
	return result;
}

int main()
{
	map<string, string>ttypes;
	string tokenString;
	vector<string> stuffToAdd;
	getline(cin,tokenString,'\n');
	while(!tokenString.empty())			//read in the types in the map
	{
		stuffToAdd = splitting(tokenString);
		string first = stuffToAdd[0];
		string second = stuffToAdd[1];
		ttypes[second] = first;
		getline(cin,tokenString,'\n');
	}
	for(map<string,string>::iterator ii = ttypes.begin() ; ii != ttypes.end(); ++ii)
	{
		cout << ii->first<< " " << ii->second << endl;
	}
	//cout << "\n";
	//traverse through the map
	getline(cin,tokenString,'\n');

	string resultt = "";
	while(!tokenString.empty())
	{
		resultt = checkingType(ttypes, tokenString);
		//cout << resultt;
		getline(cin,tokenString, '\n');
	}
	cout << "\n";
}
