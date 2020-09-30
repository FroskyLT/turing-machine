#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <thread>
#include <windows.h>
#include <mutex>

using namespace std;

const char d1[] = "1.txt"; // bin to hex
const char d2[] = "2.txt"; // add bin
const char d3[] = "3.txt"; // increment bin
const char d4[] = "4.txt"; // running

mutex mtx;

class turingMachine {
private:
	int pos = 0;
	string initialInput;
	vector<vector<string>> code;
public:
	void set_values(int pos, string initialInput, vector<vector<string>> code) {
		this->pos = pos;
		this->initialInput = initialInput;
		this->code = code;
	}
	void simulate_machine(SHORT x = 1, SHORT y = 10);
	bool find_state(vector <string> el, string state, char head) {
		if (el[0] == state && el[1][0] == head) return true;
		else return false;
	}
};

void turingMachine::simulate_machine(SHORT x, SHORT y)
{
	int index = pos;
	string tape = initialInput, newState = code[0][4];

	tape.push_back(' ');
	tape.insert(tape.begin(), ' ');
										// it[0] - currentState
	char head = tape[index];			// it[1] - currentSym
										// it[2] - newSym
	bool run = true;					// it[3] - direction
	int turn = 0;						// it[4] - newState
	while (run)
	{
		bool found = false;
		turn++;
		for (auto it : code)
		{
			if (find_state(it, newState, head))
			{

				if (tape[index] != it[2][0])
				{
					mtx.lock();

					tape[index] = it[2][0];
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y });
					cout << tape << "\n";

					mtx.unlock();
					Sleep(50);
				}

				if (it[3] == "R" && index != tape.size() - 1)	index++;
				else if (it[3] == "L" && index != 0)			index--;
				else
				{
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y });
					printf("%c[2K", 27);
					cout << "\n" << "error out of range " << index << "\n" << endl;
					run = false;
				}

				head = tape[index];
				found = true;
				newState = it[4];

				break;
			}
		}
		if (!found) {
			run = 0;
			mtx.lock();
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y });
			printf("%c[2K", 27); 
			cout << "Halted. No rule for state " << newState << " and symbol " << head << ".\n"<<"\n";
			mtx.unlock();
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000) run = 0;		// the 'Up' arrow key is currently being held down
	}
}

class D1 : public turingMachine {
public:
	void input_values() {
		ifstream fd(d1);
		int pos;
		string initialInput;
		vector<string> row(5);
		vector<vector<string>> code;

		fd >> initialInput;
		fd >> pos;

		while (fd >> row[0] >> row[1] >> row[2] >> row[3] >> row[4])
		{
			code.push_back(row);
		}

		set_values(pos, initialInput, code);

		fd.close();
	}
};
class D2 : public turingMachine {
public:
	void input_values() {
		ifstream fd(d2);
		int pos;
		string initialInput;
		vector<string> row(5);
		vector<vector<string>> code;

		fd >> initialInput;
		fd >> pos;

		while (fd >> row[0] >> row[1] >> row[2] >> row[3] >> row[4])
		{
			code.push_back(row);
		}

		set_values(pos, initialInput, code);

		fd.close();
	}
};
class D3 : public turingMachine {
public:
	void input_values() {
		ifstream fd(d3);
		int pos;
		string initialInput;
		vector<string> row(5);
		vector<vector<string>> code;

		fd >> initialInput;
		fd >> pos;

		while (fd >> row[0] >> row[1] >> row[2] >> row[3] >> row[4])
		{
			code.push_back(row);
		}

		set_values(pos, initialInput, code);

		fd.close();
	}
};
class D4 : public turingMachine {
public:
	void input_values() {
		ifstream fd(d4);
		int pos;
		string initialInput;
		vector<string> row(5);
		vector<vector<string>> code;

		fd >> initialInput;
		fd >> pos;

		while (fd >> row[0] >> row[1] >> row[2] >> row[3] >> row[4])
		{
			code.push_back(row);
		}

		set_values(pos, initialInput, code);

		fd.close();
	}
};


void firstF(D1 d1)	{ d1.simulate_machine(1, 10); }
void secondF(D2 d2) { d2.simulate_machine(1, 11); }
void thirdF(D3 d3)	{ d3.simulate_machine(1, 12); }
void fourthF(D4 d4) { d4.simulate_machine(1, 13); }
void allF() {
	D1 d1; D2 d2; D3 d3; D4 d4;
	d1.input_values(); d2.input_values();
	d3.input_values(); d4.input_values();

	thread first	(firstF, d1);
	thread second	(secondF, d2);
	thread third	(thirdF, d3);
	thread fourth	(fourthF, d4);

	first.join();
	second.join();
	third.join();
	fourth.join();
}


int main()
{
	D1 d1; D2 d2; D3 d3; D4 d4;
	d1.input_values(); d2.input_values();
	d3.input_values(); d4.input_values();
	while (true)
	{
		system("CLS");
		cout << "________Turing simulator_________" << endl;
		cout << "Choose which file you want start:" << endl;
		cout << "1) 1.txt"							<< endl;
		cout << "2) 2.txt"							<< endl;
		cout << "3) 3.txt"							<< endl;
		cout << "4) 4.txt"							<< endl;
		cout << "5) All together"					<< endl;
		cout << "6) Exit"							<< endl;
		cout << "_________________________________" << endl;

		int x;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 9 });
		cin >> x;

		switch (x)
		{
		case 1: d1.simulate_machine(); break;
		case 2: d2.simulate_machine(); break;
		case 3: d3.simulate_machine(); break;
		case 4: d4.simulate_machine(); break;
		case 5:	 allF();			   break;
		case 6:	 exit(0);			   break;
		default: cout << "incorrect value\n"; exit(0);
		}

		cout << "Wait a second..";
		Sleep(3000);
	}

	return 0;
}

//raylib SetConsoleCursorPosition