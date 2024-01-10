#include <sstream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include "randgen.h"
#include "strutils.h"
#include <iostream>

using namespace std;

struct square {
	int b, a, c1, c2, c3, c4;
	bool went;
};

struct maze {
	vector<vector<square>> l;
};
string directionchoosing(int x, int y, int M, int N, vector<vector<square>> new_mat) {
	vector<string> directions;

	bool canGoLeft = x > 0 && new_mat[y][x].c1 == 0 && !new_mat[y][x - 1].went;
	bool canGoRight = x + 1 < N && new_mat[y][x].c2 == 0 && !new_mat[y][x + 1].went;
	bool canGoDown = y > 0 && new_mat[y][x].c4 == 0 && !new_mat[y - 1][x].went;
	bool canGoUp = y + 1 < M && new_mat[y][x].c3 == 0 && !new_mat[y + 1][x].went;

	if (canGoLeft) directions.push_back("left");
	if (canGoRight) directions.push_back("right");
	if (canGoDown) directions.push_back("down");
	if (canGoUp) directions.push_back("up");

	RandGen rand;
	int vec_size = directions.size();

	return vec_size != 0 ? directions[rand.RandInt(0, vec_size - 1)] : "dead_end";
}

string choosingwall(int x, int y, int M, int N, vector<vector<square>> mat) {
	vector<string> choicess;
	auto tryAddDirection = [&](int dx, int dy, const string& direction) {
		if (x + dx >= 0 && x + dx < N && y + dy >= 0 && y + dy < M && !mat[y + dy][x + dx].went) {
			choicess.push_back(direction);
		}
		};

	tryAddDirection(-1, 0, "left");
	tryAddDirection(1, 0, "right");
	tryAddDirection(0, -1, "down");
	tryAddDirection(0, 1, "up");

	RandGen rand;
	int vec_size = choicess.size();
	return vec_size != 0 ? choicess[rand.RandInt(0, vec_size - 1)] : "dead_end";
}






bool unvisited_cells(vector<vector<square>> mat, int M, int N) {
	return std::any_of(mat.begin(), mat.end(), [](const auto& row) {
		return std::any_of(row.begin(), row.end(), [](const square& cell) {
			return !cell.went;
			});
		});
}


template <class Object>
class Stack {

public:
	Stack();
	Stack(const Stack& rhs);
	~Stack();

	bool isEmpty() const;
	bool isFull() const;
	void makeEmpty();

	void pop();
	void push(const Object& x);
	Object topAndPop();
	Object& top() const;

	const Stack& operator=(const Stack& rhs);

private:
	struct ListNode {
		Object element;
		ListNode* next;

		ListNode(const Object& theElement, ListNode* n = NULL)
			: element(theElement), next(n) {}
	};

	ListNode* upmoststack;  
};

template <class Object>
Stack<Object>::Stack() { upmoststack = NULL; }

template <class Object>
bool Stack<Object>::isFull() const { return false; }

template <class Object>
bool Stack<Object>::isEmpty() const { return upmoststack == NULL; }


template <class Object>
Object& Stack<Object>::top() const {

	if (!isEmpty()) { return upmoststack->element; }

}
template <class Object>
void Stack<Object>::push(const Object& x) {

	upmoststack = new ListNode(x, upmoststack);
}

template <class Object>
void Stack<Object>::pop() {

	if (!isEmpty()) {

		ListNode* oldie = upmoststack;
		upmoststack = upmoststack->next;
		delete oldie;
	}

}



template <class Object>
void Stack<Object>::makeEmpty() {

	while (!isEmpty()) { pop(); }
}

template <class Object>
Object Stack<Object>::topAndPop() {

	Object Item = top();
	pop();
	return Item;
}




template <class Object>
const Stack<Object>& Stack<Object>::operator=(const Stack<Object>& rhs) {
	if (this != &rhs) {
		makeEmpty();

		if (!rhs.isEmpty()) {
			
			ListNode* right_ptr = rhs.upmoststack;
			ListNode* this_ptr = new ListNode(right_ptr->element);
			upmoststack = this_ptr;

			
			while (right_ptr->next != NULL) {
				right_ptr = right_ptr->next;
				this_ptr->next = new ListNode(right_ptr->element);
				this_ptr = this_ptr->next;
			}
		}
	}
	return *this;
}



template <class Object>
Stack<Object>::Stack(const Stack<Object>& rhs) {

	upmoststack = NULL;
	*this = rhs; 
}

//Destructor
template <class Object>
Stack<Object>::~Stack() { makeEmpty(); }

