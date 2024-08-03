#include<bits/stdc++.h>
using namespace std;

// 3*5*6*6*7 --> 3780%26 = 10
const int magicNumber = 10;

class Encryption {
    private:
        string message;
        vector<int> key;
    public:
        Encryption();
        vector<int> keyGeneration(int);
        void rearrangeMessage();
        void encode();
        void writeKey();
        void writeEncryptedText();
        friend void rearrangeKey(vector<int> &);
};

class Decryption {
    private:
        string encryptedText;
        string decryptedText;
        vector<int> key;
    public:
        Decryption();
        void rearrangeDecryptedText();
        void decode();
        void readKey();
        void readEncryptedText();
        friend void rearrangeKey(vector<int> &);
};


int characterShift(int front, int back) {
    int frontDigitProduct = 1, backDigitProduct = 1;
    
    while(front) {
        int lastDigit = front%10;
        frontDigitProduct *= lastDigit;
        front /= 10;
    }

    while(back) {
        int lastDigit = back%10;
        backDigitProduct *= lastDigit;
        back /= 10;
    }

    return (frontDigitProduct + backDigitProduct);
}

// reversing and swapping the alternate elements
void rearrangeKey(vector<int> &key) {
    
    reverse(key.begin(), key.end());

    for(int i=0; i<key.size()-1; i+=2) {
        if(i+1 < key.size()) {
            swap(key[i],key[i+1]);
        }
    }
}


                                        // Function Defintions of Encryption Class

 // initialising message and key, writing generated key in file                                       
Encryption::Encryption() {
    cout<<"Enter the message to be Encrypted : ";
    getline(cin,message);

    key = keyGeneration(message.length());
    writeKey();
}

// generating the key of lenght 2n, within the range of 32 to 126
vector<int> Encryption::keyGeneration(int messageLength) {
    
    vector<int> key(2*messageLength);
    
    // time is taken as seed
    srand((unsigned) time(NULL));

    for(int i=0; i<2*messageLength; i++) {
        int x = 32 + (rand()%(126-32+1));
        key[i] = x;
    }

    return key;
}

//writing key in a file
void Encryption::writeKey() {
    
    fstream keyFile;
    keyFile.open("keyFile.txt",ios::out);

    for(auto const &x : key) {
        keyFile<<x<<" ";
    }

    keyFile.close();
}

//writing encrypted text in a file
void Encryption::writeEncryptedText() {
    
    fstream encryptedFile;
    encryptedFile.open("encryptedTextFile.txt",ios::out);

    encryptedFile<<message;

    encryptedFile.close();
}

// reversing, swapping alternate characters, and adding magic number to each character
void Encryption::rearrangeMessage() {

    reverse(message.begin(),message.end());

    for(int i=0; i<message.length()-1; i+=2) {
        if(i+1 < message.length()) {
            swap(message[i],message[i+1]);
        }
    }

    // making sure each character is within the range of 32-126
    for(int i=0; i<message.length(); i++) {
        if(int(message[i]) >= 117) {
            message[i] = ((message[i]+ magicNumber)%127 + 32);
        }
        else {
            message[i] = (message[i] + magicNumber);
        }
    }
}


void Encryption::encode() {

    rearrangeMessage();
    rearrangeKey(key);

    int front = 0;
    int back = key.size()-1;

    while(front < back) {

        // here shift can take value between 0 to 162
        int shift = characterShift(key[front],key[back]);
        
        // reducing value of shift to 0 to 95
        if(shift > 95) {
            shift = shift%95 + 32;
        }

        // shifting each character's ASCII value by 'shift'
        if(message[front] + shift >= 127) {
            message[front] = (message[front] + shift)%127 + 32;
        }    
        else {
            message[front] = (message[front] + shift);
        }

        front++;
        back--;
    }

    cout<<"\nEncrypted Text is : "<<message;

    writeEncryptedText();
}

                                        // Function Defintions of Decryption Class

// reading encrypted message and original key from file to initialise the object of Decryption class
Decryption::Decryption() {
    readEncryptedText();
    readKey();
}

// reading key from file
void Decryption::readKey() {

    fstream keyFile;
    keyFile.open("keyFile.txt",ios::in);

    if(keyFile.is_open()) {
        
        int value;
        while(keyFile >> value) {
            key.push_back(value);
        }

    }
    else {
        cout<<"Couldn't open the required File!";
    }

    keyFile.close();

}

// reading encrypted message from file
void Decryption::readEncryptedText() {

    fstream file;
    file.open("encryptedTextFile.txt",ios::in);

    if(file.is_open()) {
        while(file) {
            getline(file,encryptedText);
        }
    }
    else {
        cout<<"Couldn't open the required File!";
    }

    file.close();

}


void Decryption::decode() {

    // rearranging the original key
    rearrangeKey(key);

    int front = 0;
    int back = key.size()-1;

    while(front < back) {

        // shift can vary from 0 to 162
        int shift = characterShift(key[front],key[back]);

        // reducing shift to 0 to 95
        if(shift > 95) {
            shift = shift%95 + 32;
        }

        // subtracting 'shift' from each character, because we added it during encryption
        if(encryptedText[front] - shift < 32) {
            decryptedText.push_back(127 - (32 - (encryptedText[front] - shift)));
        }
        else {
            decryptedText.push_back((encryptedText[front] - shift));
        }

        front++;
        back--;
    }

    // rearranging the decrypted text to get the original message
    rearrangeDecryptedText();

    cout<<"\n\nDecrypted Text is : "<<decryptedText;

}

// subtracting magic number, reversing and swapping alternate elements
void Decryption::rearrangeDecryptedText() {

    for(int i=0; i<decryptedText.length(); i++) {
        if(int(decryptedText[i] - magicNumber) < 32) {
            decryptedText[i] = 127 - (32 - (decryptedText[i] - magicNumber));
        }
        else {
            decryptedText[i] = decryptedText[i] - magicNumber;
        }
    }

    for(int i=0; i<decryptedText.length()-1; i+=2) {
        if(i+1 < decryptedText.length()) {
            swap(decryptedText[i],decryptedText[i+1]);
        }
    }

    reverse(decryptedText.begin(),decryptedText.end());

}

// driver function
int main() {
    
    Encryption obj;
    obj.encode();

    Decryption dd;
    dd.decode();

}