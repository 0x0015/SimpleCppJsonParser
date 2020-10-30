#pragma once
#include <iostream>
#include <vector>
#include <map>
using namespace std;

enum JsonType{
	JsonNull=0,
	JsonString=1,
	JsonGeneric=0,
	JsonNone=0,
	JsonArray=2,
	JsonMap=3,
	JsonBoolean=4,
	JsonNumber=5
};

class JsonValue{
	public:
		int type;
		vector<JsonValue> list;
		map<string, JsonValue> dict;
		string value;
		bool boolean; //impliment later
		double number;//impliment later
		void print(){
			if(type == 0){
				cout<<"Generic Json Value"<<endl;
			}
			if(type == 1){
				cout<<"String Json Value"<<endl;
			}
			if(type == 2){
				cout<<"Array Json value"<<endl;
			}
			if(type == 3){
				cout<<"Map Json value"<<endl;
			}
			if(type == 4){
				cout<<"Boolean Json Value"<<endl;
			}
			if(type == 5){
				cout<<"Number Json Value"<<endl;
			}
			if(type == 6){
				cout<<"Null Json Value"<<endl;
			}
		}
		void setValue(string v){
			if(type == 0){
				value = v;
				type = 1;
			}else{
			if(type == 1){
				value = v;
			}else{
				cout<<"Json Error:  attempting to set type " + to_string(type) + " Json value to a string"<<endl;
			}
			}
		}
		void setValue(vector<JsonValue> v){
			if(type == 0){
				list = v;
				type = 2;
			}else{
			if(type == 2){
				list = v;
			}else{
				cout<<"Json Error:  attempting to set type " + to_string(type) + " Json value to a list"<<endl;
			}
			}
		}
		void setValue(map<string, JsonValue> v){
			if(type == 0){
				dict = v;
				type = 3;
			}else{
			if(type == 3){
				dict = v;
			}else{
				cout<<"Json Error:  attempting to set type " + to_string(type) + " Json value to a map"<<endl;
			}
			}
		}
		void setValue(bool b){
			if(type == 0){
				boolean = b;
				type = 4;
			}else{
				if(type  == 4){
					boolean = b;
				}else{
					cout<<"Json Error:  attempting to set type " + to_string(type) + " Json value to a boolean"<<endl;
				}
			}
		}
		void setValue(double n){
			if(type == 0){
				number = n;
				type = 5;
			}else{
				if(type  == 5){
					number = n;
				}else{
					cout<<"Json Error:  attempting to set type " + to_string(type) + " Json value to a number"<<endl;
				}
			}
		}
		void addValue(JsonValue v){
			if(type == 2){
				list.push_back(v);
			}else{
				cout<<"Json Error:  trying to append a type " + to_string(v.type) + " Json value to a list"<<endl;
			}
		}
		void addValue(string k, JsonValue v){
			if(type == 3){
				dict[k] = v;
			}else{
				cout<<"Json Error:  trying to insert a type " + to_string(v.type) + " Json value into a map"<<endl;
			}
		}
		string exportJson(){
			string output = "";
			if(type == 0){
			
			}
			if(type == 1){
				output = "\"" + value + "\"";//could just return(and might for optimization) but for now just stay the same.
			}
			if(type == 2){
				if(list.size() < 1){
					output = "[]";
				}else{
				output = output + "[\n";
				for(int i=0;i<list.size();i++){
					output = output + list[i].exportJson();
					if(i >= list.size()-1){
						output = output + "\n";
					}else{
						output = output + ",\n";
					}
				}
				output = output + "]";
				}
			}
			if(type == 3){
				if(dict.size() < 1){
					output = "{}";
				}else{
				output = output + "{\n";
				for(auto& x : dict){
					output = output + "\"" + x.first + "\" : " + x.second.exportJson();
					output = output + ",\n";
				}
				if(output.substr(output.size()-2, 1) == ","){
					output = output.substr(0, output.size()-2) + output.substr(output.size()-1, 1);
				}
				output = output + "}";
				}
			}
			if(type == 4){
				if(boolean){
					output = "true";
				}else{
					output = "false";
				}
			}
			if(type == 5){
				output = to_string(number);
			}
			if(type == 6){
				output = "null";
			}
			return(output);
		}
		string getStringAt(string data, size_t index){
			return(data.substr(index, 1));
		}
		string findEndString(string data){//fix problem with space in front of first quotation mark.  maybe just require quotation mark
			int i=0;
			string output = "";
			if(getStringAt(data, 0) == "\""){
				i++;
			}
			while(i<data.size()){
				if(getStringAt(data,i) == "\""){
					return(output);
				}
				output = output + getStringAt(data,i);
				i++;
			}
			cout<<"Json Import Error:  never found string end!"<<endl;
			return(output);
		}
		string findEndSquareBracket(string data){
			//cout<<"findEndSquareBracket " + data<<endl;
			int i=0;
			string output = "";
			int counter = 0;
			if(getStringAt(data,0) == "["){
				i++;
				counter++;
			}
			while(i<data.size()){
				if(getStringAt(data,i) == "["){
					counter++;
				}
				if(getStringAt(data,i) == "]"){
					counter--;
				}
				if(counter == 0){
					//cout<<"findEndCurlyBracket return " + output<<endl;
					return(output);
				}
				output = output + getStringAt(data,i);
				i++;
			}
			cout<<"Json Import Error:  never found closing square bracket"<<endl;
			
			return(output);
		}
		string findEndCurlyBracket(string data){
			int i=0;
			string output = "";
			int counter = 1;
			if(getStringAt(data,0) == "{"){
				i++;
			}
			while(i<data.size()){
				if(getStringAt(data,i) == "{"){
					counter++;
				}
				if(getStringAt(data,i) == "}"){
					counter--;
				}
				if(counter == 0){
					return(output);
				}
				output = output + getStringAt(data,i);
				i++;
			}
			cout<<"Json Import Error:  never found closing curly bracket"<<endl;
			return(output);
		}
		vector<string> split(const string text, string delimiter) {
		    string tmp = "";
		    int sqbcount = 0;
		    int cbcount = 0;
		    bool inquote = false;
		    vector<string> stk;
		    for(int i=0;i<text.size();i++){
			    if(getStringAt(text, i) == "["){sqbcount++;}
			    if(getStringAt(text, i) == "{"){cbcount++;}
			    if(getStringAt(text, i) == "]"){sqbcount--;}
			    if(getStringAt(text, i) == "}"){cbcount--;}
			    if(getStringAt(text, i) == "\""){inquote = !inquote;}
			if(sqbcount == 0 && cbcount == 0 && !inquote){
			if(getStringAt(text, i) == delimiter){
				stk.push_back(tmp);
				tmp = "";
			}else{
				tmp = tmp + getStringAt(text,i);
			}
		    }else{
			    tmp = tmp + getStringAt(text, i);
		    }
		    }
		    if(tmp != ""){
			    stk.push_back(tmp);
		    }
		    return stk;
		}
		string preprocessStringWhitespace(string data){
			string output = "";
			bool inQuote = false;
			for(int i=0;i<data.size();i++){
				if(getStringAt(data, i) == "\""){
					inQuote = !inQuote;
				}
				if(getStringAt(data, i) != " "){
					output = output + getStringAt(data, i);
				}
			}
			return(output);
		}
		bool importJson(string d){//add object/dict support
			string data = preprocessStringWhitespace(d);
			//string data = d;
			if(type != 0){
				cout<<"Json Import Error:  please only import Json Data to a blank JsonValue"<<endl;
				return(false);
			}
			if(getStringAt(data,0) == "\""){
				type = 1;
				value = findEndString(data);
			}
			if(getStringAt(data,0) == "["){
				type = 2;
				string inBracket = findEndSquareBracket(data);
				vector<string> items = split(inBracket, ",");
				for(int i=0;i<items.size();i++){
					//cout<<"processing array item " + items[i]<<endl;
					JsonValue temp(JsonNone);
					temp.importJson(items[i]);
					list.push_back(temp);
				}
			}
			if(getStringAt(data,0) == "{"){
				type = 3;
				string inBracket = findEndCurlyBracket(data);
				vector<string> items = split(inBracket, ",");
				for(int i=0;i<items.size();i++){
					//cout<<"processing array item " + items[i]<<endl;
					vector<string> mapItems = split(items[i], ":");
					if(mapItems.size() < 2){
						cout<<"Json Import Error:  incorrect object commas"<<endl;
						return(false);
					}
					JsonValue temp(JsonNone);
					temp.importJson(mapItems[1]);
					string key = mapItems[0];
					if(getStringAt(key, 0) == "\""){
						key = key.substr(1, key.size()-1);
					}
					if(getStringAt(key, key.size()-1) == "\""){
						key = key.substr(0, key.size()-1);
					}
					dict[key]=temp;
				}
			}
			if(data.substr(0,4) == "true"){
				type = 4;
				boolean = true;
			}
			if(data.substr(0,5) == "false"){
				type = 4;
				boolean = false;
			}
			if((getStringAt(data, 0) == "0") ||
					(getStringAt(data, 0) == "1") ||
					(getStringAt(data, 0) == "2") ||
					(getStringAt(data, 0) == "3") ||
					(getStringAt(data, 0) == "4") ||
					(getStringAt(data, 0) == "5") ||
					(getStringAt(data, 0) == "6") ||
					(getStringAt(data, 0) == "7") ||
					(getStringAt(data, 0) == "8") ||
					(getStringAt(data, 0) == "9") ||
					(getStringAt(data, 0) == ".")){
				type = 5;
				number = stod(data);
			}
			if(data.substr(0,4) == "null"){
				type = 6;
			}
			return(true);
		}
		JsonValue(){
			type = 0;
		}
		JsonValue(JsonType t){
			type = t;
			if(t < 0){
				type = 6;
			}
		}
		JsonValue(string v){
			//cout<<"string initializer"<<endl;
			type = 1;
			value = v;
		}
		JsonValue(const char* v){//nessecary because c++ things that const char*(c string) is closer to a bool than a c++ string
			type = 1;
			value = string(v);
		}
		JsonValue(vector<JsonValue> l){
			type = 2;
			list = l;
		}
		JsonValue(map<string, JsonValue> m){
			type = 3;
			dict = m;
		}
		JsonValue(bool b){
			//cout<<"boolean initializer"<<endl;
			type = 4;
			boolean = b;
		}
		JsonValue(double n){
			type = 5;
			number = n;
		}
};