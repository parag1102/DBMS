#include<iostream>
#include<string.h>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

char line[500];											//global string in which a line from a file is read


char *ptr_to_entity_names[50];									//stores the pointers to the variable names
char *ptr_to_attribute_names[50];								//stores the pointers to the attribute names
char *ptr_to_relationship_names[50];								//stores the pointers to the relationship names
char *ptr_to_Entity_in_Relationship_names[50];							//stores the pointers to the Entity_in_Relationship names
char *ptr_to_Relationship_Attribute_names[50];							//stores the pointers to the Relationship_Attribute names

int count_for_names=0;										//stores number of entities	                        		
int count_for_anames=0;					                        		//stores number of attributes in entities
int count_for_rnames=0;										//stores number of relationships
int count_for_ernames=0;					                        	//stores number of entities in a relationship
int count_for_ranames=0;					                        	//stores number of relation attributes


struct Attribute
	{
		char *name;
		int primary,multivalued;
		Attribute *next;	
	};
		
struct Entity
	{
		int weak_entity; 		
		char *name;
		Attribute *attr;
		Entity *next;
	}*head_entity=NULL;

struct Entity_in_Relationship
	{
		char *name;
		int many,fullp;
		Entity_in_Relationship *next;	
	};

struct Relationship_Attribute
	{
		char *name;
		Relationship_Attribute *next;
	};

struct Relationship
	{
		int identifying;		
		char *name;
		Entity_in_Relationship *list;
		Relationship_Attribute *attr;
		int cardinality;
		Relationship *next;
	}*head_relationship=NULL;

struct Table
{
	string name;
	string data;
	Table *next;
}
*table_head = NULL;


void interpreting_file(char *file_name);							//function declarations
Table *get_table(string name);
Entity *get_entity(string name);
void save_in_file();
void convert();


void remove_spacestabs(char* line)
	{
		char arr1[100];
		int j=0;
		for(int i=0;line[i]!='\0'&&line[i]!='#';i++)          				//checking for the end of the line or the start of the comment
			{
				if(line[i]==' ' || line[i]=='\t')				//removing spaces and tabs
					continue;
				else
					{
						arr1[j]=line[i];                                //copying the contents of 'line' in 'arr1' without any spaces or tabs
						j++;
					}
			}
		arr1[j]='\0';									//making the character array 'arr1' a string
		if(arr1[0]=='\0')
			line[0]='\0';
		else
			strcpy(line,arr1);                                                	//now 'line' contains no spaces or tabs
	}




int main()
	{
		char file[50];
		cout<<"Enter the file name (write test_case_*.txt)"<<endl;				//length of the file name can be 50 characters long.
		cin.getline(file,50);								//That's why 50 is given as the second argument in 'getline'
		interpreting_file(file);							//calling the function 'interpreting_file'
		convert();
		save_in_file();
		return 0;
	}




