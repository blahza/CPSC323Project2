/*
CPSC 323 - Assignment 2
Contributors:
Adrian Alaweneh
Andrew Duong
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// We will be using this to output to the file
ofstream outfile;

//Declaring Syntax rules
void R1();
void R2(bool &protoR2);
void R3();
void R4();
void R5();
void R6();
void R7();
void R8();
void R9();
void R10();
void R11();
void R12();
void R13();
void R14();
void R15();
void R16();
void R17();
void R18();
void R19();
void R20();
void R21();
void R22();
void R23();
void R24();
void R25();
void R26();
void R27();
void R28();
void R29();
void R30();
void R31();

// This is where we store the Tokens and Lexemes
vector<string> tokens;
vector<string> lexemes;

// This is the index we will use once we start using our parser
unsigned int global_index = 0;

// Check if integer
bool CheckInteger(string s) {
	for (unsigned i = 0; i < s.length(); i++) {
		if (!isdigit(s[i])) {
			return false;
		}
	}
	return true;
}

// Check if keyword
bool CheckKeyword(string word) {
	const int size = 16;
	string keywords[size] = {
		"assign",  "bool", "boolean",
		"compound",  "else", "endif",
		"false", "function", "if", "int",
		"put", "return", "real", "get", "true", "while"
	};

	for (int i = 0; i < size; i++) {
		if (word == keywords[i]) {
			return true;
		}
	}
	return false;
}

// Check if real
bool CheckReal(string s) {
	for (unsigned i = 0; i < s.length(); i++) {
		if (!isdigit(s[i]) && s[i] != '.') {
			return false;
		}
	}

	return true;
}

// Check if the real is a legal input (making sure there's an integer after the dot)
bool FakeReal(string s) {
	for (unsigned i = 0; i < s.length(); i++) {

		// Check if integer exist after the dot
		if (s[i] == '.') {
			if (!isdigit(s[i + 1])) {
				return true;
			}
		}
	}

	return false;
}

// Checking if operator
bool CheckOperator(char ch) {
	const int number_of_operators = 7;
									  
	char operators[number_of_operators] = { '+', '-', '*', '/', '=', '<', '>' };

	for (int i = 0; i < number_of_operators; i++) {
		if (ch == operators[i]) {
			return true;
		}
	}
	return false;
}

// Checking if seperator
bool CheckSeperator(char ch) {
	const int number_of_seperators = 10;
										 
	char seperators[number_of_seperators] = { '(', ')', '{', '}', '[', ']', '.', ',', ';', ':' };

	for (int i = 0; i < number_of_seperators; i++) {
		if (ch == seperators[i]) {
			return true;
		}
	}
	return false;
}

// Checking the type of token
void CheckToken(string value) {
	if (CheckKeyword(value)) {
		tokens.push_back("keyword");
		lexemes.push_back(value);
	}
	else if (CheckInteger(value)) {
		tokens.push_back("integer");
		lexemes.push_back(value);
	}
	else if (CheckReal(value) && (!FakeReal(value))) {
		tokens.push_back("real");
		lexemes.push_back(value);
	}
	else if (CheckReal(value) && FakeReal(value)) {
		tokens.push_back("illegal input");
		lexemes.push_back(value);
	}
	else if (value == "%%") {
		tokens.push_back("seperator");
		lexemes.push_back(value);
	}
	else {
		tokens.push_back("identifier");
		lexemes.push_back(value);
	}
}

// Analyzing the current input
string Analyze(char ch, string value) {

	// %%
	if (ch == '%') {
		if (value.empty()) {
			value = value + ch;
			return value;
		}
		else if (!value.empty() && value != "%") {
			tokens.push_back("illegal input");
			lexemes.push_back(value);
		}
		else {
			value = value + string(1, ch);
			tokens.push_back("seperator");
			lexemes.push_back(value);
			return "";
		}
	}

	// Relational Operators
	if (ch == '=' || ch == '>' || ch == '<' || ch == '^') {
		if (value == "=") {
			value = value + string(1, ch);
			tokens.push_back("operator");
			lexemes.push_back(value);
			return "";
		}
		else if (value == "^" && ch == '=') {
			value = value + string(1, ch);
			tokens.push_back("operator");
			lexemes.push_back(value);
			return "";
		}
		else {
			if (value != "") {
				CheckToken(value);
			}

			// Next statement returns ch as a string value.
			return string() + ch;
		}
	}

	if (value == "=" || value == ">" || value == "<") {
		tokens.push_back("operator");
		lexemes.push_back(value);
		value = "";
	}

	// can't have '^' token by itself. Only "^=" is valid.
	if (value == "^") {
		tokens.push_back("illegal input");
		lexemes.push_back(value);
	}

	if (isalnum(ch) || ch == '.' || ch == '$') {
		value = value + ch;
		return value;
	}

	if ((isblank(ch) || !isalpha(ch) || ch != '$') && value != "") {
		CheckToken(value);
	}

	string temp;

	if (CheckOperator(ch)) {
		temp += ch;
		tokens.push_back("operator");
		lexemes.push_back(temp);
		temp = "";
	}
	if (CheckSeperator(ch)) {
		temp += ch;
		tokens.push_back("seperator");
		lexemes.push_back(temp);
		temp = "";
	}

	return "";
}

// Our Lexer function
void Lexer() {
	ifstream infile;
	infile.open("Rat18S.txt");

	if (!infile.is_open()) {
		cout << "Rat18S.txt was not able to be opened!" << endl;
	}

	char ch;
	bool comment_flag = 0;
	string value = "";

	while (!infile.eof()) {
		ch = infile.get();

		// CHECKING COMMENTS
		if (ch == '!' && comment_flag == 0) {
			comment_flag = 1;
			continue;
		}
		if (ch == '!' && comment_flag == 1) {
			comment_flag = 0;
			value = "";
			continue;
		}
		if (comment_flag) {
			continue;
		}

		value = Analyze(ch, value);
	}

	infile.close();
}

// Prints out the token type and lexeme value
void PrintToken() {
	cout << "=====================================================" << endl;
	outfile << "=====================================================" << endl;
	if (tokens[global_index] == "operator") {
		cout << "Token: " << tokens[global_index] << "\t\t\t" << "Lexeme: " << lexemes[global_index] << endl;
		outfile << "Token: " << tokens[global_index] << "\t\t\t" << "Lexeme: " << lexemes[global_index] << endl;
	}
	else if (tokens[global_index] == "keyword") {
		cout << "Token: " << tokens[global_index] << "\t\t\t" << "Lexeme: " << lexemes[global_index] << endl;
		outfile << "Token: " << tokens[global_index] << "\t\t\t" << "Lexeme: " << lexemes[global_index] << endl;
	}
	else if (tokens[global_index] == "real") {
		cout << "Token: " << tokens[global_index] << "\t\t\t\t" << "Lexeme: " << lexemes[global_index] << endl;
		outfile << "Token: " << tokens[global_index] << "\t\t\t\t" << "Lexeme: " << lexemes[global_index] << endl;
	}
	else if (tokens[global_index] == "integer") {
		cout << "Token: " << tokens[global_index] << "\t\t\t" << "Lexeme: " << lexemes[global_index] << endl;
		outfile << "Token: " << tokens[global_index] << "\t\t\t" << "Lexeme: " << lexemes[global_index] << endl;
	}
	else {
		cout << "Token: " << tokens[global_index] << "\t\t" << "Lexeme: " << lexemes[global_index] << endl;
		outfile << "Token: " << tokens[global_index] << "\t\t" << "Lexeme: " << lexemes[global_index] << endl;
	}
	cout << "=====================================================" << endl;
	outfile << "=====================================================" << endl;
}

// These are all the function for all the syntax rules of Rat18S, check SyntaxRules.txt for a complete list
void R1() {
	cout << "<Rat18S> -> <Opt Function Definitions>   %%  <Opt Declaration List>  <Statement List>" << endl;
	outfile << "<Rat18S> -> <Opt Function Definitions>   %%  <Opt Declaration List>  <Statement List>" << endl;
	bool empty = false;
	R2(empty);
	if (empty == false) {
		if (lexemes[global_index] == "%%") {
			PrintToken();
			global_index++;
			R10();
			R14();
		}
		else {
			cout << "ERROR: Expected %%" << endl;
			outfile << "ERROR: Expected %%" << endl;
		}
	}
	else {
		R10();
		R14();
	}
}

void R2(bool &empty) {
	if (lexemes[global_index] == "function") {
		cout << "<Opt Function Definitions> -> <Function Definitions>" << endl;
		outfile << "<Opt Function Definitions> -> <Function Definitions>" << endl;
		R3();
		empty = false;
	}
	else {
		cout << "<Opt Function Definitions> -> <Empty>" << endl;
		outfile << "<Opt Function Definitions> -> <Empty>" << endl;
		R31();
		empty = true;
	}
}

void R3() {
	cout << "<Function Definitions> -> <Function>" << endl;
	outfile << "<Function Definitions> -> <Function>" << endl;
	R4();
	if (lexemes[global_index] == "function") {
		cout << "<Function Definitions> -> <Function> <Function Definitions>" << endl;
		outfile << "<Function Definitions> -> <Function> <Function Definitions>" << endl;
		PrintToken();
		global_index++;
		R3();
	}
}

void R4() {
	if (lexemes[global_index] == "function") {
		cout << "<Function> -> function  <Identifier>  [ <Opt Parameter List> ]  <Opt Declaration List>  <Body>" << endl;
		outfile << "<Function> -> function  <Identifier>  [ <Opt Parameter List> ]  <Opt Declaration List>  <Body>" << endl;
		PrintToken();
		global_index++;
		if (tokens[global_index] == "identifier") {
			PrintToken();
			global_index++;
			if (lexemes[global_index] == "[") {
				PrintToken();
				global_index++;
				R5();
				if (lexemes[global_index] == "]") {
					PrintToken();
					global_index++;
					R10();
					R9();
				}
				else {
					cout << "ERROR: Expected ']'" << endl;
					outfile << "ERROR: Expected ']'" << endl;
				}
			}
			else {
				cout << "ERROR: Expected '['" << endl;
				outfile << "ERROR: Expected '['" << endl;
			}
		}
		else {
			cout << "ERROR: identifier was expected!" << endl;
			outfile << "ERROR: identifier was expected!" << endl;
		}
	}
	else {
		cout << "ERROR: function was expected!" << endl;
		outfile << "ERROR: function was expected!" << endl;
	}
}

void R5() {
	if (tokens[global_index] == "identifier") {
		cout << "<Opt Parameter List> -> <Parameter List>" << endl;
		outfile << "<Opt Parameter List> -> <Parameter List>" << endl;
		R6();
	}
	else {
		cout << "<Opt Parameter List> -> <Empty>" << endl;
		outfile << "<Opt Parameter List> -> <Empty>" << endl;
		R31();
	}
}

void R6() {
	cout << "<Parameter List> -> <Parameter>" << endl;
	outfile << "<Parameter List> -> <Parameter>" << endl;
	R7();
	if (lexemes[global_index] == ",") {
		cout << "<Parameter List> -> <Parameter> , <Parameter List>" << endl;
		outfile << "<Parameter List> -> <Parameter> , <Parameter List>" << endl;
		PrintToken();
		global_index++;
		R6();
	}
}

void R7() {
	cout << "<Parameter> -> <IDs > : <Qualifier>" << endl;
	outfile << "<Parameter> -> <IDs > : <Qualifier>" << endl;
	R13();
	if (lexemes[global_index] == ":") {
		PrintToken();
		global_index++;
		R8();
	}
	else {
		cout << "ERROR: Expected a ':'" << endl;
		outfile << "ERROR: Expected a ':'" << endl;
	}
}

void R8() {
	if (lexemes[global_index] == "int") {
		cout << "<Qualifier> -> int" << endl;
		outfile << "<Qualifier> -> int" << endl;
		PrintToken();
		global_index++;
	}
	else if (lexemes[global_index] == "boolean") {
		cout << "<Qualifier> -> boolean" << endl;
		outfile << "<Qualifier> -> boolean" << endl;
		PrintToken();
		global_index++;
	}
	else if (lexemes[global_index] == "real") {
		cout << "<Qualifier> -> real" << endl;
		outfile << "<Qualifier> -> real" << endl;
		PrintToken();
		global_index++;
	}
	else {
		cout << "ERROR: Qualifer not valid!" << endl;
		outfile << "ERROR: Qualifer not valid!" << endl;
	}
}

void R9() {
	if (lexemes[global_index] == "{") {
		cout << "<Body> -> {  <Statement List>  }" << endl;
		outfile << "<Body> -> {  <Statement List>  }" << endl;
		PrintToken();
		global_index++;
		R14();
		if (lexemes[global_index] == "}") {
			PrintToken();
			global_index++;
		}
		else {
			cout << "ERROR: Expected '}'" << endl;
			outfile << "ERROR: Expected '}'" << endl;
		}
	}
	else {
		cout << "ERROR: Expected '{'" << endl;
		outfile << "ERROR: Expected '{'" << endl;
	}
}

void R10() {
	if (lexemes[global_index] == "int") {
		cout << "<Opt Declaration List> -> <Declaration List>" << endl;
		outfile << "<Opt Declaration List> -> <Declaration List>" << endl;
		R11();
	}
	else if (lexemes[global_index] == "boolean") {
		cout << "<Opt Declaration List> -> <Declaration List>" << endl;
		outfile << "<Opt Declaration List> -> <Declaration List>" << endl;
		R11();
	}
	else if (lexemes[global_index] == "real") {
		cout << "<Opt Declaration List> -> <Declaration List>" << endl;
		outfile << "<Opt Declaration List> -> <Declaration List>" << endl;
		R11();
	}
	else {
		cout << "<Opt Declaration List> -> <Empty>" << endl;
		outfile << "<Opt Declaration List> -> <Empty>" << endl;
		R31();
	}
}

void R11() {
	cout << "<Declaration List> -> <Declaration> ;" << endl;
	outfile << "<Declaration List> -> <Declaration> ;" << endl;
	R12();
	if (lexemes[global_index] == ";") {
		PrintToken();
		global_index++;
		if (lexemes[global_index] == "int") {
			cout << "<Declaration List> -> <Declaration> ; <Declaration List>" << endl;
			outfile << "<Declaration List> -> <Declaration> ; <Declaration List>" << endl;
			R11();
		}
		else if (lexemes[global_index] == "boolean") {
			cout << "<Declaration List> -> <Declaration> ; <Declaration List>" << endl;
			outfile << "<Declaration List> -> <Declaration> ; <Declaration List>" << endl;
			R11();
		}
		else if (lexemes[global_index] == "real") {
			cout << "<Declaration List> -> <Declaration> ; <Declaration List>" << endl;
			outfile << "<Declaration List> -> <Declaration> ; <Declaration List>" << endl;
			R11();
		}
		else if (tokens[global_index] == "identifier") {
			cout << "<Declaration List> -> <Declaration> ; <Declaration List>" << endl;
			outfile << "<Declaration List> -> <Declaration> ; <Declaration List>" << endl;
			R11();
		}
	}
	else {
		cout << "ERROR: Expected ;" << endl;
		outfile << "ERROR: Expected ;" << endl;
	}
}

void R12() {
	cout << "<Declaration -> <Qualifier > <IDs>" << endl;
	outfile << "<Declaration -> <Qualifier > <IDs>" << endl;
	R8();
	R13();
}

void R13() {
	if (tokens[global_index] == "identifier") {
		cout << "<IDs> -> <Identifier>" << endl;
		outfile << "<IDs> -> <Identifier>" << endl;
		PrintToken();
		global_index++;
		if (lexemes[global_index] == ",") {
			cout << "<IDs> -> <Identifier>, <IDs>" << endl;
			outfile << "<IDs> -> <Identifier>, <IDs>" << endl;
			PrintToken();
			global_index++;
			R13();
		}
	}
	else {
		cout << "ERROR: identifier type is expected!" << endl;
		outfile << "ERROR: identifier type is expected!" << endl;
	}
}

void R14() {
	cout << "<Statement List> -> <Statement>" << endl;
	outfile << "<Statement List> -> <Statement>" << endl;
	R15();
	if (global_index < lexemes.size()) {
		if (lexemes[global_index] == "{") {
			cout << "<Statement List> -> <Statement> <Statement List>" << endl;
			outfile << "<Statement List> -> <Statement> <Statement List>" << endl;
			R14();
		}
		else if (tokens[global_index] == "identifier") {
			cout << "<Statement List> -> <Statement> <Statement List>" << endl;
			outfile << "<Statement List> -> <Statement> <Statement List>" << endl;
			R14();
		}
		else if (lexemes[global_index] == "if") {
			cout << "<Statement List> -> <Statement> <Statement List>" << endl;
			outfile << "<Statement List> -> <Statement> <Statement List>" << endl;
		}
		else if (lexemes[global_index] == "return") {
			cout << "<Statement List> -> <Statement> <Statement List>" << endl;
			outfile << "<Statement List> -> <Statement> <Statement List>" << endl;
			R14();
		}
		else if (lexemes[global_index] == "put") {
			cout << "<Statement List> -> <Statement> <Statement List>" << endl;
			outfile << "<Statement List> -> <Statement> <Statement List>" << endl;
			R14();
		}
		else if (lexemes[global_index] == "get") {
			cout << "<Statement List> -> <Statement> <Statement List>" << endl;
			outfile << "<Statement List> -> <Statement> <Statement List>" << endl;
			R14();
		}
		else if (lexemes[global_index] == "while") {
			cout << "<Statement List> -> <Statement> <Statement List>" << endl;
			outfile << "<Statement List> -> <Statement> <Statement List>" << endl;
			R14();
		}
	}
}

void R15() {
	if (lexemes[global_index] == "{") {
		cout << "<Statement> -> <Compound>" << endl;
		outfile << "<Statement> -> <Compound>" << endl;
		R16();
	}
	else if (tokens[global_index] == "identifier") {
		cout << "<Statement> -> <Assign>" << endl;
		outfile << "<Statement> -> <Assign>" << endl;
		R17();
	}
	else if (lexemes[global_index] == "if") {
		cout << "<Statement> -> <If>" << endl;
		outfile << "<Statement> -> <If>" << endl;
		R18();
	}
	else if (lexemes[global_index] == "return") {
		cout << "<Statement> -> <Return>" << endl;
		outfile << "<Statement> -> <Return>" << endl;
		R19();
	}
	else if (lexemes[global_index] == "put") {
		cout << "<Statement> -> <Print>" << endl;
		outfile << "<Statement> -> <Print>" << endl;
		R20();
	}
	else if (lexemes[global_index] == "get") {
		cout << "<Statement> -> <Scan>" << endl;
		outfile << "<Statement> -> <Scan>" << endl;
		R21();
	}
	else if (lexemes[global_index] == "while") {
		cout << "<Statement> -> <While>" << endl;
		outfile << "<Statement> -> <While>" << endl;
		R22();
	}
	else {
		cout << "ERROR: Expected a statement." << endl;
		outfile << "ERROR: Expected a statement." << endl;
	}
}

void R16() {
	cout << "<Compound> -> {  <Statement List>  } " << endl;
	outfile << "<Compound> -> {  <Statement List>  } " << endl;
	PrintToken();
	global_index++;
	R14();
	if (lexemes[global_index] == "}") {
		PrintToken();
		global_index++;
	}
	else {
		cout << "ERROR: Expected a '}'" << endl;
		outfile << "ERROR: Expected a '}'" << endl;
	}
}

void R17() {
	cout << "<Assign> -> <Identifier> = <Expression> ;" << endl;
	outfile << "<Assign> -> <Identifier> = <Expression> ;" << endl;
	PrintToken();
	global_index++;
	if (lexemes[global_index] == "=") {
		PrintToken();
		global_index++;
		R25();
		if (lexemes[global_index] == ";") {
			PrintToken();
			global_index++;
		}
		else {
			cout << "ERROR: Missing ';'" << endl;
			outfile << "ERROR: Missing ';'" << endl;
		}
	}
	else {
		cout << "ERROR: Expected an '='" << endl;
		outfile << "ERROR: Expected an '='" << endl;
	}
}

void R18() {
	if (lexemes[global_index] == "if") {
		PrintToken();
		global_index++;
		if (lexemes[global_index] == "(") {
			PrintToken();
			global_index++;
			R23();
			if (lexemes[global_index] == ")") {
				PrintToken();
				global_index++;
				R15();
				if (lexemes[global_index] == "endif") {
					cout << "<If> -> if ( <Condition> ) <Statement> endif" << endl;
					outfile << "<If> -> if ( <Condition> ) <Statement> endif" << endl;
					PrintToken();
					global_index++;
				}
				else if (lexemes[global_index] == "else") {
					cout << "<If> -> if ( <Condition> ) <Statement> else <Statement> endif" << endl;
					outfile << "<If> -> if ( <Condition> ) <Statement> else <Statement> endif" << endl;
					PrintToken();
					global_index++;
					R15();
					if (lexemes[global_index] == "endif") {
						PrintToken();
						global_index++;
					}
					else {
						cout << "ERROR: expected endif" << endl;
						outfile << "ERROR: expected endif" << endl;
					}
				}
				else {
					cout << "ERROR: Expected endif." << endl;
					outfile << "ERROR: Expected endif." << endl;
				}
			}
			else {
				cout << "ERROR: expected )" << endl;
				outfile << "ERROR: expected )" << endl;
			}

		}
		else {
			cout << "ERROR: Expected (" << endl;
			outfile << "ERROR: Expected (" << endl;
		}
	}
	else {
		cout << "ERROR: expected if" << endl;
		outfile << "ERROR: expected if" << endl;
	}
}

void R19() {
	if (lexemes[global_index] == "return") {
		cout << "<Return> -> return;" << endl;
		outfile << "<Return> -> return;" << endl;
		PrintToken();
		global_index++;
		if (lexemes[global_index] == ";") {
			PrintToken();
			global_index++;
		}
		else {
			cout << "<Return> -> return <Expression> ;" << endl;
			outfile << "<Return> -> return <Expression> ;" << endl;
			R25();
			if (lexemes[global_index] == ";") {
				PrintToken();
				global_index++;
			}
			else {
				cout << "ERROR: Missing ';'!" << endl;
				outfile << "ERROR: Missing ';'!" << endl;
			}
		}
	}
	else {
		cout << "ERROR: expected return" << endl;
		outfile << "ERROR: expected return" << endl;
	}

}

void R20() {
	if (lexemes[global_index] == "put") {
		cout << "<Print> -> put ( <Expression> );" << endl;
		outfile << "<Print> -> put ( <Expression> );" << endl;
		PrintToken();
		global_index++;
		if (lexemes[global_index] == "(")
		{
			PrintToken();
			global_index++;
			R25();
			if (lexemes[global_index] == ")") {
				PrintToken();
				global_index++;
				if (lexemes[global_index] == ";") {
					PrintToken();
					global_index++;
				}
				else {
					cout << "ERROR: expected ;" << endl;
					outfile << "ERROR: expected ;" << endl;
				}
			}
		}
		else {
			cout << "ERROR: expected (" << endl;
			outfile << "ERROR: expected (" << endl;
		}
	}
	else {
		cout << "ERROR: expected put" << endl;
		outfile << "ERROR: expected put" << endl;
	}
}

void R21() {
	if (lexemes[global_index] == "get") {
		cout << "<Scan> -> get ( <IDs> );" << endl;
		outfile << "<Scan> -> get ( <IDs> );" << endl;
		PrintToken();
		global_index++;
		if (lexemes[global_index] == "(") {
			PrintToken();
			global_index++;
			R13();
			if (lexemes[global_index] == ")") {
				PrintToken();
				global_index++;
				if (lexemes[global_index] == ";") {
					PrintToken();
					global_index++;
				}
				else {
					cout << "ERROR: Missing ';'" << endl;
					outfile << "ERROR: Missing ';'" << endl;
				}
			}
			else {
				cout << "ERROR: Expected ')'." << endl;
				outfile << "ERROR: Expected ')'." << endl;
			}
		}
		else {
			cout << "ERROR: Expected a '('" << endl;
			outfile << "ERROR: Expected a '('" << endl;
		}
	}
	else {
		cout << "ERROR: Expected 'get'." << endl;
		outfile << "ERROR: Expected 'get'." << endl;
	}
}

void R22() {
	if (lexemes[global_index] == "while") {
		cout << "<While> -> while ( <Condition>  )  <Statement>" << endl;
		outfile << "<While> -> while ( <Condition>  )  <Statement>" << endl;
		PrintToken();
		global_index++;
		if (lexemes[global_index] == "(") {
			PrintToken();
			global_index++;
			R23();
			if (lexemes[global_index] == ")") {
				PrintToken();
				global_index++;
				R15();
			}
			else {
				cout << "ERROR: ')' was expected." << endl;
				outfile << "ERROR: ')' was expected." << endl;
			}
		}
		else {
			cout << "ERROR: '(' was expected." << endl;
			outfile << "ERROR: '(' was expected." << endl;
		}
	}
	else {
		cout << "ERROR: while was expected." << endl;
		outfile << "ERROR: while was expected." << endl;
	}
}

void R23() {
	cout << "<Condition> -> <Expression>  <Relop>   <Expression>" << endl;
	outfile << "<Condition> -> <Expression>  <Relop>   <Expression>" << endl;
	R25();
	R24();
	R25();
}

void R24() {
	if (lexemes[global_index] == "==") {
		cout << "<Relop> -> ==" << endl;
		outfile << "<Relop> -> ==" << endl;
		PrintToken();
		global_index++;
	}
	else if (lexemes[global_index] == "^=") {
		cout << "<Relop> -> ^=" << endl;
		outfile << "<Relop> -> ^=" << endl;
		PrintToken();
		global_index++;
	}
	else if (lexemes[global_index] == ">") {
		cout << "<Relop> -> >" << endl;
		outfile << "<Relop> -> >" << endl;
		PrintToken();
		global_index++;
	}
	else if (lexemes[global_index] == "<") {
		cout << "<Relop> -> <" << endl;
		outfile << "<Relop> -> <" << endl;
		PrintToken();
		global_index++;
	}
	else if (lexemes[global_index] == "=>") {
		cout << "<Relop> -> =>" << endl;
		outfile << "<Relop> -> =>" << endl;
		PrintToken();
		global_index++;
	}
	else if (lexemes[global_index] == "=<") {
		cout << "<Relop> -> =<" << endl;
		outfile << "<Relop> -> =<" << endl;
		PrintToken();
		global_index++;
	}
	else {
		cout << "<Relop> is invalid" << endl;
		outfile << "<Relop> is invalid" << endl;
	}
}

void R25() {
	cout << "<Expression> -> <Term> <Expression Prime>" << endl;
	outfile << "<Expression> -> <Term> <Expression Prime>" << endl;
	R27();
	R26();
}

void R26() {
	if (lexemes[global_index] == "+") {
		cout << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
		outfile << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
		PrintToken();
		global_index++;
		R27();
		R26();
	}
	else if (lexemes[global_index] == "-") {
		cout << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
		outfile << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
		PrintToken();
		global_index++;
		R27();
		R26();
	}
	else if (tokens[global_index] == "identifier") {
		cout << "<Expression Prime> -> <Term>" << endl;
		outfile << "<Expression Prime> -> <Term>" << endl;
		PrintToken();
		global_index++;
		R27();
	}
	else if (tokens[global_index] == "integer") {
		cout << "<Expression Prime> -> <Term>" << endl;
		outfile << "<Expression Prime> -> <Term>" << endl;
		PrintToken();
		global_index++;
		R27();
	}
	else if (lexemes[global_index] == "(") {
		cout << "<Expression Prime> -> <Term>" << endl;
		outfile << "<Expression Prime> -> <Term>" << endl;
		PrintToken();
		global_index++;
		R27();
	}
	else if (tokens[global_index] == "real") {
		cout << "<Expression Prime> -> <Term>" << endl;
		outfile << "<Expression Prime> -> <Term>" << endl;
		PrintToken();
		global_index++;
		R27();
	}
	else if (lexemes[global_index] == "true") {
		cout << "<Expression Prime> -> <Term>" << endl;
		outfile << "<Expression Prime> -> <Term>" << endl;
		PrintToken();
		global_index++;
		R27();
	}
	else if (lexemes[global_index] == "false") {
		cout << "<Expression Prime> -> <Term>" << endl;
		outfile << "<Expression Prime> -> <Term>" << endl;
		PrintToken();
		global_index++;
		R27();
	}
	else {
		cout << "<Expression Prime> -> <Empty>" << endl;
		outfile << "<Expression Prime> -> <Empty>" << endl;
		R31();
	}
}

void R27() {
	cout << "<Term> -> <Factor> <Term Prime>" << endl;
	outfile << "<Term> -> <Factor> <Term Prime>" << endl;
	R29();
	R28();
}

void R28() {
	if (global_index < lexemes.size()) {
		if (lexemes[global_index] == "*") {
			cout << "<Term Prime> -> *  <Factor> <Term Prime>" << endl;
			outfile << "<Term Prime> -> *  <Factor> <Term Prime>" << endl;
			PrintToken();
			global_index++;
			R29();
			R28();
		}
		else if (lexemes[global_index] == "/") {
			cout << "<Term Prime> -> /  <Factor> <Term Prime>" << endl;
			outfile << "<Term Prime> -> /  <Factor> <Term Prime>" << endl;
			PrintToken();
			global_index++;
			R29();
			R28();
		}
		else if (tokens[global_index] == "identifier") {
			cout << "<Term Prime> -> <Factor>" << endl;
			outfile << "<Term Prime> -> <Factor>" << endl;
			R29();
		}
		else if (tokens[global_index] == "integer") {
			cout << "<Term Prime> -> <Factor>" << endl;
			outfile << "<Term Prime> -> <Factor>" << endl;
			R29();
		}
		else if (lexemes[global_index] == "(") {
			cout << "<Term Prime> -> <Factor> << endl" << endl;
			outfile << "<Term Prime> -> <Factor>" << endl;
			R29();
		}
		else if (tokens[global_index] == "real") {
			cout << "<Term Prime> -> <Factor>" << endl;
			outfile << "<Term Prime> -> <Factor>" << endl;
			R29();
		}
		else if (lexemes[global_index] == "true") {
			cout << "<Term Prime> -> <Factor>" << endl;
			outfile << "<Term Prime> -> <Factor>" << endl;
			R29();
		}
		else if (lexemes[global_index] == "false") {
			cout << "<Term Prime> -> <Factor>" << endl;
			outfile << "<Term Prime> -> <Factor>" << endl;
			R29();
		}
		else {
			cout << "<Term Prime> -> <Empty>" << endl;
			outfile << "<Term Prime> -> <Empty>" << endl;
			R31();
		}
	}
	else {
		cout << "<Term Prime> -> <Empty>" << endl;
		outfile << "<Term Prime> -> <Empty>" << endl;
		R31();
	}
}

void R29() {
	if (lexemes[global_index] == "-") {
		cout << "<Factor> -> -  <Primary>" << endl;
		outfile << "<Factor> -> -  <Primary>" << endl;
		PrintToken();
		global_index++;
		R30();
	}
	else {
		cout << "<Factor> -> <Primary>" << endl;
		outfile << "<Factor> -> <Primary>" << endl;
		R30();
	}
}

void R30() {
		if (tokens[global_index] == "identifier") {
			cout << "<Primary> -> <Identifier>" << endl;
			outfile << "<Primary> -> <Identifier>" << endl;
			PrintToken();
			global_index++;
			if (lexemes[global_index] == "(") {
				cout << "<Primary> -> <Identifier>  ( <ID> )" << endl;
				outfile << "<Primary> -> <Identifier>  ( <ID> )" << endl;
				PrintToken();
				global_index++;
				R13();
				if (lexemes[global_index] == ")") {
					PrintToken();
					global_index++;
				}
				else {
					cout << "ERROR: Expected a ')'" << endl;
					outfile << "ERROR: Expected a ')'" << endl;
					PrintToken();
					global_index++;
				}
			}
		}
		else if (tokens[global_index] == "integer") {
			cout << "<Primary> -> <Integer>" << endl;
			outfile << "<Primary> -> <Integer>" << endl;
			PrintToken();
			global_index++;
		}
		else if (lexemes[global_index] == "(") {
			cout << "<Primary> -> ( <Expression> )" << endl;
			outfile << "<Primary> -> ( <Expression> )" << endl;
			PrintToken();
			global_index++;
			R25();
			if (lexemes[global_index] == ")") {
				PrintToken();
				global_index++;
			}
			else {
				cout << "ERROR: Expected a ')'" << endl;
				outfile << "ERROR: Expected a ')'" << endl;
			}
		}
		else if (tokens[global_index] == "real") {
			cout << "<Primary> -> <Real>" << endl;
			outfile << "<Primary> -> <Real>" << endl;
			PrintToken();
			global_index++;
		}
		else if (lexemes[global_index] == "true") {
			cout << "<Primary> -> true" << endl;
			outfile << "<Primary> -> true" << endl;
			PrintToken();
			global_index++;
		}
		else if (lexemes[global_index] == "false") {
			cout << "<Primary> -> false" << endl;
			outfile << "<Primary> -> false" << endl;
			PrintToken();
			global_index++;
		}
		else {
			cout << "ERROR: Primary is invalid." << endl;
			outfile << "ERROR: Primary is invalid." << endl;
			PrintToken();
			global_index++;
		}
}

void R31() {
	cout << "<Empty> -> Epsilon" << endl;
	outfile << "<Empty> -> Epsilon" << endl;
}

// Our Parser function
void Parser() {
	outfile.open("Rat18S_output.txt");
	R1();
	outfile.close();
}

// Our main function
int main()
{
	Lexer();
	Parser();

	system("Pause");

	return 0;
}