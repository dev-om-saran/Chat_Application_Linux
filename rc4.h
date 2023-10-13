#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<int> S(256);		//Fill with for loop
vector<int> T(256); 		// Fill with key


vector<int> permute(vector<int>, vector<int>);
vector<int> str_to_vector(string str);
string encrypt(vector<int>s , vector<int> t, string p);
string decrypt(vector<int>s, vector<int> t, string p);

/*
int main() {

	

	int key[] = { 1,2,3,6 };
	int key2[] = { 5,7,8,9 };


	// Initilize the S and T vectors
	int tmp = 0;
	for (int i = 0; i < 256;i++) {
		S[i] = i;
		T[i] = key[( i % (sizeof(key)/sizeof(*key)) )];
	}

	S = permute(S, T);

	for (int i = 0; i < 256 ;i++) {
		cout << S[i] << " ";	
		if ((i + 1) % 16 == 0)
			cout << endl;
	}

	cout << endl;
	string p = encrypt(S, T, plaintext);
	cout << "Message: " << plaintext << endl;
	cout << "Encrypted Message: " << " " << p << endl;
	cout << "Decrypted Message:  " << decrypt(S, T, p) << endl << endl;
	
	//Init S and T
	tmp = 0;
	for (int i = 0; i < 256;i++) {
		S[i] = i;
		T[i] = key2[(i % (sizeof(key) / sizeof(*key)))];
	}

	S = permute(S, T);

	for (int i = 0; i < 256;i++) {
		cout << S[i] << " ";
		if ((i + 1) % 16 == 0)
			cout << endl;
	}

	cout << endl;
	p = encrypt(S, T, plaintext2);
	cout << "Message: " << plaintext2 << endl;
	cout << "Encrypted Message: " << p << endl;
	cout << "Decrypted Message: "<<decrypt(S, T, p) << endl << endl;


 	system("pause");
	return 0;
}

*/

vector<int> str_to_vector(string str){

	vector<int> res;
	for(int i=0;i<str.size();i++)
	res.push_back(((int)str[i] - '0'));
	
	return res;
}

void init(vector<int> key){
	for (int i = 0; i < 256;i++) 
	{
		S[i] = i;
		T[i] = key[( i % key.size())];
	}
}

void permute() {
	int j = 0;
	int tmp;
	for (int i = 0; i< 256; i++) 
	{
		j = (j + S[i] + T[i]) % 256;
		
		tmp =  S[i];
		S[i] = S[j];
		S[j] = tmp;
	}
}

string encrypt(string plain_text, string private_key) {
	
	vector<int> key = str_to_vector(private_key);
	
	init(key);
	permute();
	
	
	int i = 0;
	int j = 0;
	int tmp = 0; 
	int k = 0;

	//Temp variables
	int b;	
	int c;

	string cipher_text;
	//cout << "Keys Generated for plaintext: ";

	for (int r = 0; r < plain_text.length(); r++) {

		i = (i + 1) % 256;
		j = (j + S[i]) % 256;

		// swap section
		b = S[i];
		S[i] = S[j];
		S[j] = b;

		tmp = (S[i] + S[j]) % 256;
		k = S[tmp];

		//cout << k << " ";
		c = ((int)plain_text[r] ^ k);	//Cast p char as an int then xor with k	

		cipher_text += (char)c;  //cast int as char then append to string

	}
	//cout << endl;
	return cipher_text;
}

string decrypt(string cipher_text, string private_key) {

	vector<int> key = str_to_vector(private_key);
	
	init(key);
	permute();	
	
	int i = 0;
	int j = 0;
	int tmp = 0;
	int k = 0;


	//temp variables
	int b;
	int c;

	string plain_text;

	for (int r = 0; r < cipher_text.length(); r++) {

		i = (i + 1) % 256;
		j = (j + S[i]) % 256;

		// swap section
		b = S[i];
		S[i] = S[j];
		S[j] = b;

		tmp = (S[i] + S[j]) % 256;
		k = S[tmp];

		c = ((int)cipher_text[r] ^ k);	// cast the p string as and int then xor with k


		plain_text += (char)c;	// Cast plaintext int array as a char

	}
	return plain_text;
}