void interpreting_file(char *file_name)
	{
								//declaration 'char line[500];' could also be given here but we have already made line a global array
		ifstream fin;
		fin.open(file_name,ios::in);        		//opening the requested file
		fin.getline(line,100);                          //instruction are of maximum length 100 characetrs .We can increase it if we wish to
		remove_spacestabs(line);
		
		
		while(strcmp(line,"/Entity")!=0)	                                       	//checking the lines before '/Entity'
			{
				if( (strcmp(line,"\0")==0) || (strcmp(line,"Entity")==0) )      //skipping 'Entity' keyword or any blank line
					{
						fin.getline(line,100);
						remove_spacestabs(line);
						continue;
					}		
				
				Entity *te = new Entity;					//te is a pointer to a temporary entity
				te->weak_entity=0;						//initializing the 'Entity' fields
				te->name=NULL;				
				te->attr=NULL;
				te->next=NULL;
		
		
				while(strcmp(line,"/$p")!=0)
					{
						if( (strcmp(line,"\0")==0) || (strcmp(line,"$p")==0) )  //skipping 'p' keyword or any blank line
							{
								fin.getline(line,100);
								remove_spacestabs(line);
								continue;
							}		
								


						char *s;
						s=strchr(line,':');					//checking the end of the entity name in the parsed file
						if(s!=NULL)						
							{						
								char *p=&line[0];
								char entity_name[50];
								int k=0;
								while((p!=s))
									{
										entity_name[k]=*p;	//getting the name of the entity
										p++;
										k++;
									}
								entity_name[k]='\0';			//making the character array 'entity_name' a string
								int t= strlen(entity_name);
								t++;
								char *temp=new char[t];
								strcpy(temp,entity_name);
								ptr_to_entity_names[count_for_names]=temp; 											//storing the pointer to the name of the variable in the array 'ptr_to_entity_names'
								/*char* parag=ptr_to_entity_names[count_for_names];
								int z=strlen(parag);			//for debugging
								cout.write(parag,z)<<endl;*/
								
								te->name=temp;
								
								count_for_names++;
								fin.getline(line,100);			//read next line
								remove_spacestabs(line);
							}

						char *h;
						h=strchr(line,'@');					//checking the end of the weak entity flag in the parsed file
						if(h!=NULL)						
							{
								int weak_flag=line[0]-48;
								te->weak_entity=weak_flag;								
								
								/*cout<<te->weak_entity<<endl;*/	//for debugging							
								fin.getline(line,100);			
								remove_spacestabs(line);		//reading next line						
							}
						

						char *w;
						w=strchr(line,'!');					//checking the end of the attribute name in the parsed file
						if(w!=NULL)						
							{						
								Attribute *ta = new Attribute;		//ta is a pointer to a temporary attribute in an entity
								ta->name=NULL;				//initializing the 'Attribute' fields
								ta->multivalued=0;
								ta->primary=0;
								ta->next=NULL;
						
				
								char *u=&line[0];
								char attribute_name[50];
								int l=0;
								while((u!=w))
									{
										attribute_name[l]=*u;	//getting the name of the attribute
										u++;
										l++;
									}
								attribute_name[l]='\0';			//making the character array 'attribute_name' a string
								int v= strlen(attribute_name);
								v++;
								char *temp2=new char[v];
								strcpy(temp2,attribute_name);
								ptr_to_attribute_names[count_for_anames]=temp2; 										//storing the pointer to the name of the attribute in the array 'ptr_to_attribute_names'
								/*char* parikshit=ptr_to_attribute_names[count_for_anames];
								int x=strlen(parikshit);		//for Debugging
								cout.write(parikshit,x)<<endl;*/
								
								ta->name=temp2;

								count_for_anames++;
								

								u++;
								char *g;
								g=strchr(line,'(');	//checking the start of the primary and multivalued flags in the parsed file
								u=strchr(line,')');	//checking the end of the primary and multivalued flags in the parsed file
								
								int primary_flag;
								int multivalued_flag;
								if(u!=NULL)						
									{						
										if(g!=NULL)								
											{
												primary_flag=(*(++g))-48;
												multivalued_flag=(*(--u))-48;
											}						
									}
								ta->multivalued=multivalued_flag;
								ta->primary=primary_flag;
								
								/*cout<<"("<<ta->primary<<","<<ta->multivalued<<")"<<endl;*/		//for Debugging
								
								if((te->attr)==NULL)			//appending the temporary attribute in the link list 'attr'
									{
										te->attr=ta;	
										ta->next=NULL;
									}
								else
									{	
										Attribute *pari=te->attr;
										while((pari->next)!=NULL)
											pari=pari->next;
										ta->next=NULL;									
										pari->next=ta;
									}
																
								fin.getline(line,100);			//reading next line
								remove_spacestabs(line);
							}
					}
				
				if((head_entity)==NULL)			//appending the temporary entity in the link list 'head_entity'
					{
						head_entity=te;
						te->next=NULL;
					}
				else
					{
						Entity *para=head_entity;
						while((para->next)!=NULL)
							para=para->next;
						te->next=NULL;									
						para->next=te;
					}

				fin.getline(line,100);			//reading next line
				remove_spacestabs(line);
			}

		fin.getline(line,100);                                  //instruction are of maximum length 100 characetrs .We can increase it if we wish to
		remove_spacestabs(line);

		while(strcmp(line,"/Relationship")!=0)	                                       		//checking the lines before '/Relationship'
			{
				if( (strcmp(line,"\0")==0) || (strcmp(line,"Relationship")==0) )      	//skipping 'Relationship' keyword or any blank line
					{
						fin.getline(line,100);
						remove_spacestabs(line);
						continue;
					}		
				
				Relationship *te = new Relationship;					//te is a pointer to a temporary relationship
				te->identifying=0;							//initializing the 'Relationship' fields
				te->name=NULL;				
				te->list=NULL;
				te->next=NULL;
				te->cardinality=0;
		
		
				while(strcmp(line,"/$p")!=0)
					{
						if( (strcmp(line,"\0")==0) || (strcmp(line,"$p")==0) )  //skipping 'p' keyword or any blank line
							{
								fin.getline(line,100);
								remove_spacestabs(line);
								continue;
							}		
								


						char *s;
						s=strchr(line,':');					//checking the end of the relationship name in the parsed file
						if(s!=NULL)						
							{						
								char *p=&line[0];
								char relationship_name[50];
								int k=0;
								while((p!=s))
									{
										relationship_name[k]=*p;	//getting the name of the relationship
										p++;
										k++;
									}
								relationship_name[k]='\0';			//making the character array 'relationship_name' a string
								int t= strlen(relationship_name);
								t++;
								char *temp=new char[t];
								strcpy(temp,relationship_name);
								ptr_to_relationship_names[count_for_rnames]=temp; 										//storing the pointer to the name of the relationship in the array 'ptr_to_relationship_names'
								/*char* parag=ptr_to_relationship_names[count_for_rnames];
								int z=strlen(parag);				//for Debugging
								cout.write(parag,z)<<endl;*/
								
								te->name=temp;
								
								count_for_rnames++;
								fin.getline(line,100);			//reading next line
								remove_spacestabs(line);
							}

						char *h;
						h=strchr(line,'@');					//checking the end of the identifying flag in the parsed file
						if(h!=NULL)						
							{
								int identifying_flag=line[0]-48;
								te->identifying=identifying_flag;								
								
								/*cout<<te->identifying<<endl;*/	//for Debugging
								fin.getline(line,100);			//reading next line
								remove_spacestabs(line);								
							}
						

						char *w;
						w=strchr(line,'%');			//checking the end of the name of entity (in relationship) in the parsed file
						if(w!=NULL)						
							{						
								Entity_in_Relationship *ta = new Entity_in_Relationship;
								ta->name=NULL;		//ta is a pointer to a temporary entity in relationship		
								ta->many=0;		//initializing the 'Entity_in_Relationship' fields
								ta->fullp=0;
								ta->next=NULL;
						
				
								char *u=&line[0];
								char Entity_in_Relationship_name[50];
								int l=0;
								while((u!=w))
									{
										Entity_in_Relationship_name[l]=*u;	//getting the name of the Entity_in_Relationship
										u++;
										l++;
									}
								Entity_in_Relationship_name[l]='\0';//making the character array 'Entity_in_Relationship_name' a string
								int v= strlen(Entity_in_Relationship_name);
								v++;
								char *temp2=new char[v];
								strcpy(temp2,Entity_in_Relationship_name);
								ptr_to_Entity_in_Relationship_names[count_for_ernames]=temp2; 							//storing the pointer to the name of the Entity_in_Relationship in the array 'ptr_to_Entity_in_Relationship_names'
								
								/*char* parikshit=ptr_to_Entity_in_Relationship_names[count_for_ernames];
								int x=strlen(parikshit);				//for Debugging
								cout.write(parikshit,x)<<endl;*/
								
								ta->name=temp2;

								count_for_ernames++;
								(te->cardinality)++;
								

								u++;
								char *g;
								g=strchr(line,'(');						
								u=strchr(line,')');	//checking the end of the many and full participation flags in the parsed file 	
								
								int many_flag;
								int fullp_flag;
								if(u!=NULL)						
									{						
										if(g!=NULL)								
											{
												many_flag=(*(++g))-48;
												fullp_flag=(*(--u))-48;
											}						
									}
								ta->many=many_flag;
								ta->fullp=fullp_flag;
								
								/*cout<<"("<<ta->many<<","<<ta->fullp<<")"<<endl;*/		//for Debugging
								
								if((te->list)==NULL)	//appending the temporary entity (in relationship) in the link list 'head_entity'
									{
										te->list=ta;
										ta->next=NULL;
									}
								else
									{	
										Entity_in_Relationship *pari=te->list;
										while((pari->next)!=NULL)
											pari=pari->next;
										ta->next=NULL;									
										pari->next=ta;
									}
																
								fin.getline(line,100);		//reading next line
								remove_spacestabs(line);
							}
						

						w=strchr(line,'!');				//checking the end of the relationship attribute name in the parsed file
						if(w!=NULL)						
							{						
								Relationship_Attribute *ta = new Relationship_Attribute;
								ta->name=NULL;			//ta is a pointer to a temporary relationship attribute
								ta->next=NULL;			//initializing the 'Relationship_Attribute' fields
						
				
								char *u=&line[0];
								char Relationship_Attribute_name[50];
								int l=0;
								while((u!=w))
									{
										Relationship_Attribute_name[l]=*u;	//getting the name of the relationship attribute
										u++;
										l++;
									}
								Relationship_Attribute_name[l]='\0';		//making the character array 'Attribute_name' a string
								int v= strlen(Relationship_Attribute_name);
								v++;
								char *temp2=new char[v];
								strcpy(temp2,Relationship_Attribute_name);
								ptr_to_Relationship_Attribute_names[count_for_ranames]=temp2; 							//storing the pointer to the name of the relationship attribute in the array 'ptr_to_Relationship_Attribute_names'
								/*char* parikshit=ptr_to_Relationship_Attribute_names[count_for_ranames];
								int x=strlen(parikshit);			//for Debugging
								cout.write(parikshit,x)<<endl;*/
								
								ta->name=temp2;

								count_for_ranames++;

								if((te->attr)==NULL)	//appending the temporary relationship attribute in the link list 'head_entity'
									{
										te->attr=ta;
										ta->next=NULL;
									}
								else
									{	
										Relationship_Attribute *pari=te->attr;
										while((pari->next)!=NULL)
											pari=pari->next;
										ta->next=NULL;									
										pari->next=ta;
									}

								
								fin.getline(line,100);	//reading next line
								remove_spacestabs(line);
							}

					}
				
				if((head_relationship)==NULL)				//appending the temporary relationship in the link list 'head_relationship'
					{
						head_relationship=te;
						te->next=NULL;
					}
				else
					{
						Relationship *para=head_relationship;
						while((para->next)!=NULL)
							para=para->next;
						te->next=NULL;									
						para->next=te;
					}

				/*cout<<te->cardinality<<endl;*/			//for Debugging

				fin.getline(line,100);					//reading next line
				remove_spacestabs(line);
			}
		fin.close();
	}
	
