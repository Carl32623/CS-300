//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Carl LaLonde
// Date		   : 8/18/2024
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Project 2 ABCU Binary Search Tree
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

//Structure to hold course information
struct Course {
	string courseNumber;  //unique identifier
	string courseName;
	vector<string> prerequisites;

	//Constructors
	Course() {}; //empty object
	Course(string number, string name) {
		courseNumber = number;
		courseName = name;
	}
};

//Internal stucture for tree node
struct Node {
	Course course;
	Node* left;  //pointer to left child
	Node* right; // pointer to right child

	//default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	//Initialize with a course
	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};

//Binary Search Tree class to manage courses
class BinarySearchTree {
private:
	Node* root;
	void addNode(Node* node, Course course);  //function to add node to tree
	void inOrder(Node* node) const;			  // function to traverse the tree in order
	

public:
	BinarySearchTree();  //constructor
	void inOrder() const; //function to print courses in order
	void insert(Course course); //function to insert new course into tree
	Course search(string courseNumber) const;  //function to search for a course by course number
};

//Default constructor
BinarySearchTree::BinarySearchTree() {
	root = nullptr; //initialize root to null
}

//Recursive function to add a course to a node
void BinarySearchTree::addNode(Node* node, Course course) {
	//if new course number is less than current node course number
	if (node != nullptr && node->course.courseNumber.compare(course.courseNumber) > 0) {
		if (node->left == nullptr) {
			node->left = new Node(course);
			return;
		}
		else {
			this->addNode(node->left, course);
		}
	}
	else {
		//if new course number is greater than current node course number
		if (node != nullptr && node->course.courseNumber.compare(course.courseNumber) < 0) {
			if (node->right == nullptr) {
				node->right = new Node(course);
				return;
			}
			else {
				this->addNode(node->right, course);
			}
		}
	}

}

//function to insert a course
void BinarySearchTree::insert(Course course) {
	if (root == nullptr) {
		//if tree is empty, new course becomes root
		root = new Node(course);
	}
	else {
		//else, recursively addNode process from root
		this->addNode(root, course);
	}
}

//Recursively traverse tree in order
void BinarySearchTree::inOrder(Node* node) const {
	if (node != nullptr) {
		inOrder(node->left); //visit left side
		cout << node->course.courseNumber << ", " << node->course.courseName << endl;
		inOrder(node->right); // visit right side
	}
}

//Traverse the tree in order from root
void BinarySearchTree::inOrder() const {
	inOrder(root);
}

//Convert case to upper
string toUpper(string str) {
	for (int i = 0; i < str.length(); i++) {
		str.at(i) = toupper(str.at(i));  //converts each character to upper case.
	}
	return str;
}

//Search for a course
Course BinarySearchTree::search(string courseNumber) const {
	courseNumber = toUpper(courseNumber);
	Node* currNode = root;

	//traverse tree to find course
	while (currNode != nullptr) {
		string nodeCourseNumber = toUpper(currNode->course.courseNumber);
		if (nodeCourseNumber.compare(courseNumber) == 0) {
			return currNode->course; // coure found
		}
		else if (courseNumber.compare(nodeCourseNumber) < 0) {
			currNode = currNode->left; //go left side
		}
		else {
			currNode = currNode->right; // go right side
		}
	}
	return Course(); //return empty course if not found
}

//Load couses from csv file
void loadCourses(string csvPath, BinarySearchTree* bst) {
	ifstream file(csvPath); //open file
	string line;

	if (!file.is_open()) {	//if file could not be opened
		cout << "Error opening file: " << csvPath << endl;
		return;
	}
	cout << "Loading file: " << csvPath << endl << endl; //if file was opened successfully

	while (getline(file, line)) {
		stringstream ss(line);
		string courseNumber, courseName, prerequisites;

		getline(ss, courseNumber, ','); //get course number
		getline(ss, courseName, ','); //get course name

		Course course(courseNumber, courseName);

		//get prereqs for course
		while (getline(ss, prerequisites, ',')) {
			prerequisites = toUpper(prerequisites); //convert to upper case
			prerequisites.erase(remove(prerequisites.begin(), prerequisites.end(), ' '), prerequisites.end()); //removes any whitespaces
			if (!prerequisites.empty()) { //makes sure prerequisites string is empty
				course.prerequisites.push_back(prerequisites); //adds prereq to courses prereq vector
			}
		}
		//insert course into tree
		bst->insert(course);

	}
	file.close(); //close the file

}

//Function to display the main menu
void displayMenu() {
	cout << "Welcome to the course planner." << endl << endl;
	cout << "1. Load Data Structure." << endl;
	cout << "2. Print Course List. " << endl;
	cout << "3. Print Courses." << endl;
	cout << "9. Exit" << endl << endl;
}

//Main Method
int main() {
	BinarySearchTree* bst = new BinarySearchTree(); //create new bst
	Course course;

	int choice = 0;
	string fileName = "CS 300 ABCU_Advising_Program_Input.csv";
	string courseNumber;

	//main loop to handle menu choice
	while (choice != 9) {
		displayMenu();
		cout << "What would you like to do?" << endl;
		string input;
		cin >> input;
		cout << endl;

		//input must be a single digit
		if (input.length() == 1 && isdigit(input[0])) {
			choice = stoi(input);
		}
		else {
			cout << input << " is not an option." << endl << endl;
			continue;
		}
		//switch statement to handle user choice
		switch (choice) {
		case 1: {		//load courses from file
			cout << "Please enter the file name to load data:" << endl;
			cin.ignore();  //clear input buffer
			getline(cin, fileName);  //get full path including spaces
			loadCourses(fileName, bst);  //calls function to load courses into bst
			cout << "Courses loaded successfully." << endl << endl;
			break;
		}
		case 2: {		//print course list in alphanumeric order
			bst->inOrder();
			cout << endl;
			break;
		}
		case 3: {		//search and print a single courses information
			cout << "What course would you like to know about?" << endl;
			cin >> courseNumber;
			cout << endl;
			Course course = bst->search(courseNumber);  //search tree by course number
			if (course.courseNumber.empty()) {  //if course is empty
				cout << "Course does not exist." << endl << endl;
			}
			else {	//if course is not empty
				cout << course.courseNumber << ", " << course.courseName << endl;
				cout << "Prerequisites: ";
				if (course.prerequisites.empty()) {//if course does not have any prereqs
					cout << "None";
				}
				else {
					for (const auto& prereq : course.prerequisites) { //if course has prereqs
						cout << prereq << " ";
					}
				}
				cout << endl << endl;
			}
			break;
		}
		case 9: {		//exit program
			cout << "Thank you for using the course planner." << endl << endl;
			return 0;
		}
		default: {
			cout << choice << " is not an option." << endl << endl;  
			break;
		}
		}
	}
	return 0;
}
