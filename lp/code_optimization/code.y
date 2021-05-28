%{
    #include <stdio.h>
    #include <bits/stdc++.h>
    #include <string.h>
    using namespace std;

	extern FILE * yyin;

    extern "C"
    {
            int yyparse(void);
            int yylex(void);  
            int yywrap()
            {
                    return 1;
            }
    }
    void yyerror(char*);

	int no_of_instructions = 0;
	map<int,string> instList;				// instruction number -> instruction map
	map<int,vector<string> > basicBlock;	// Block number -> list of instruction in that block 
	set<int> leaders;						// set of instruction numbers which are leaders of block
	vector<vector<int> > bbadj;				// Flow graph of blocks as adjacancy list
%}

%token VAR NUM OP EQ RELOP IF GOTO NL
%union  {    
        	char str[20];
        };
%type<str> VAR NUM OP EQ RELOP IF GOTO
%type<str> instruction val line

%%

S:  line S          {
                    }
    | %empty        {
                        YYACCEPT;
                    }
    ;

line: instruction NL  {
						// Add 3 address instruction to instList
                        instList[no_of_instructions++] = $$;
                    }
    ;
instruction:  VAR EQ val OP val           {
                                            strcat($1," ");
                                            strcat($2," ");
                                            strcat($3," ");
                                            strcat($4," ");
                                            strcat($1,strcat($2,strcat($3,strcat($4,$5))));
                                            strcpy($$,$1);
                                        }
        |   VAR EQ val                  {
                                            strcat($1," ");
                                            strcat($2," ");
                                            strcat($1,strcat($2,$3));
                                            strcpy($$,$1);
                                        }
        |   GOTO NUM   					{
											sprintf($2, "%d", (atoi($2)-1));
                                            strcat($1," ");
                                            strcpy($$,strcat($1,$2));
                                        }
        |   IF val RELOP val GOTO NUM   {
											sprintf($6, "%d", (atoi($6)-1));
                                            strcat($1," ");
                                            strcat($2," ");
                                            strcat($3," ");
                                            strcat($4," ");
                                            strcat($5," ");
                                            strcat($5,$6);
                                            strcat($2,strcat($3,strcat($4,$5)));
                                            strcpy($$,strcat($1,$2));
                                        }
        ;

val:    VAR                             { 
                                            strcpy($$,$1);
                                        }
    |   NUM                             { 
                                            strcpy($$,$1);
                                        }
    ;


%%


//to create the flow graph from the basic blocks
void createFlowGraph()
{
	bbadj.resize(basicBlock.size());
	for(auto it=basicBlock.begin();it!=basicBlock.end();it++)//iterate over all basic blocks
	{
		int ind=-1;
		string lastStmt=it->second.back();//get the last stmt of the current block
		if((ind=lastStmt.rfind("goto"))!=-1)//if last stmt has a goto then there's an edge from the current block to the block which is the target of the goto
		{
			bbadj[it->first].push_back(stoi(lastStmt.substr(ind+5)));//using stoi on the number substring to get the target block number
			if(ind!=0 && it->first!=basicBlock.size()-1)
			{
				bbadj[it->first].push_back(it->first+1);
			}
		}
		else//else there's an edge from the current block to the next block
		{
			if(it->first!=basicBlock.size()-1)
				bbadj[it->first].push_back(it->first+1);
		}
	}
}

