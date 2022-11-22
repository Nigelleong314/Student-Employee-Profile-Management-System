#include	<iostream>
#include 	<fstream>
#include	<string>
#include	<cstdlib>
#include	"List.h"
#include	"Student.h"

using namespace std;

bool BuildStuList(char *, List *);
bool DeleteRecord(List *, char *);
bool Display(List, int);
bool InsertResult(char *, List *);
bool getAverageCGPAByCourse(List);
bool filterStudent(List, List *, char *, int, int);
bool findEligibleFYPStudent(List, List *);
bool identifyGoodPoorStudent(List, List *, List *);
int menu();

Student s[50];
Node *cur;
int c = 0; //student array count

int main() {
	List l, l2, FYPlist, goodList, poorList;
	char stxt[] = "student.txt";
	char etxt[] = "exam.txt";
	int source, year, totalcredit, choice;
	char c[3];

	do {
		choice = menu();
		switch (choice) {
		case 1: {
			if (BuildStuList(stxt, &l)) { //function return result
				cout << "File read successfully." << endl;
			}
			else {
				cout << "Unable to open file." << endl;
			}
			break;
		}
		case 2: {
			if (DeleteRecord(&l, s->id)) {
				cout << "Record deleted." << endl;
			}
			else {
				cout << "Record not found." << endl;
			}
			break;
		}
		case 3: {
			if (!InsertResult(etxt, &l)) { //check if function return false
				cout << "List is empty." << endl;
			}
			break;
		}
		case 4: {
			cout << "Display to screen (source = 1) or file (source = 2)\nEnter source: ";
			do {
				cin >> source;
			} while (source != 1 && source != 2); //validation
			if (!Display(l, source)) {
				cout << "List is empty." << endl;
			}
			break;
		}
		case 5: {
			if (!getAverageCGPAByCourse(l)) {
				cout << "List is empty." << endl;
			}
			break;
		}
		case 6: {
			cout << "Course: ";
			cin >> c;
			cout << "Year: ";
			cin >> year;
			cout << "Minimum total credit: ";
			cin >> totalcredit;
			if (filterStudent(l, &l2, c, year, totalcredit)) {
				Display(l2, 1);
			}
			else {
				cout << "List1 is empty or List2 is not empty." << endl;
			}
			break;
		}
		case 7: {
			if (!findEligibleFYPStudent(l, &FYPlist)) {
				cout << "List1 is empty." << endl;
			}
			if (FYPlist.empty()) { //check if list is empty
				cout << "There is no student that is eligible to take FYP." << endl;
			}
			else {
				Display(FYPlist, 1);
			}
			break;
		}
		case 8: {
			if (!identifyGoodPoorStudent(l, &goodList, &poorList)) {
				cout << "List1 is empty." << endl;
			}
			if (goodList.empty()) {
				cout << "There is no student in good list." << endl;
			}
			else {
				Display(goodList, 1);
			}
			if (poorList.empty()) {
				cout << "There is no student in poor list." << endl;
			}
			else {
				Display(poorList, 1);
			}
			break;
		}
		case 9: {
			cout << "Program exited." << endl;
			break;
		}
		default: break;
		}
	} while (choice != 9); //loop choices
	system("pause");
	return 0;
}

bool BuildStuList(char *filename, List *list) {
	system("cls");
	ifstream stxt(filename); //read from student file
	if (!stxt) { //check if student.txt is found
		return 0;
	}
	int dc = 0; //duplicate count
	while (!stxt.eof()) {
		string a; //remove unnecessary data
		stxt >> a >> a >> a >> s[c].id >> a >> a;
		stxt.ignore(); //remove space in front
		stxt.getline(s[c].name, 20);
		stxt >> a >> a >> s[c].course >> a >> a >> a >> s[c].phone_no;
		getline(stxt, a, 'S'); //skip empty lines
		cur = list->head;
		bool dupe = 0; //duplicate checker
		for (int y = 0; y < c; y++) {
			if (s[c].compareID(s[y])) { //compare current with previous array
				dupe = 1;
				dc++;
			}
		}
		if (!dupe) { //insert to list when no duplicate
			list->insert(s[c]);
			c++; //increase count for student array
		}
	}
	stxt.close();
	if (dc) {
		cout << dc << " duplicate(s) found and excluded. "; //output duplicate count
	}
	return 1;
}

