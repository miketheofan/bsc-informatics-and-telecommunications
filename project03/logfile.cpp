#include "logfile.hpp"

Logfile::Logfile(int id){

	this->id = id;
	this->concurrentPeriods = new List;
}

Logfile::~Logfile(){

	this->file.close();
	delete concurrentPeriods;
}

/* Following function writes a message to the global log file, in which every process must be able to write to. For this reason, writting to this file, is done through a class which has this function that 
every time opens the file, writes to it and then closes it again till next insertion. If we were to do that without the help of this class, we would have a problem with multiple processes opening the file
at the same time. */
void Logfile::write(string message){

	string logName;

	logName.append("log"+to_string(id)+".txt");

	this->file.open(logName,fstream::app);
	this->file << message;
	this->file.close();

	return;
}

/* The following function finds periods in which multiple saladmakers worked concurrently. To do that, she uses another function which is called quickFing. */
void Logfile::findConcurrent(int digits){

	string logName;
	logName.append("log"+to_string(id)+".txt");

	ifstream MyReadFile(logName);
	string text , time , process , action;

	switch(digits){

		case(1):{

			while(getline(MyReadFile,text)){ /* First, we traverse through every line (message) of log file. */

				if( text.substr(17,4) == "Chef" ) continue; /* If message is written by chef, we continue to next message. */

				action = text.substr(30,text.length()-31);

				if( action == "Start making salad"){ /* Then check if message refers to a saladmaker starting a process of cooking a salad. */

					this->quickFind(text,digits); /* If yes call quickFind function. */
				}
			}

		}case(2):{

			while(getline(MyReadFile,text)){

				if( text.substr(18,4) == "Chef" ) continue;

				action = text.substr(31,text.length()-32);

				if( action == "Start making salad"){

					this->quickFind(text,digits);
				}
			}

		}case(3):{

			while(getline(MyReadFile,text)){

				if( text.substr(19,4) == "Chef" ) continue;

				action = text.substr(32,text.length()-33);

				if( action == "Start making salad"){

					this->quickFind(text,digits);
				}
			}

		}case(4):{

			while(getline(MyReadFile,text)){

				if( text.substr(20,4) == "Chef" ) continue;

				action = text.substr(33,text.length()-34);

				if( action == "Start making salad"){

					this->quickFind(text,digits);
				}
			}

		}case(5):{

			while(getline(MyReadFile,text)){

				if( text.substr(21,4) == "Chef" ) continue;

				action = text.substr(34,text.length()-35);

				if( action == "Start making salad"){

					this->quickFind(text,digits);
				}
			}
		}
	}

	this->concurrentPeriods->print();
}

/* quickFind function gets a message as an argument (and of course the number of digits of process id), traverses file again till we find the message that we got and then till we find another message
that was written by the same process indicating that saladmaker has ended cooking a salad, we check if any other process was running concurrently with our process. */
void Logfile::quickFind(string content,int digits){

	string logName;
	logName.append("log"+to_string(id)+".txt");

	ifstream MyReadFile(logName);
	string text , time , process , action , inserted;
	int foundText = 0;

	switch(digits){

		case(1):{

			while(getline(MyReadFile,text)){

					if(text == content) foundText =1; /* If we find the message in log file , update foundText variable. */

					if(!foundText) continue; /* Otherwise, continue with next message (line). */

					if( text.substr(17,4) == "Chef" ) continue; /* If message was written by chef, dismiss it. */

					time = text.substr(1,11);
					process = text.substr(17,11);
					action = text.substr(30,text.length()-31);

					if( action == "End making salad" && process != content.substr(17,11) ){ /* If message was written by a saladmaker, indicates that a saladmaker has started cooking and that saladmaker isn't
					the one whose message we have all this time, then that means that a saladmaker was cooking at the same time. So insert the span in which both saladmakers where working concurrently to the 
					list that this class has. */

						inserted.append( "[" + content.substr(1,11) + "," + time + "]" );
						
						this->concurrentPeriods->insert(inserted);
						inserted.clear();

					}else if( action == "End making salad" && process == content.substr(17,11) ) /* Otherwise, it means that current saladmaker has finished cooking a salad so we stop checking for further
					spans. */
						break;
			}

		}case(2):{

			while(getline(MyReadFile,text)){

					if(text == content) foundText =1;

					if(!foundText) continue;

					if( text.substr(18,4) == "Chef" ) continue;

					time = text.substr(1,11);
					process = text.substr(18,11);
					action = text.substr(31,text.length()-32);

					if( action == "End making salad" && process != content.substr(18,11) ){

						inserted.append( "[" + content.substr(1,11) + "," + time + "]" );
						
						this->concurrentPeriods->insert(inserted);
						inserted.clear();

					}else if( action == "End making salad" && process == content.substr(18,11) )
						break;
			}

		}case(3):{

			while(getline(MyReadFile,text)){

					if(text == content) foundText =1;

					if(!foundText) continue;

					if( text.substr(19,4) == "Chef" ) continue;

					time = text.substr(1,11);
					process = text.substr(19,11);
					action = text.substr(32,text.length()-33);

					if( action == "End making salad" && process != content.substr(19,11) ){

						inserted.append( "[" + content.substr(1,11) + "," + time + "]" );
						
						this->concurrentPeriods->insert(inserted);
						inserted.clear();

					}else if( action == "End making salad" && process == content.substr(19,11) )
						break;
			}

		}case(4):{

			while(getline(MyReadFile,text)){

				if(text == content) foundText =1;

				if(!foundText) continue;

				if( text.substr(20,4) == "Chef" ) continue;

				time = text.substr(1,11);
				process = text.substr(20,11);
				action = text.substr(33,text.length()-34);

				if( action == "End making salad" && process != content.substr(20,11) ){

					inserted.append( "[" + content.substr(1,11) + "," + time + "]" );
					
					this->concurrentPeriods->insert(inserted);
					inserted.clear();

				}else if( action == "End making salad" && process == content.substr(20,11) )
					break;
			}

		}case(5):{

			while(getline(MyReadFile,text)){

					if(text == content) foundText =1;

					if(!foundText) continue;

					if( text.substr(21,4) == "Chef" ) continue;

					time = text.substr(1,11);
					process = text.substr(21,11);
					action = text.substr(34,text.length()-35);

					if( action == "End making salad" && process != content.substr(21,11) ){

						inserted.append( "[" + content.substr(1,11) + "," + time + "]" );
						
						this->concurrentPeriods->insert(inserted);
						inserted.clear();

					}else if( action == "End making salad" && process == content.substr(21,11) )
						break;
			}
		}
	}
}