//print the contents of the adjacency list representing the flow graph
void displayFlowGraph()
{
	cout<<"Flow Graph:"<<endl;
	for(int i=0;i<bbadj.size();i++)
	{
		cout<<i<<": ";
		for(int j=0;j<bbadj[i].size();j++)
		{
			cout<<bbadj[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

void findLeaders()//leader : first stmt of each block
{
	leaders.insert(0);	// first statement is always a leader
	for(auto it = instList.begin();it!=instList.end();it++)	// loop over all instructions
	{
		int ind = it->second.rfind("goto");				
		if(ind == -1)									// if goto is not instruction go to next instruction
		{
			continue;
		}
		leaders.insert(stoi(it->second.substr(ind+5)));	// Target statement of goto block is leader
		auto next = it;
		next++;
		if(next != instList.end())
		{
			leaders.insert(next->first);				// Next instruction after goto instruction is leader 
		}
	}
}

//print the leaders
void displayLeaders()
{
	cout<<"Leaders: ";
	for(auto it=leaders.begin();it!=leaders.end();it++)
	{
		cout<<*it<<" ";
	}
	cout<<"\n";
}

// To Identify basic blocks based on leaders
void createBasicBlocks()
{
	map<int,int> line2Block;	// line -> block map
	int block_number =-1;
	for(auto it=instList.begin();it!=instList.end();it++)	// loop over all instructions
	{
		if(leaders.find(it->first)!=leaders.end())
		{
			block_number++;							// if statement is leader increment block number
		}
		line2Block[it->first]=block_number;
	}
	block_number=-1;
	for(auto it=instList.begin();it!=instList.end();it++)
	{
		if(leaders.find(it->first)!=leaders.end())
		{
			block_number++;
		}
		int ind=-1;
		string instr=it->second;
		if((ind=it->second.rfind("goto"))!=-1)	// if goto instruction
		{
			// change goto instruction: Target instruction number -> Target Block number
			instr=it->second.substr(0,ind+5) + to_string(line2Block[stoi(it->second.substr(ind+5))]);
		}
		basicBlock[block_number].push_back(instr);	// Add instruction to corresponding block
	}
}
void displayBasicBlocks()
{
	cout<<"Basic Blocks:"<<endl;
	for(auto it=basicBlock.begin();it!=basicBlock.end();it++)
	{
		cout<<"Block "<<it->first<<":"<<endl;
		for(int i=0;i<it->second.size();i++)
		{
			cout<<"\t"<<it->second[i]<<endl;
		}
		cout<<endl;
	}
}

//displaying the parsed instructions
void displayInst()
{
	cout<<"Input 3 Addrress Statements:"<<endl;
	for(auto it=instList.begin();it!=instList.end();it++)
	{
		cout<<it->first<<" : "<<it->second<<endl;
	}
	cout<<endl;
}

//function to check for loops in the flow graph
//using graph coloring : 0 : not visited
//						 1 : currently visting
//						 2 : completely visited
void dfsLoop(int i,vector<int> &visited,vector<int> &path)//path stores the currently traversed path
{
	visited[i] = 1;
	path.push_back(i);
	for(const int &next : bbadj[i])
	{
		if(visited[next] == 0)//if next node is not visited then visit it
		{
			dfsLoop(next,visited,path);
		}
		else if(visited[next] == 1)//else if we find a node while visiting it then we found a loop
		{
			cout<<"Found natural loop: ";
			path.push_back(next);
			int j = find(path.begin(),path.end(),next) - path.begin();
			int k = path.rend() - find(path.rbegin(),path.rend(),next);
			while(j < k)//the loop path will be between the first and last occurence of the `next` value
			{
				cout<<path[j]<<" ";
				j++;
			}
			cout<<endl;
			path.pop_back();
		}
	}
	visited[i] = 2;//set to visited
	path.pop_back();
}

//calls the dfsLoop function for each edge(as the graph might be disconnected)
void displayLoop()
{
	int n = bbadj.size();
	vector<int> visited(n,0);
	vector<int> path;
	for(int i=0;i<n;i++)
	{
		if(!visited[i])
		{
			dfsLoop(i,visited,path);
		}
	}
	cout<<endl;
}

//function to display the unreachable blocks
void displayDeadCode()
{
	int n = bbadj.size();
	vector<int> inDeg(n,0);
	for(int i=0;i<n;i++)//find the inDegree of all the nodes
	{
		for(const int &v : bbadj[i])
		{
			inDeg[v]++;
		}
	}
	queue<int> q;
	for(int i=0;i<n;i++)//if some node has inDegree as 0 then it is unreachable so add it to dead blocks
	{
		if(i != 0 && inDeg[i] == 0)
		{
			q.push(i);
		}
	}
	//now if some block is unreachable then all of its outgoing edges also have to be removed
	//as they can cause some unreachable block to have inDegree > 0
	//so start a BFS from the unreachable nodes and remove their outgoing edges
	//and if after removal the inDegree of its neighbor also reduces to 0 then push it in the queue
	vector<int> dead;
	while(!q.empty())
	{
		int t = q.front();
		q.pop();
		dead.push_back(t);
		for(const int &next : bbadj[t])//decrement the inDegree of the block connected by its outgoing edges
		{
			inDeg[next]--;
			if(next != 0 && inDeg[next] == 0)//push in the queue if its inDegree also reduces to 0
			{
				q.push(next);
			}
		}
	}
	cout<<"Dead blocks: ";
	for(const int &d : dead)
	{
		cout<<d<<" ";
	}
	cout<<endl;
}

void displayDominators()
{
	int n = bbadj.size();
	vector<set<int>> dom(n,set<int>());	// To store dominators
	
	dom[0].insert(0);
	vector<int> temp;
	for(int i=0;i<n;i++)
	{
		temp.push_back(i);
	}
	for(int i=1;i<n;i++)	// Initiate dominators of instruction other that first to uninversel set
	{
		dom[i].insert(temp.begin(),temp.end());
	}
	
	vector<vector<int> > preds(n,vector<int>());	// Calculate Immediate predecesors
	for(int i=0;i<n;i++)
	{
		for(const int &x:bbadj[i])
		{
			preds[x].push_back(i);
		}
	}
	
	bool flag=true;
	vector<bool> computed(n,false);
	/*
		algorithm used:
		// dominator of the start node is the start itself
		Dom(n0) = {n0}
		// for all other nodes, set all nodes as the dominators
		for each n in N - {n0}
			Dom(n) = N;
		// iteratively eliminate nodes that are not dominators
		while changes in any Dom(n)
			for each n in N - {n0}:
				Dom(n) = {n} union with intersection over Dom(p) for all p in pred(n)
	*/		
	while(flag)	// While changes occur
	{
		flag = false;
		for(int i=1;i<n;i++)
		{
			if(preds[i].size() == 0)
			{
				continue;
			}
			set<int> inter = dom[preds[i][0]];
			for(int j=0;j<preds[i].size();j++)	// Perform intersection with all immediate predecesors
			{
				set<int> out;
				set<int> second = dom[preds[i][j]];
				set_intersection(inter.begin(),inter.end(),second.begin(),second.end(),inserter(out,out.begin()));
				inter = out;
			}
			inter.insert(i);
			if(inter != dom[i])		// if changed set flag
			{
				flag = true;
			}
			dom[i] = inter;
			computed[i] = true;
		}
	}
	
	// Print Dominators
	cout<<"Dominators: "<<endl;;
	cout<<"Block 0: 0"<<endl;
	for(int i=1;i<n;i++)
	{
		cout<<"Block "<<i<<": ";
		if(!computed[i])
		{
			cout<<"No dominators!"<<endl;
			continue;
		}
		for(auto x:dom[i])
		{
			cout<<x<<" ";
		}
		cout<<endl;
	}
}

int main(int argc, char **argv)
{
	cout<<endl;
    yyin = fopen(argv[1], "r");
    yyparse();
    displayInst();
    cout<<"---------------------------------------------------------------------------\n";

	findLeaders();
	displayLeaders();
    cout<<"---------------------------------------------------------------------------\n";

	createBasicBlocks();
	displayBasicBlocks();
    cout<<"---------------------------------------------------------------------------\n";
	
	createFlowGraph();
	displayFlowGraph();
    cout<<"---------------------------------------------------------------------------\n";
	
	displayLoop();
    cout<<"---------------------------------------------------------------------------\n";
	
	displayDeadCode();
    cout<<"---------------------------------------------------------------------------\n";
	
	displayDominators();
    cout<<"---------------------------------------------------------------------------\n";
}

void yyerror(char *s)
{
    cout<<"*** Error ***"<<endl;
}