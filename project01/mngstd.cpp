#include "types.hpp"

int currentYear;
int TABLESIZE;
int DEFAULT_TABLESIZE = 20;

int getFileSize(string);
int getCurrentYear();
void fill(HashTable*,InvertedIndex*,string);
void i(HashTable*,InvertedIndex*,StudentID,LastName,FirstName,Zip,Year,GPS);
bool l(HashTable*,InvertedIndex*,StudentID);
void d(HashTable*,InvertedIndex*,StudentID);
int n(HashTable*,InvertedIndex*,Year);
void t(HashTable*,InvertedIndex*,int,Year);
void a(HashTable*,InvertedIndex*,Year);
void m(HashTable*,InvertedIndex*,Year);
void c(HashTable*,InvertedIndex*);
void p(HashTable*,InvertedIndex*,int);

int main(int argc,char** argv){

	currentYear = getCurrentYear();/*This function returns the current year using <time.h>
									and is stored in a variable named currentYear.The main usage of this variable
									is to store the students in the corresponding Inverted Index's node 
									by the year that he entered the university.*/ 

	StudentID studentid;
	Zip zip;
	Year year;
	GPS gps;
	FirstName first;
	LastName last;
	int num , input;



	if(argc > 1){ /*Check if there was an input file given in the execution of the program.If there was more than 1 word given after the ./mngstd call,
					count the number of inputs there are in the file using the getFileSize() function and store the return value of the function to the 
					global variable TABLESIZE.*/

		for(int i=0;i<argc;i++)
			if( strcmp(argv[i],"-i") ){

				TABLESIZE = getFileSize(argv[i+2]);
				if(TABLESIZE == -1){

					cout << "File not found." << endl << endl;
					TABLESIZE = DEFAULT_TABLESIZE;
				}
				break;
			}
	}else TABLESIZE = DEFAULT_TABLESIZE;

	HashTable ht;
	InvertedIndex ii;
	
/*Again if an input file was given,fill both Hash Table and Inverted Index using the values that exist in the file.*/
	if(argc>1){

		for(int i=0;i<argc;i++)
			if( strcmp(argv[i],"-i") ){

				if(TABLESIZE != -1)
	 				fill(&ht,&ii,argv[i+2]);
				break;
			}
	}

	do{

		cout << "1.i(nsert)\n2.l(ook-up)\n3.d(elete)\n4.n(umber)\n5.t(op)\n6.a(verage)\n7.m(inimum)\n8.c(ount)\n9.p(ostal code)\n10.exit\n";
		cout << "Give me your action(1-10):";
		cin >> input;

		switch(input){

			case(1):{

				cout << "studentid:";
				cin >> studentid;
				cout << "lastname:";
				cin >>last;
				cout << "firstname:";
				cin >> first;
				cout << "zip:";
				cin >> zip;
				cout << "year:";
				cin >> year;
				cout << "gps:";
				cin >> gps;

				cout << endl;
				i(&ht,&ii,studentid,last,first,zip,year,gps);
				cout << endl;

				break;
				return 0;
			}
			case(2):{

				cout << "studentid:";
				cin >> studentid;

				cout << endl;
				l(&ht,&ii,studentid);
				cout << endl;

				break;
			}
			case(3):{

				cout << "studentid:";
				cin >> studentid;

				cout << endl;
				d(&ht,&ii,studentid);
				cout << endl;

				break;
			}
			case(4):{

				cout << "year:";
				cin >> year;

				int temp = n(&ht,&ii,year);
				if(!temp)
					cout << "No students enrolled in " << year << endl;
				else
					cout << endl << temp << " students in " << year << endl << endl;
				break;
			}
			case(5):{

				cout << "num:";
				cin >> num;
				cout << "year:";
				cin >> year;

				cout << endl;
				t(&ht,&ii,num,year);
				cout << endl;

				break;
			}
			case(6):{

				cout << "year:";
				cin >> year;

				cout << endl;
				a(&ht,&ii,year);
				cout << endl;

				break;
			}
			case(7):{

				cout << "year:";
				cin >> year;

				cout << endl;
				m(&ht,&ii,year);
				cout << endl;

				break;
			}
			case(8):{

				cout << endl;
				c(&ht,&ii);
				cout << endl;

				break;
			}
			case(9):{

				cout << "rank:";
				cin >> num;

				cout << endl;
				p(&ht,&ii,num);
				cout << endl;

				break;
			}
			case(10):{
/*At this point,if option 10 is given by the user , the program ends and every memory is automatically being re-allocated as both ht and ii are static values,
	so the destructor will automatically be called when the program ends.*/
				cout << "exit program" << endl;
				break;
			}
		}

	}while(input != 10);

	return 0;
}