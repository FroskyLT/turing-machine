#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <thread>
#include <windows.h>
#include <mutex>
#include <stdlib.h>

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
	void simulate_machine();
	bool find_state(vector <string> el, string state, char head) {
		if (el[0] == state && el[1][0] == head) return true;
		else return false;
	}
};

void turingMachine::simulate_machine()
{
	int index = pos;
	string tape = initialInput, newState = code[0][4];

	tape.push_back(' ');
	tape.insert(tape.begin(), ' ');
									// it[0] - currentState
	char head = tape[index];		// it[1] - currentSym
									// it[2] - newSym
	bool run = true;				// it[3] - direction
	int turn = 0;					// it[4] - newState
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
					tape[index] = it[2][0];
					//system("CLS");
					//cout << "\033[2J\033[1;1H";
					cout << tape << "\n";
					//Sleep(100);
				}

				if (it[3] == "R" && index != tape.size() - 1)	index++;
				else if (it[3] == "L" && index != 0)			index--;
				else
				{
					cout << "\n" << "error out of range " << index << "\n" << endl;
					run = false;
				}

				head = tape[index];
				found = true;
				newState = it[4];

				//cout << "next: " << it[0] << " " << it[1] << " " << it[2] << " " << it[3] << " " << it[4] << "\n" << endl;
				//cout << tape << " " << "turn: " << turn << " index: " << index << " " << head << "\n";
				//cout << "pervious: "<< it[0] << " " << it[1] << " " << it[2] << " " << it[3] << " " << it[4] << endl;

				break;
			}
		}
		if (!found) {
			run = 0;
			cout << "Halted. No rule for state " << newState << " and symbol " << head << ".\n"<<"\n";
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000) // the 'Up' arrow key is currently being held down
		{
			exit(0);
		}
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


void firstF(D1 d1)	{ mtx.lock(); d1.simulate_machine(); mtx.unlock(); }
void secondF(D2 d2) { mtx.lock(); d2.simulate_machine(); mtx.unlock(); }
void thirdF(D3 d3)	{ mtx.lock(); d3.simulate_machine(); mtx.unlock(); }
void fourthF(D4 d4) { mtx.lock(); d4.simulate_machine(); mtx.unlock(); }

int hotKey() {
	if (RegisterHotKey(NULL, 1, MOD_SHIFT, 0x52)) // SHIFT + R
	{
		cout << "Made hotkey";
	}
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (msg.message == WM_HOTKEY)
		{
			return 0;
		}
	}
}

int main()
{
	D1 d1; D2 d2; D3 d3; D4 d4;
	d1.input_values(); d2.input_values();
	d3.input_values(); d4.input_values();

	//thread first ( d1.simulate_machine);

	thread first	(firstF, d1);
	thread second	(secondF, d2);
	thread third	(thirdF, d3);
	thread fourth	(fourthF, d4);

	first.join();
	second.join();
	third.join();
	fourth.join();

	return 0;
}