void save_table(Table *tb)
{
	Table *ttb;
	ttb = table_head;
	if(table_head == NULL)
	{
		table_head = tb;
	}
	else
	{
		while(ttb->next!=NULL)
		ttb = ttb->next;
		ttb->next = tb;
	}
}


Table *get_table(string name)
{
	Table *ttb;
	ttb = table_head;
	while(ttb->name != name)
	{
		ttb = ttb->next;
	}
	return ttb;
}

Entity *get_entity(string name)
{
	Entity *en;
	en = head_entity;
	while(en->name != name)
		en = en->next;
	return en;
}

void save_in_file()
{
	fstream fout;
	fout.open("Relational-Model",ios::out);
	Table *ttb;
	ttb = table_head;
	while(ttb != NULL)
	{
		cout<<ttb->data<<"\n";		
		fout<<ttb->data<<"\n";
		ttb = ttb->next;
	}
	fout.close();
}

void convert()
{
	Entity *e;
	Table *tb;
	Attribute *p,*mp;
	Relationship *rel;
	Entity_in_Relationship *e_rel;
	Relationship_Attribute *rel_attr;
	string str;
	e = head_entity;
	int timer = 0,len,tag = 0;
	
	while(e != NULL)							//parsing each entity set
	{
		tag = 0;
		if(e->weak_entity == 1)
		{
			e = e->next;
		}
		else
		{
			tb = new Table;
			tb->name = e->name;
			tb->data = (string)e->name + " {";
			tb->next = NULL;
			p = e->attr;
			while(p != NULL)					//parsing each attribute of a given entity 
			{
				if(p->primary == 1)					//if attribute is Primary Key
				{
					tb->data = tb->data + "(" + p->name + "),";
				}
				else 							//if attribute is non primary key
				{
					if(p->multivalued == 1)				//if attribute is multivalued
					{
						save_table(tb);
						tb = new Table;
						tb->name = (string)e->name + "-" + p->name;
						tb->data = (string)e->name + "-" + p->name + " {";
						tb->next = NULL;
						mp = e->attr;
						while(mp != NULL)	//traversing the attribute list to find primary keys
						{
							if(mp->primary == 1)		//if attribute is primary key
							{
								tb->data = tb->data + "(" + mp->name + "),";
							}
							mp = mp->next;
						}
						tb->data = tb->data + "(" + p->name + "),}";
						save_table(tb);
						tb = get_table(e->name);
						tag = 1;
					}
					else						//if attribute is not multivalued
					{
						tb->data = tb->data + p->name + ',';
					}
				}
				p = p->next;
			}
			tb->data = tb->data + '}';
			if(tag == 0)
			save_table(tb);
			e = e->next;
		}
	}
	rel = head_relationship;
	while(rel!=NULL)
	{
		if(rel->identifying == 1)
		{
			e_rel = rel->list;
			tb = new Table;
			tb->next = NULL;
			tb->data = " {";
			while(e_rel!=NULL)
			{
				if(e_rel->fullp == 1)
				{
					str = e_rel->name;
				}
				else
				{
					e = get_entity(e_rel->name);
					p = e->attr;
					while(p!=NULL)
					{
						if(p->primary == 1)
						{
							tb->data = tb->data + "(" + p->name + "),";
						}
						p = p->next;
					}
				}
				e_rel = e_rel->next;
			}
			tb->name = str;
			tb->data = str + tb->data;
			e = get_entity(str);
			p = e->attr;
			while(p!=NULL)
			{
				if(p->primary == 1)
				{
					tb->data = tb->data + "(" + p->name + "),";
				}
				else
				{
					tb->data = tb->data + p->name + ",";
				}
				p = p->next;
			}
			rel_attr = rel->attr;
			while(rel_attr!=NULL)
			{
				tb->data = tb->data + rel_attr->name + ",";
				rel_attr = rel_attr->next;
			}
			tb->data = tb->data + "}";
			save_table(tb);
		}
		else
		{
			if(rel->cardinality > 2)
			{
				tb = new Table;
				tb->name = rel->name;
				tb->next = NULL;
				tb->data = (string)rel->name + " {";
				e_rel = rel->list;
				while(e_rel!=NULL)
				{
					if(e_rel->many == 1)
					{
						e = get_entity(e_rel->name);
						p = e->attr;
						while(p!=NULL)
						{
							if(p->primary == 1)
							{
								tb->data = tb->data + "(" + p->name + "),";
							}
							p = p->next;
						}
					}
					e_rel = e_rel->next;
				}
				rel_attr = rel->attr;
				while(rel_attr!=NULL)
				{
					tb->data = tb->data + rel_attr->name + ",";
					rel_attr = rel_attr->next;
				}
				tb->data = tb->data + "}";
				save_table(tb);
			}
			else
			{
				e_rel = rel->list;
				while(e_rel!=NULL)
				{
					if(e_rel->many == 1)
					{
						timer++;
						str = e_rel->name;
					}
					e_rel = e_rel->next;
				}
				if(timer == 2)
				{
					tb = new Table;
					tb->name = rel->name;
					tb->next = NULL;
					tb->data = (string)rel->name + " {";
					e_rel = rel->list;
					while(e_rel!=NULL)
					{
						e = get_entity(e_rel->name);
						p = e->attr;
						while(p!=NULL)
						{
							if(p->primary == 1)
							{
								tb->data = tb->data + "(" + p->name + "),";
							}
							p = p->next;
						}
						e_rel = e_rel->next;
					}
					rel_attr = rel->attr;
					while(rel_attr!=NULL)
					{
						tb->data = tb->data + rel_attr->name + ",";
						rel_attr = rel_attr->next;
					}
					tb->data = tb->data + "}";
					save_table(tb);
				}
				else if(timer == 1)
				{
					tb = get_table(str);
					tb->data.erase((tb->data.length()-1),1);
					rel_attr = rel->attr;
					while(rel_attr!=NULL)
					{
						tb->data = tb->data + rel_attr->name + ",";
						rel_attr = rel_attr->next;
					}
					tb->data = tb->data + "}";
				}
				else if(timer == 0)
				{
					e_rel = rel->list;
					str = e_rel->name;
					tb = get_table(str);
					tb->data.erase((tb->data.length()-1),1);
					rel_attr = rel->attr;
					while(rel_attr!=NULL)
					{
						tb->data = tb->data + rel_attr->name + ",";
						rel_attr = rel_attr->next;
					}
					tb->data = tb->data + "}";
				}
			}
		}
		rel = rel->next;
	}
}

	