bool DeleteRecord(List *list, char *id) {
	system("cls");
	char sid[10];
	cout << "Delete student id: ";
	cin >> sid;
	int x = 1; //initial node number
	cur = list->head; //set to first node
	while (cur != NULL) { //loop as long as there is node
		if (!strcmp(sid, cur->item.id)) {
			list->remove(x); //remove node using node number
			return 1;
		}
		cur = cur->next; //set to next node
		x++; //increase node number
	}
	return 0;
}

bool Display(List list, int source) {
	if (list.empty()) {
		return 0;
	}
	cur = list.head;
	if (source == 1) {
		for (int x = 1; cur != NULL; x++) {
			cout << "\nStudent " << x << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
			cur->item.print(cout); //output list to screen
			cout << "\nPast exam result --------------------------------------------------------------------";
			for (int x = 0; x < cur->item.exam_cnt; x++) {
				cur->item.exam[x].print(cout);
			}
			if (cur->item.exam_cnt == 0) {
				cout << "\n\nTHIS STUDENT HAVEN'T TAKEN ANY EXAM YET\n\n";
			}
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
			cur = cur->next;
		}
	}
	else if (source == 2) {
		ofstream ostxt("student_result.txt");
		for (int x = 1; cur != NULL; x++) {
			ostxt << "\nStudent " << x << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
			cur->item.print(ostxt); //output list to file
			ostxt << "\nPast exam result --------------------------------------------------------------------";
			for (int x = 0; x < cur->item.exam_cnt; x++) {
				cur->item.exam->print(ostxt);
			}
			if (cur->item.exam_cnt == 0) {
				ostxt << "\n\nTHIS STUDENT HAVEN'T TAKEN ANY EXAM YET\n\n";
			}
			cur = cur->next;
			ostxt << "Student " << x << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
		}
		ostxt.close();
	}
	return 1;
}

bool InsertResult(char *filename, List *list) {
	ifstream etxt(filename);
	if (!etxt) {
		return 0;
	}
	for (int x = 0; x < c; x++) { //remove previous values in case readded
		s[x].exam_cnt = 0;
	}
	for (int y; !etxt.eof(); s[y].exam_cnt++) {
		char sid[10];
		etxt >> sid;
		for (y = 0; y < c; y++) { //loop within student count
			if (!strcmp(sid, s[y].id)) { //check if student id same as in list
				break; //use current student count, y
			}
		}
		etxt >> s[y].exam[s[y].exam_cnt].trimester >> s[y].exam[s[y].exam_cnt].year >> s[y].exam[s[y].exam_cnt].numOfSubjects;
		for (int z = 0; z < s[y].exam[s[y].exam_cnt].numOfSubjects; z++) { //loop insert subject depend on the number of subjects
			etxt >> s[y].exam[s[y].exam_cnt].sub[z].subject_code >> s[y].exam[s[y].exam_cnt].sub[z].subject_name
				>> s[y].exam[s[y].exam_cnt].sub[z].credit_hours >> s[y].exam[s[y].exam_cnt].sub[z].marks;
		}
		s[y].exam[s[y].exam_cnt].calculateGPA(); //calculate GPA
	}
	for (int x = 0; x < c; x++) {
		if (s[x].exam_cnt) {
			s[x].calculateCurrentCGPA(); //calculate CGPA
		}
		cur = list->head;
		for (int y = 1; y <= list->count; y++) { //set data into list
			if (!strcmp(cur->item.id, s[x].id)) { //compare to see if list and array same
				list->set(y, s[x]);
			}
			cur = cur->next;
		}
	}
	return 1;
}

bool getAverageCGPAByCourse(List list) {
	if (list.empty()) {
		return 0;
	}
	char c[5][3] = { "CS","IA","IB","CN","CT" }; //FICT students
	cout << "Course\tAverage\tCGPA\tNo. of Students\tNo. of Students\n\t\t\tBelow Average\tAbove Average" << endl;
	for (int y = 0; y < 5; y++) { //search for respective courses
		double total = 0; //cumulative CGPA
		int above = 0, below = 0, freq = 0;
		cur = list.head;
		while (cur != NULL) {
			if (!strcmp(c[y], cur->item.course)) { //check if courses are the same
				total += cur->item.current_cgpa; //adding up to total
				freq++;
			}
			cur = cur->next;
		}
		if (!freq) { //skip output if found no student in course
			continue;
		}
		cur = list.head;
		while (cur != NULL) {
			if (!strcmp(c[y], cur->item.course)) { //splitting student by their cgpa
				if (cur->item.current_cgpa < total / freq) { // average = total / freq
					below++;
				}
				else {
					above++;
				}
			}
			cur = cur->next;
		}
		cout << fixed << setprecision(5) << "  " << c[y] << "\t  " << total / freq << "\t\t" << below << "\t\t" << above << endl;
	}
	return 1;
}

