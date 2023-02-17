/*
This file is about long sequence number addition/subtraction.
Copyright (C) 2023 Zifan (Felix) Fei felix.zifan.fei@gmail.com or cakucaco@163.com
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/
/* long sequence number addition/subtraction written by Zifan (Felix) Fei <felix.zifan.fei@gmail.com> or <cakucaco@163.com> */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>

using namespace std; // For convenience. In real projects, this is not recommended.

bool isNum(const string &s);
bool isBaseValid(char *arg);
int char_to_int(char c);
void int_to_char(string &c, int n, int j);
void int_to_char_addition(string &c, int n, int j, int Base);
int find_first_nonzeros(const string &str);

int main(int argc, char **argv)
{
    string helpInfo = 
            "Usage - Proj2 required options:\n"
            "  --testFile <filename>    testfile \n"
            "  --inputBase <number>     input decimal btw 2-36 \n"
            "  --outBase <number>       output decimal btw 2-36 \n";

    if (argc < 2 || argc > 4) {
        cout << "input parameter error" << endl;
        cout << helpInfo << endl;
        exit(-1);
    }
    
    string testFilePath = argv[1];
    int inputBase, outputBase;
    vector<string> sign, numData;
    
    int flag = 0; // Denote 2nd or 3rd input is valid or not.
    if (argc == 4) {
        if (isBaseValid(argv[2]) && isBaseValid(argv[3])) {
            inputBase = stoi(string(argv[2]));
            outputBase = stoi(string(argv[3]));
        } else { flag = 1; } // flag = 1: invalid
    } else if (argc == 3) {
        if (isBaseValid(argv[2])) {
            inputBase = stoi(string(argv[2]));
            outputBase = inputBase;
        } else { flag = 1; } // flag = 1: invalid
    } else if (argc == 2) {
        inputBase = 10;
        outputBase = 10;
    } else { // Only 2, 3 or 4 inputs are acceptable.
        cout << "The number of input parameters are incorrect." << endl;
        cout << helpInfo << endl;
        exit(-2);
    }
    
    if (flag == 1) {
        cout << "The 2nd or 3rd input parameter error" << endl;
        cout << helpInfo << endl;
        exit(-2);
    }
    
    // Read the data from the file.
	ifstream fin;
    fin.open(testFilePath, ios::in);
    
    if (!fin.is_open()) {
		cout << "cannot find the file" << endl;
		return -1;
	}
    
    int count_line = 0;
    while(!fin.eof()) {
        string line;
        getline(fin, line);
        
        // Exception handling.
        if(line.size() == 0) {
            continue;
        } else if (line.substr(0, 1) == "#" || line.substr(0, 1) == "*" ||
                 line.substr(0, 1) == "!" || line.substr(0, 1) == "%") {
            cout << "inputfile is not valid" << endl;
            exit(-3);
        }
        
        // Check there is only two lines of valid sequences, no more.
        count_line++;
        if (count_line > 2) {
            cout << "inputfile has more than 2 valid sequences" << endl;
            exit(-3);
        }
        
        // Handle signs in the head of the sequence, if they exist.
        if (line.substr(0, 1) == "+" || line.substr(0, 1) == "-") {
            sign.emplace_back(line.substr(0,1));
            line = line.substr(1, line.size() - 1);
        } else {
            sign.emplace_back("+");
        }
        
        // Remove multiple 0s in the front part of the sequence, if they exist.
        int i = find_first_nonzeros(line);
        line = line.substr(i, line.size() - i);
        
        // Check each sequence whether it contains invalid numbers/characters.
        // Also check the inputbase is between 1-9 or 10-36 [A-Z(a-z)] first.
        if (inputBase <= 10) { // User should input 2-10, 1 is useless but acceptable.
            for (char const &ch : line) {
                if(ch < 48 || ch > 47 + inputBase) {
                    cout << "inputfile has invalid integers" << endl;
                    exit(-3);
                }
            }
        } else {
            for (char &ch : line) { 
            // User should input 11-36, so it allows 0-9 plus some alphabets.
            // Here we also change the lowercase to uppercase.
            if ((ch >= 48 && ch <= 57) || (ch >= 65 && ch <= 54 + inputBase)
                || (ch >= 97 && ch <= 86 + inputBase)) {
                    if(isalpha(ch) && islower(ch)) {
                        ch = toupper(ch);
                    }
            } else {
                    cout << "inputfile has invalid alphabets" << endl;
                    exit(-3);
                 }
            }
        }
        
        numData.push_back(line);
    }
    
    // Make sure the number with a bigger absolute value as the long sequence.
    // The longer one will either be added or subtracted with a smaller one.
    // If addition, then outputData will be have a size + 1 to allowed one more carry.
    // If subtraction, then outputData will copy the long sequence first.
    int shorter, longer, long_seq, short_seq;
    if(numData[0].size() >= numData[1].size()) {
        shorter = numData[1].size();
        longer = numData[0].size();
        long_seq = 0;
        short_seq = 1;
    }
    else {
        shorter = numData[0].size();
        longer = numData[1].size();
        long_seq = 1;
        short_seq = 0;
    }
    
    string outputData;
    if (sign[0] == sign[1]) { // Addition (both sign is either ++ or --).
        int num;
        outputData.resize(longer + 1, '0');
        
        for(int i = shorter - 1, j = longer - 1; i >= 0; --i, --j) {
            // Check current carry of 2 sequences whether it is A-Z.
            num = char_to_int(numData[short_seq][i]);
            num += char_to_int(numData[long_seq][j]);
            int_to_char_addition(outputData, num, j, inputBase);
        }
        
        // e.g. Based on 10, 9999991234 + 8766.
        for(int i = longer - shorter - 1; i >= 0; --i) {
            num = char_to_int(numData[long_seq][i]);
            int_to_char_addition(outputData, num, i, inputBase);
        }
        
        // Remove an additional "0" at the beginning, if it is not changed to 1.
        if(outputData[0] == '0') {
            outputData.erase(0, 1);
        }
    }
    else { /* Subtraction (sign is the opposite, +- or -+) */
        // 1. Find a bigger one, the length comparation is done. Now we need to 
        // check if the length is equal, compare every digits from the high to 
        // lower significant digit. Guarantee the longer sequence is the minuend.
        if (longer == shorter) {
            for (int i = 0; i < longer; i++) {
                if (numData[short_seq][i] < numData[long_seq][i]) {
                    break;
                }
                else if(numData[short_seq][i] > numData[long_seq][i]) {
                    int temp = long_seq;
                    long_seq = short_seq;
                    short_seq = temp;
                    break;
                }
            }
        }
        
        // 2. Copy the minuend to outputData.
        outputData = numData[long_seq];
        
        // 3. Subtraction with borrowing (regrouping).
        for(int i = 0; i < shorter; i++) {
            // Check current carry of 2 sequences whether it is A-Z.
            int num = 0;
            num = char_to_int(numData[long_seq][longer - shorter + i]);
            num -= char_to_int(numData[short_seq][i]);
            
            if (num >= 0) {
                int_to_char(outputData, num, longer - shorter + i - 1);
            }
            else {
                int j = i; // It may keep borrowing such as 10000001 - 98.
                do {
                    // 3.1 Dealing with the current carry.
                    num += inputBase;
                    int_to_char(outputData, num, longer - shorter + j - 1);
                    // 3.2 Dealing with a higher carry.
                    // -1 due to the first part is 0-9 then A-Z. If digits only,
                    // consider the special case: num = 0 -> -1
                    num = char_to_int(outputData[longer - shorter + j - 1]);
                    num -= 1;
                    j--;
                } while (num < 0);
                // 4. There must be a higher carry to guarantee it is not 0 and
                // it must be bigger, such as the first 1 in 10000001, or the
                // first 9 in the 9853 to minus 8979. Then no need to borrow.
                int_to_char(outputData, num, longer - shorter + j - 1);
            }
        }
    }
    
    int i = find_first_nonzeros(outputData);
    outputData = outputData.substr(i, outputData.size() - i);
    // If the result is 0, then set a "0" for it.
    if (outputData.empty()) {
        outputData = "0";
    }
    
    // One deque for converted output.
    deque<char> outputFinal;
    // Convert outputData based on inputBase to another radix base.
    if (argc == 4 && inputBase != outputBase) {
        // A temporary seq for division algorithm, one for temp div result.
        string temp_seq = outputData, convert;
        while(!temp_seq.empty()) {
            int q, r = 0, num, i = 0;
            while (i < temp_seq.size()) {
                // Change every char to num first.
                num = char_to_int(temp_seq[i]);
                q = (r * inputBase + num) / outputBase;
                r = (r * inputBase + num) % outputBase;
                convert += (q > 9) ? 'A' + (q - 10) : '0' + q;
                i++;
            }
            char r_ch = (r > 9) ? 'A' + (r - 10) : '0' + r;
            outputFinal.push_front(r_ch);
            
            i = find_first_nonzeros(convert);
            if(i != 0) {
                convert = convert.substr(i, convert.size() - i);
            }
            
            temp_seq = convert;
            convert = "";
        }
    }
    
    if (argc == 4 && inputBase != outputBase) {
        if (sign[long_seq] == "-") {
            cout << sign[long_seq];
        }
        for(char n : outputFinal)
            cout << n;
    }
    else if (sign[long_seq] == "-") {
        cout << sign[long_seq] << outputData << endl;
    }
    else {
        cout << outputData << endl;
    }
    
    return 0;
}

bool isNum(const string &s) {
    for (char const &ch : s) {
        if(!isdigit(ch)) {
            return false;
        }
    }
    return true;
}

bool isBaseValid(char *arg) {
    // Check whether inputBase is a valid integer.
    if (isNum(string(arg))) {
        int inputBase = stoi(string(arg));
        // Check whether inputBase is a valid number.
        if (inputBase < 1 || inputBase > 36) {
            return false;
        }
    } else {
        return false;
    }
    return true;
}

int char_to_int(char c) {
    return (isalpha(c)) ? c - 55 : c - '0';
}

void int_to_char(string &c, int n, int j) {
    c[j + 1] = (n > 9) ? 'A' + (n - 10) : '0' + n;
}

void int_to_char_addition(string &c, int n, int j, int Base) {
    // c[j + 1] is either initialized as 0 or assigned as 1.
    n += c[j + 1] - '0';
    if (n >= Base) {
        c[j] = '1';
        n -= Base;
    }
    
    int_to_char(c, n, j);
}

int find_first_nonzeros(const string &str) {
    int i = 0;
    while(str[i] == '0') {
        i++;
    }
    return i;
}