int main() {

	int rows, columns, mazeNumber;
	int x1, y1, x2, y2;
	int ID;

	
	cout << "Enter the number of mazes: ";
	cin >> mazeNumber;
	while (cin.fail() || mazeNumber <= 0) {
		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		cout << "Invalid input. Please enter a positive integer for the number of mazes: ";
		cin >> mazeNumber;
	}


	cout << "Enter the number of rows and columns (M and N): ";
	cin >> rows >> columns;
	while (cin.fail() || rows <= 0 || columns <= 0) {
		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		cout << "Invalid input. Please enter positive integers for rows and columns: ";
		cin >> rows >> columns;
	}

	vector<vector<vector<square>>> mazes;
	for (int o = 0; o < mazeNumber; o++) {
		vector<vector<square>> mat(rows, vector<square>(columns));
		square current_cell{};
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				current_cell.a = i;
				current_cell.b = j;
				current_cell.c1, current_cell.c2 = 1,current_cell.c3 = 1,current_cell.c4 = 1;
				current_cell.went = false;
				mat[i][j] = current_cell;

			}
		}
		square CurrentCell = mat[0][0];
		Stack<square> myStack;
		myStack.push(CurrentCell);
		mat[CurrentCell.a][CurrentCell.b].went = true;
		while (unvisited_cells(mat, rows, columns)) {
			string direct = choosingwall(CurrentCell.b, CurrentCell.a, rows, columns, mat);
			bool hasElements = !myStack.isEmpty();

			
			if (direct == "left" || direct == "right" || direct == "up" || direct == "down") {
				
				int dRow = (direct == "up") ? 1 : (direct == "down") ? -1 : 0;
				int dCol = (direct == "left") ? -1 : (direct == "right") ? 1 : 0;


				mat[CurrentCell.a][CurrentCell.b].c1 -= dCol; 
				mat[CurrentCell.a][CurrentCell.b].c2 += dCol; 
				mat[CurrentCell.a][CurrentCell.b].c3 += dRow; 
				mat[CurrentCell.a][CurrentCell.b].c4 -= dRow; 

			
				CurrentCell = mat[CurrentCell.a + dRow][CurrentCell.b + dCol];
			}
			else {
				
				bool hasElements = !myStack.isEmpty(); 
				while (hasElements && choosingwall(CurrentCell.b, CurrentCell.a, rows, columns, mat) == "dead_end") {
					myStack.pop();
					hasElements = !myStack.isEmpty();
					if (hasElements) {
						CurrentCell = myStack.top();
					}
					else {
						break; 
					}
				}
			}


			if (direct != "dead_end" || hasElements) {
				myStack.push(CurrentCell);
				mat[CurrentCell.a][CurrentCell.b].went = true;
			}
		}

		string ID = itoa(o + 1);
		string filename = "maze_" + ID + ".txt";
		ofstream myFile;
		myFile.open(filename.c_str());
		myFile << rows << " " << columns << endl;
		for (int k = 0; k < rows; k++) {
			for (int p = 0; p < columns; p++) {
				square currentCell = mat[k][p];
				myFile << "x=" << currentCell.b << " y=" << currentCell.a << " l=" << currentCell.c1 << " r="
					<< currentCell.c2 << " u=" << currentCell.c3 << " d=" << currentCell.c4 << endl;
			}
		}
		mazes.push_back(mat);
		myFile.close();
	}


	cout << "All mazes are created." << endl;


	cout << "Enter a maze ID between 1 to " << mazeNumber << " inclusive to find a path: ";
	cin >> ID;
	while (cin.fail() || ID < 1 || ID > mazeNumber) {
		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		cout << "Invalid input. Please enter a maze ID between 1 to " << mazeNumber << " inclusive: ";
		cin >> ID;
	}


	cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
	cin >> x1 >> y1;
	while (cin.fail() || x1 < 0 || x1 >= columns || y1 < 0 || y1 >= rows) {
		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		cout << "Invalid coordinates. Please enter valid x and y coordinates for entry points: ";
		cin >> x1 >> y1;
	}


	cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
	cin >> x2 >> y2;
	while (cin.fail() || x2 < 0 || x2 >= columns || y2 < 0 || y2 >= rows) {
		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		cout << "Invalid coordinates. Please enter valid x and y coordinates for exit points: ";
		cin >> x2 >> y2;
	}

	
	vector<vector<square>> new_mat = mazes[ID - 1];


	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			new_mat[i][j].went = false;
		}
	}

	square currentCell = new_mat[y1][x1];
	square LastCell = new_mat[y2][x2];
	Stack<square> pointer;
	pointer.push(currentCell);
	new_mat[currentCell.a][currentCell.b].went = true;

	while (!((currentCell.b == LastCell.b) && (currentCell.a == LastCell.a))) {
		string way_to_go = directionchoosing(currentCell.b, currentCell.a, rows, columns, new_mat);

		
		if (way_to_go == "left") {
			currentCell = new_mat[currentCell.a][currentCell.b - 1];
		}
		else if (way_to_go == "right") {
			currentCell = new_mat[currentCell.a][currentCell.b + 1];
		}
		else if (way_to_go == "up") {
			currentCell = new_mat[currentCell.a + 1][currentCell.b];
		}
		else if (way_to_go == "down") {
			currentCell = new_mat[currentCell.a - 1][currentCell.b];
		}
		else { 
			while (!pointer.isEmpty() && directionchoosing(currentCell.b, currentCell.a, rows, columns, new_mat) == "dead_end") {
				pointer.pop();
				if (!pointer.isEmpty()) {
					currentCell = pointer.top();
				}
				else {
					break; 
				}
			}
			continue; 
		}

		pointer.push(currentCell);
		new_mat[currentCell.a][currentCell.b].went = true;
		currentCell.went = true;
	}


	string nameofthefile = "maze_" + to_string(ID) + "_path_" + to_string(x1) + "_" + to_string(y1) + "_" + to_string(x2) + "_" + to_string(y2) + ".txt";
	ofstream path_to_file(nameofthefile); 

	if (!path_to_file.is_open()) {
		cerr << "Error opening file: " << nameofthefile << endl;
	}

	Stack<square> stackToCoordinate;
	square temporary_cell;

	while (!pointer.isEmpty()) {
		temporary_cell = pointer.top();
		stackToCoordinate.push(temporary_cell);
		pointer.pop();
	}


	while (!stackToCoordinate.isEmpty()) {
		temporary_cell = stackToCoordinate.top();
		path_to_file << temporary_cell.b << " " << temporary_cell.a << endl;
		stackToCoordinate.pop();
	}

	path_to_file.close();


	return 0;
}