bool filterStudent(List list1, List *list2, char *course, int year, int totalcredit) {
	if (list1.empty() || !list2->empty()) {
		return 0;
	}
	cur = list1.head;
	while (cur != NULL) {
		int y, y1, y2;
		y1 = cur->item.id[0] - '0'; //get decade
		y2 = cur->item.id[1] - '0'; //get year
		y = 2000 + y1 * 10 + y2; //get YYYY fornat
		if (!strcmp(course, cur->item.course) && year == y && cur->item.totalCreditsEarned >= totalcredit) { //check if match all conditions
			list2->insert(cur->item);
		}
		cur = cur->next;
	}
	return 1;
}

bool findEligibleFYPStudent(List list1, List *FYPlist) {
	if (list1.empty()) {
		return 0;
	}
	cur = list1.head;
	while (cur != NULL) {
		bool c1 = 0, c2 = 0;
		for (int x = 0; x < cur->item.exam_cnt; x++) { //loop exam list
			for (int y = 0; y < cur->item.exam[y].numOfSubjects; y++) { //loop subkect list within exam
				if (!strcmp(cur->item.exam[x].sub[y].subject_code, "UCCD2502")) { //finding by course code
					if (strcmp(cur->item.exam[x].sub[y].getGrade(), "F") && strcmp(cur->item.exam[x].sub[y].getGrade(), "N/A")) { //check no student fail or not taken exam
						c1 = 1; //condition 1 met
					}
				}
				if (!strcmp(cur->item.exam[x].sub[y].subject_code, "UCCD2513")) {
					if (strcmp(cur->item.exam[x].sub[y].getGrade(), "F") && strcmp(cur->item.exam[x].sub[y].getGrade(), "N/A")) {
						c2 = 1; //condition 2 met
					}
				}
			}
		}
		if (cur->item.totalCreditsEarned >= 30 && c1 && c2) { //insert when all condition met
			FYPlist->insert(cur->item);
		}
		cur = cur->next;
	}
	return 1;
}

bool identifyGoodPoorStudent(List list1, List *goodList, List *poorList) {
	if (list1.empty()) {
		return 0;
	}
	cur = list1.head;
	while (cur != NULL) {
		if (cur->item.exam_cnt < 3) { //skip when taken lesser than 3 exam
			cur = cur->next;
			continue;
		}
		else {
			int gc = 0, pc = 0; //good student and poor student count
			for (int x = 0; x < cur->item.exam_cnt; x++) {
				bool g = 1, p = 1; //initial acceptance for student
				if (cur->item.exam[x].gpa < 3.5) {
					g = 0; //deny filter
				}
				else { //student gpa >= 3.5
					for (int y = 0; y < cur->item.exam[x].numOfSubjects; y++) {
						if (!strcmp(cur->item.exam[x].sub[y].getGrade(), "F")) { //check if F
							g = 0;
						}
					}
				}
				if (cur->item.exam[x].gpa > 2) {
					p = 0;
				}
				if (g) { //when not denied after filter, increase good student count
					gc++;
				}
				if (p) {
					pc++;
				}
			}
			if (gc >= 3) { //insert to list when good count at least 3
				goodList->insert(cur->item);
			}
			if (pc >= 3) {
				poorList->insert(cur->item);
			}
		}
		cur = cur->next;
	}
	return 1;
}

int menu() {

	cout << R"(
Menu

[1] Read File
[2] Delete Record
[3] Insert Past Exam Result
[4] Display Output 
[5] Get Average CGPA By Course
[6] Filter Student
[7] Find Eligible FYP Student
[8] Identify Good and Poor Result Student
[9] Exit Program

Enter choice: )";
	int x;
	cin >> x;
	return x;
}