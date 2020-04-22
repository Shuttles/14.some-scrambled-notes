#include <iostream>
#include <fstream>
#include <string>
using namespace std;


const int MAX_CHAR = 128;
int leaf = 0, root, bytelen;
char byte;


typedef struct Hnode{
	int weight;
	int father = -1, right = -1, left = -1;
	string code;
	int HuffmenLen = 0;
}HT;
HT Huffman_Node[MAX_CHAR * 2];

void file_count();	//对文件字符的计数，得出每个字符的频率 
void init_Huffman();	//构建哈夫曼数 
void init_Code(int p, string now_Code);	// 构建哈夫曼编码 
void save_Huffman();	//将构建好的哈夫曼编码保存在文件中 
void addbit(char c);	//在
void byteclear();
void huffman_Compress(); 

int main() { 
	file_count();
	init_Huffman();
	init_Code(root, "0");
	save_Huffman();
	huffman_Compress();
	return 0;
} 

void file_count() {
	ifstream file("file.txt");
	if (!file.is_open()) cout << "error" << endl;
	while (!file.eof()) {
		char c;
		file.get(c);
		if (Huffman_Node[(int)c].weight == 0) leaf++;
		Huffman_Node[(int)c].weight++;
	}
	file.close();
	return ;
}

void init_Huffman() {
	for (int i = MAX_CHAR; i < 2 * MAX_CHAR; i++) {
		int loc1 = -1, loc2 = -1;
		for (int j = 0; j < i; j++) {
			if (Huffman_Node[j].father != -1 || Huffman_Node[j].weight == 0) continue;
			if (loc1 == -1 || (Huffman_Node[j].weight < Huffman_Node[loc1].weight)) {
				loc2 = loc1;
				loc1 = j;
			} else if (loc2 == -1 || Huffman_Node[j].weight < Huffman_Node[loc2].weight) 
				loc2 = j;
		}
		if (loc2 == -1) break;
		Huffman_Node[i].weight = Huffman_Node[loc1].weight + Huffman_Node[loc2].weight;
		Huffman_Node[i].left = loc1 > loc2 ? loc2 : loc1;
		Huffman_Node[i].right = loc1 < loc2 ? loc2 : loc1;
		Huffman_Node[loc2].father = i;
		Huffman_Node[loc1].father = i;
		root = i;
	}
}

void init_Code(int p, string now_Code) {
	if (!Huffman_Node[p].weight) return ;
	Huffman_Node[p].HuffmenLen++;
	Huffman_Node[p].HuffmenLen += Huffman_Node[Huffman_Node[p].father].HuffmenLen;
	if (Huffman_Node[p].right == -1 && Huffman_Node[p].left == -1) {
		Huffman_Node[p].code = now_Code;
		return ;
	} 
	init_Code(Huffman_Node[p].left, now_Code + "0");
	init_Code(Huffman_Node[p].right, now_Code + "1");
	return ;
}

void save_Huffman() {
	ofstream file("HufTree.txt", ios_base::out | ios_base::trunc);
	for (int i = 0; i < 2 * MAX_CHAR; i++) {
		if (Huffman_Node[i].weight) {
			if (Huffman_Node[i].left == -1) {
				file << (char)i << ' ' << Huffman_Node[i].weight << ' ' << Huffman_Node[i].code << endl;
			} else {
				file << Huffman_Node[i].weight << ' ' << Huffman_Node[i].left << ' ' << Huffman_Node[i].right << endl;
			}
		}
	}
	file.close();
	return ;
}

void addbit(char c) {
	if (c == '0') byte = byte << 1;
	else byte = (byte << 1) | 1;
	bytelen++;
	return ;
}

void byteclear() {
	bytelen = 0;
	byte = 0;
	return ;
	
	
}

void huffman_Compress() {
	ifstream input;
	ofstream output;
	input.open("file.txt");
	output.open("HufCode.txt", ios_base::out | ios_base::trunc);
	char c;
	while (input.get(c)) {
		for (int i = 0; i < Huffman_Node[c].HuffmenLen; i++) {
			if ((Huffman_Node[c].code)[i] == '0') addbit('0');
			else addbit('1');
			if (bytelen == 8) {
				output.put(byte);
				byteclear();
			}
		}
	}
}

