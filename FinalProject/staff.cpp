#include "staff.h"
#include "Header.h"
#include "student.h"
#include "console.h"
#include "student.h"
#include "course.h"
#include "Semester.h"
#include "DKMon.h"
#include "SchoolYear.h"

ListSV* Create_Node_Sv(SinhVien sv)
{
	ListSV* a = new ListSV;
	a->info = sv;
	a->pNext = NULL;
	return a;
}
NodeGV* CreateNodeGV(GiaoVien gv)
{
	NodeGV* a = new NodeGV;
	a->info = gv;
	a->pNext = NULL;
	return a;
}
void AddTailGV(ListGV& dsgv, GiaoVien gv)
{
	NodeGV* p = CreateNodeGV(gv);
	if (dsgv.pHead == NULL)
		dsgv.pHead = p;
	else
	{
		NodeGV* k = dsgv.pHead;
		while (k->pNext != NULL)
			k = k->pNext;
		k->pNext = p;
	}
}
void CreateNewClass(ListLop& ds,const SchoolYear&Y)
{
	ofstream f;
	f.open(Y.DSLop, ios::app);
	Lop c;
	cout << "Nhap ma lop: "; getline(cin, c.Ma);
	f << c.Ma << endl;
	cout << "Nhap ten lop: "; getline(cin, c.Ten);
	f << c.Ten << endl;
	cout << "Nhap nien khoa: "; getline(cin, c.NienKhoa);
	f << c.NienKhoa << endl;
	c.pHead = NULL;
	ds.l[ds.n] = c;
	++ds.n;
	f.close();
}

void AddStudent_Input(ListLop& dsl, const SchoolYear& Y)
{
	string c;
	string Pass = "123456";
	SinhVien sv;
	gotoxy(70, 8);
	cout << "Nhap ID lop: ";
	getline(cin, c);
	int STTLop,i;
	int KT = CheckClass(dsl, c, dsl.n);
	if (KT != 0)
	{
		gotoxy(70, 9);
		cout << "Ngay nhap hoc: (dd mm yyyy)";
		for (i = 0; i < 3; ++i) cin >> sv.begin[i];
		cin.ignore();
		gotoxy(70, 10);
		cout << "Nhap ID sinh vien: ";
		getline(cin, sv.ID);
		gotoxy(70, 11);
		cout << "Nhap ho: "; getline(cin, sv.FirstName);
		gotoxy(70, 12);
		cout << "Nhap ten: "; getline(cin, sv.LastName);
		gotoxy(70, 13);
		cout << "Nhap gioi tinh: "; getline(cin, sv.Gender);
		gotoxy(70, 14);
		cout << "Nhap ngay sinh (dd/mm/yyyy): "; getline(cin, sv.DateOfBirth);
		gotoxy(70, 15);
		cout << "Nhap Social ID: "; getline(cin, sv.SocialID);
		if (KT == -1)
			STTLop = 0;
		else STTLop = KT;
		sv.YearStudent = 1;
		sv.Semester = 1;
		sv.pass = Pass;
		AddTailStudent(dsl.l[STTLop].pHead, sv);
		fstream file;
		file.open(string(dsl.l[STTLop].Ma) + "_" + FILECSV+".csv", ios::in);
		string s[7];
		int STT = 0;
		getline(file, s[0]);
		while (!file.eof())
		{
			for (int i = 0; i < 6; ++i) getline(file, s[i], ',');
			getline(file, s[6]);
			if (s[0] != "") STT = stoi(s[0]);
		}
		file.close();
		file.open(string(dsl.l[STTLop].Ma) + "_" + FILECSV+".csv", ios::app);
		file << STT + 1 << "," << sv.ID << "," << sv.FirstName << "," << sv.LastName << "," << sv.Gender << ","
			<< sv.DateOfBirth << "," << sv.SocialID << endl;
		file.close();
	}
	else
	{
		cout << "Khong tim thay lop " << c << endl;
		cout << "Vui long nhap lai..." << endl;
	}
}
void ReadFileDSGV(ListGV& dsgv, const SchoolYear& Y)
{
	dsgv.pHead = NULL;
	ifstream file;
	file.open(FILEDSGV);
	if (file.fail())
	{
		cout << "Failed to open this file!" << endl;
		exit(0);
	}
	while (!file.eof())
	{
		GiaoVien gv;
		file.clear();
		getline(file, gv.ID);
		if (gv.ID.length() == 0) break;
		getline(file, gv.pass);
		getline(file, gv.FirstName);
		getline(file, gv.LastName);
		getline(file, gv.Gender);
		getline(file, gv.DateOfBirth);
		getline(file, gv.SocialID);
		AddTailGV(dsgv, gv);
	}
	file.close();
}
void writeFileTeacher(ListGV dsgv, const SchoolYear& Y)
{
	ofstream file;
	file.open(FILEDSGV);
	if (file.fail())
	{
		cout << "Failed to open this file!" << endl;
		exit(0);
	}
	NodeGV* p = dsgv.pHead;
	while (p != NULL)
	{
		file << p->info.ID << endl << p->info.pass << endl << p->info.FirstName << endl << p->info.LastName << endl;
		file << p->info.Gender << endl << p->info.DateOfBirth << endl << p->info.SocialID << endl;
		p = p->pNext;
	}
	file.close();
}

int Checkcourses(ListCourses dsl, char a[10])
{
	NodeCourse* p = dsl.head;
	int i = 0;
	for (p; p != NULL; p = p->next)
	{
		if (p->course.ID==a)
			if (i == 0) return -1; 
			else return i;
		i++;
	}
	return 0; 
}
void UpdateCSV(ListLop& ds, const SchoolYear& Y)
{
	ofstream file;
	file.open(Y.DSSinhVien, ios_base::app);
	int ViTriLop;
	vector<string> row;
	SinhVien sv;
	for (int i = 0;i < ds.n; ++i) 
	{
		ifstream f1;
		f1.open(string(ds.l[i].Ma) + "_" + FILECSV + ".csv", ios::in | ios::out);
		string line = "", word;
		getline(f1, line);
		ViTriLop = 0;
		ListSV* k;
		while (f1.is_open())
		{
			getline(f1, line);
			if (line.size() == 0) break;
			stringstream s(line);
			row = {};
			while (getline(s, word, ','))
			{
				row.push_back(word);
			}
			sv.Class= ds.l[i].Ma;
			sv.ID= row[1];
			sv.FirstName = row[2];
			sv.LastName = row[3];
			sv.Gender = row[4];
			sv.DateOfBirth = row[5];
			sv.SocialID = row[6];
			sv.pass = "123456";
			sv.Semester = 1;
			sv.YearStudent = 1;
			sv.begin[0] = 5; sv.begin[1] = 10; sv.begin[2] = 2020; // gan co dinh
			bool flat = true;
			for (k = ds.l[i].pHead; k != NULL; k = k->pNext)
				if (k->info.ID == sv.ID && k->info.FirstName == sv.FirstName && k->info.LastName == sv.LastName && k->info.SocialID == sv.SocialID && k->info.ID == sv.ID && k->info.Gender == sv.Gender)
					flat = false;
			if (flat == true) AddTailStudent(ds.l[i].pHead, sv);
		}
		f1.close();
	}
	file.close();
	WriteFileStudent(ds, Y);
}

void UpdateStudent(const SchoolYear& Y)
{
	Time temp = getTime();
	fstream file, file1;
	file.open(Y.DSSinhVien, ios::in);
	file1.open("list.txt", ios::out);
	SinhVien temp1;
	int i;
	while (!file.eof())
	{
		getline(file,temp1.Class);
		getline(file, temp1.ID);
		getline(file, temp1.pass);
		getline(file, temp1.FirstName);
		getline(file, temp1.LastName);
		getline(file, temp1.Gender);
		getline(file, temp1.DateOfBirth);
		getline(file, temp1.SocialID);
		for (i = 0; i < 3; ++i)
			file >> temp1.begin[i];
		file.get();
		file >> temp1.YearStudent;
		file.get();
		file >> temp1.Semester;
		file.get();
		if (temp1.Class=="") break;
		else
		{
			file1 << temp1.Class << endl;
			file1 << temp1.ID << endl;
			file1 << temp1.pass << endl;
			file1 << temp1.FirstName << endl;
			file1 << temp1.LastName << endl;
			file1 << temp1.Gender << endl;
			file1 << temp1.DateOfBirth << endl;
			file1 << temp1.SocialID << endl;
			file1 << temp1.begin[0] << " " << temp1.begin[1] << " " << temp1.begin[2] << endl;
			if (temp.year - temp1.begin[2] == 1)
			{
				if ((temp.month == temp1.begin[1] && temp.day >= temp1.begin[0]) || (temp.month > temp1.begin[1]))
					temp1.YearStudent = 2;
			}
			else if (temp.year - temp1.begin[2] > 1)
			{
				if (temp.month < temp1.begin[1])
					temp1.YearStudent = temp.year - temp1.begin[2] - 1;
				else if (temp.month >= temp1.begin[1])
					temp1.YearStudent = temp.year - temp1.begin[2];
			}
			else temp1.YearStudent = 1;
			if (temp1.YearStudent > 6)
			{
				file1 << 7 << endl;
			}
			else
				file1 << temp1.YearStudent << endl;
			file1 << temp1.Semester << endl;

		}
	}
	file.close();
	file1.close();
	remove(Y.DSSinhVien.c_str());
	rename("list.txt", Y.DSSinhVien.c_str());
}
ListSV* findStudentOfCourses(const ListCourses& l, string mamon, int se, const SchoolYear& Y)
{
	ListSV* lsv = NULL;
	fstream file;
	file.open(to_string(se) + Y.StudentOfSubject, ios::in);
	string s;
	SinhVien sv;
	int i;
	while (getline(file, s))
	{
		if (s==mamon)
		{
			getline(file, s);
			while (true)
			{
				sv.Class=s;
				getline(file, sv.ID);
				getline(file, sv.pass);	
				getline(file, sv.FirstName);	
				getline(file, sv.LastName);
				getline(file, sv.Gender);	
				getline(file, sv.DateOfBirth);	
				getline(file, sv.SocialID);	
				for (i = 0; i < 3; ++i) file >> sv.begin[i];
				file >> sv.YearStudent;
				file >> sv.Semester;
				AddTailStudent(lsv, sv);
				file.ignore();
				getline(file, s);
				if (CheckCourses(l, s) || s.size() == 0) break;
			}
			break;
		}
	}
	file.close();
	return lsv;
}
int ViewListOfClass(ListLop& ds)
{
	char key;
	int vitri = 0;
	int numberofPage = CountNodePage(ds.n);
	int page = 1, STT, count, i;
	while (true)
	{
		STT = 15 * (page - 1);
		count = 0;
		system("cls");
		gotoxy(20, 4); cout << "+---------------------------------------------------------------------------------+" << endl;
		gotoxy(20, 5); cout << char(124) << "  " << setw(5) << left << " STT " << char(124) << "  " << setw(15) << left << "   Ma lop   " << char(124) << "  " << setw(40) << left << "                  Ten lop " << char(124) << "  " << setw(10) << left << "Nam hoc" << char(124) << endl;
		gotoxy(20, 6); cout << "+---------------------------------------------------------------------------------+" << endl;
		for (i = 0; i < 15; ++i)
		{
			if (STT + i > ds.n) break;
			if (sizeof(ds.l[i + STT].Ten) == 0) break;
			count = i + 1;
			gotoxy(20, 7 + i);
			cout << char(124) << "  " << setw(5) << left << STT + i + 1 << char(124) << "  " << setw(15) << left << ds.l[i + STT].Ma << char(124) << "  " << setw(40) << left << ds.l[i + STT].Ten << char(124) << "  " << setw(10) << left << ds.l[i + STT].NienKhoa << char(124);
		}
		gotoxy(20, 7 + count); cout << "+---------------------------------------------------------------------------------+";
		gotoxy(15, 7 + count + 1);
		gotoxy(50, 28); cout << page << "/" << numberofPage;
		key = GetKey();
		switch (key)
		{
		case LEFT:// sang trai
			vitri = 0;
			break;
		case RIGHT: // sang phai
			vitri = 1;
			break;
		case ESC: // thoat vong lap
			return 0;
			break;
		case ENTER:
			return 1;
			break;
		}
		if (vitri == 0)
		{
			if (page == 1) continue;
			else
				--page;
		}
		else if (vitri == 1)
		{
			if (page == numberofPage) continue;
			else
				++page;
		}
	}
}
int CountNodeStudent(ListSV* phead)
{
	ListSV* p = phead;
	int i = 0;
	while (p != NULL)
	{
		++i;
		p = p->pNext;
	}
	return i;
}
int CountNodePage(int i)
{
	int page = i / 15 + 1;
	if (i % 15 == 0)
		return page - 1;
	return page;
}
void ViewListOfStudentInClass(ListLop& ds)
{
	gotoxy(20, 30);
	cout << "Nhap ma lop: ";
	int ViTriLop;
	string Malop;
	int STT = 1;
	getline(cin, Malop);
	int KT = CheckClass(ds, Malop, ds.n);
	if (KT == 0) {
		cout << "Khong ton tai ma lop " << Malop << endl;
		return;
	}
	else if (KT == -1) ViTriLop = 0;
	else ViTriLop = KT;
	if (ds.l[ViTriLop].pHead == NULL) {
		cout << "Chua co sinh vien nao trong lop" << endl;
		return;
	}
	//
	char key;
	char arrowleft = 27;
	char arrowright = 26;
	int vitri = 0;
	int numberofstudent = CountNodeStudent(ds.l[ViTriLop].pHead);
	int numberofPage = CountNodePage(numberofstudent);
	int page = 1,i,j,count;
	ListSV* k;
	while (true)
	{
		k = ds.l[ViTriLop].pHead;
		STT = 15 * (page - 1);
		for (i = 1; i < page; ++i)
		{
			for (j = 0; j < 15; ++j)
			{
				k = k->pNext;
			}
		}
		count = 0;
		system("cls");
		gotoxy(35, 2); cout << "---------------------------- " << ds.l[ViTriLop].Ma << " ----------------------------"; // Loi in ra man hinh mat chu
		gotoxy(20, 4); cout << "+--------------------------------------------------------------------------------------------------------------+" << endl;
		gotoxy(20, 5); cout << char(124) << "  " << setw(5) << left << "STT" << char(124) << "  " << setw(15) << left << "   MSSV   " << char(124) << "  " << setw(20) << left << " Ho " << char(124) << "  " << setw(20) << " Ten" << char(124) << "  " << setw(10) << left << "Gioi tinh" << char(124) << "  " << setw(10) << "Ngay sinh" << char(124) << "  " << setw(10) << left << "CMND/CCCD" << endl;
		gotoxy(20, 6); cout << "+--------------------------------------------------------------------------------------------------------------+" << endl;
		for (i = 0; i < 15; ++i)
		{
			if (k == NULL) break;
			count = i + 1;
			gotoxy(20, 7 + i);
			cout << char(124) << "  " << setw(5) << left << i + STT + 1 << char(124) << "  " << setw(15) << left << k->info.ID << char(124) << "  " << setw(20) << left << k->info.FirstName << char(124) << "  " << setw(20) << k->info.LastName << char(124) << "  " << setw(10) << left << k->info.Gender << char(124) << "  " << setw(10) << k->info.DateOfBirth << char(124) << "  " << setw(10) << left << k->info.SocialID << char(124);
			k = k->pNext;
		}
		gotoxy(20, 7 + count); cout << "+--------------------------------------------------------------------------------------------------------------+" << endl;
		gotoxy(15, 7 + count + 1);
		gotoxy(50, 28); cout << page << "/" << numberofPage;
		key = GetKey();
		switch (key)
		{
		case LEFT:// sang trai
				vitri = 0;
				break;
		case RIGHT: // sang phai
				vitri = 1;
				break;
		case ESC: // thoat vong lap
				return;
				break;
		}
		if (vitri == 0 /*&& key == ENTER*/)
		{
			if (page == 1) continue;
			else
				--page;
		}
		if (vitri == 1/* && key == ENTER*/)
		{
			if (page == numberofPage) continue;
			else
				++page;
		}
	}
}

void ViewListOfStudentIncourses(ListCourses ds, int se,  const SchoolYear& Y)
{
	ViewListOfCourse(ds, se, Y);
	cout << "Nhap ma mon: ";
	int n = countNodeCourses(ds);
	int ViTrimon;
	char Mamon[50];
	int STT = 1;
	cin.ignore();
	cin.get(Mamon, 50, '\n');
	int KT = Checkcourses(ds, Mamon);
	if (KT == 0) {
		cout << "Khong ton tai ma mon " << Mamon << endl;
		return;
	}
	else if (KT == -1) ViTrimon = 0;
	else ViTrimon = KT;
	NodeCourse* p = ds.head;
	int i;
	for (i = 0; i < ViTrimon; ++i)
	{
		p = p->next;
	}
	ListSV* Lsv_Of_Courses = findStudentOfCourses(ds, Mamon, se, Y);
	ListSV* k = Lsv_Of_Courses;
	if (Lsv_Of_Courses == NULL) {
		cout << "Chua co sinh vien nao trong mon" << endl;
		return;
	}
	//
	char key;
	int vitri = 0;
	int numberofstudent = CountNodeStudent(Lsv_Of_Courses);
	int numberofPage = CountNodePage(numberofstudent);
	int page = 1,count,j;
	while (true)
	{
		k = Lsv_Of_Courses;
		STT = 15 * (page - 1);
		for (i = 1; i < page; ++i)
		{
			for (j = 0; j < 15; ++j)
			{
				k = k->pNext;
			}
		}
		count = 0;
		system("cls");
		gotoxy(10, 3); cout << "---------------------------- " << p->course.Name << " ----------------------------";
		gotoxy(5, 5); cout << "+--------------------------------=-----------------------------------------------------------------------------+" << endl;
		gotoxy(5, 6); cout << char(124) << "  " << setw(5) << left << "STT" << char(124) << "  " << setw(15) << left << "   MSSV   " << char(124) << "  " << setw(20) << left << " Ho " << char(124) << "  " << setw(20) << " Ten" << char(124) << "  " << setw(10) << left << "Gioi tinh" << char(124) << "  " << setw(10) << "Ngay sinh" << char(124) << "  " << setw(10) << left << "CMND/CCCD" << endl;
		gotoxy(5, 7); cout << "+--------------------------------------------------------------------------------------------------------------+" << endl;
		for (i = 0; i < 15; ++i)
		{
			if (k == NULL) break;
			count = i + 1;
			gotoxy(5, 7 + i);
			cout << char(124) << "  " << setw(5) << left << STT + i + 1 << char(124) << "  " << setw(15) << left << k->info.ID << char(124) << "  " << setw(20) << left << k->info.FirstName << char(124) << "  " << setw(20) << k->info.LastName << char(124) << "  " << setw(10) << left << k->info.Gender << char(124) << "  " << setw(10) << k->info.DateOfBirth << char(124) << "  " << setw(10) << left << k->info.SocialID << char(124);
			k = k->pNext;
		}
		gotoxy(5, 7 + count); cout << "+--------------------------------------------------------------------------------------------------------------+" << endl;
		gotoxy(0, 7 + count + 1);
		gotoxy(50, 28); cout << page << "/" << numberofPage;
		key = GetKey();
		switch (key)
		{
		case LEFT:// sang trai
				vitri = 0;
				break;
		case RIGHT: // sang phai
				vitri = 1;
				break;
		case ESC: // thoat vong lap
				return;
				break;
		}
		if (vitri == 0)
		{
			if (page == 1) continue;
			else
				--page;
		}
		if (vitri == 1)
		{
			if (page == numberofPage) continue;
			else
				++page;
		}
	}
}

void ExportListStudentInCourseToEnterScore(ListCourses dsm, int se, const SchoolYear& Y)
{
	ListSV* Lsv_Of_Courses, * k;
	int count;
	DiemMonHoc a;
	int t = YearPresent();
	string mamon, link;
	ifstream fileCheck;
	ofstream file;
	for (NodeCourse* p = dsm.head; p != NULL; p = p->next) 
	{
		Lsv_Of_Courses = findStudentOfCourses(dsm, p->course.ID,se, Y);
		k = Lsv_Of_Courses;
		if (Lsv_Of_Courses != NULL) 
		{
			count = 0;
			a.Final = a.MidTerm = a.Other = a.Total = 0.0;
			mamon = p->course.ID;
			link = "ScoreBoard" + to_string(t) + "_" + to_string(t + 1) + "_" + to_string(se) + "_" + mamon + ".csv";
            fileCheck.open(link);
			if (!fileCheck.is_open()) 
			{
				file.open(link);
				file << "STT,ID,First Name,Last Name,Total Mark, Final Mark, Midterm Mark, Other Mark" << endl;
					while (k != NULL)
					{
						count++;
						file << count << "," << k->info.ID << "," << k->info.FirstName << ","
							<< k->info.LastName << "," << a.Total << "," << a.Final << "," << a.MidTerm << "," << a.Other << endl;
						k = k->pNext;
					}
				file.close();
			}
			fileCheck.close();
		}
	}
}

void ViewScoreBoardOfACourse(ListCourses dsmon, int se,  const SchoolYear& Y)
{
	int t = YearPresent();
	string s, word, mamon;
	cout << "Nhap ma mon: ";
	getline(cin, mamon);
	string link = "ScoreBoard" + to_string(t) + "_" + to_string(t + 1) + "_" + to_string(se) + "_" + mamon + ".csv";
	ifstream fin;
	fin.open(link);
	if (!fin.is_open())
	{
		cout << "Khong ton tai ma mon " << mamon << endl;
		return;
	}
	getline(fin, s);
	int numberofstudent = 0;
	while (!fin.eof())
	{
		getline(fin, s);
		if (s.size() == 0) break;
		numberofstudent++;
	}
	//
	NodeCourse* p = dsmon.head;
	for (p; p != NULL; p = p->next)
	{
		if (p->course.ID == mamon)
			break;
	}
	fin.close();
	//

	char key;
	int vitri = 0;
	int page = 1,STT,i,j,count;
	int numberofPage = CountNodePage(numberofstudent);
	vector<string> row;
	while (true)
	{
		ifstream fin;
		fin.open(link);
		getline(fin, s);
		STT = 15 * (page - 1);
		for (i = 1; i < page; ++i)
		{
			for (j = 0; j < 15; ++j)
			{
				getline(fin, s);
			}
		}
		count = 0;
		system("cls");
		gotoxy(10, 3); cout << "\t\t---------------------------- " << p->course.Name << " ----------------------------";
		gotoxy(5, 5); cout << "+-------------------------------------------------------------------------------------------------------------------+" << endl;
		gotoxy(5, 6); cout << char(124) << "  " << setw(5) << left << "STT" << char(124) << "  " << setw(15) << left << "     MSSV" << char(124) << "  " << setw(20) << left << "         Ho" << char(124) << "  " << setw(20) << "        Ten" << char(124) << setw(10) << "  Total" << char(124) << setw(10) << "  Final" << char(124) << setw(10) << "  Midterm" << char(124) << setw(10) << "  Other" << char(124) << endl;
		gotoxy(5, 7); cout << "+--------------------------------------------------------------------------------------------------------------+" << endl;
		for (i = 0; i < 15; ++i)
		{
			count = i + 1;
			getline(fin, s);
			if (s.size() == 0)
			{
				count--;
				break;
			}
			stringstream si(s);
			row = {};
			while (getline(si, word, ','))
			{
				row.push_back(word);
			}
			gotoxy(5, 7 + i);
			cout << char(124) << "  " << setw(5) << left << i + STT + 1 << char(124) << "  " << setw(15) << left << row[1] << char(124) << "  " << setw(20) << left << row[2] << char(124) << "  " << setw(20) << row[3] << char(124) << setw(10) << row[4] << char(124) << setw(10) << row[5] << char(124) << setw(10) << row[6] << char(124) << setw(10) << row[7] << char(124) << endl;
			if (fin.eof())
			{
				--count;
				break;
			}
		}
		gotoxy(5, 7 + count); cout << "+-------------------------------------------------------------------------------------------------------------------+" << endl;
		gotoxy(0, 7 + count + 1);
		gotoxy(50, 28); cout << page << "/" << numberofPage;
		fin.close();
		key = GetKey();
		switch (key)
		{
		case LEFT:// sang trai
				vitri = 0;
				break;
		case RIGHT: // sang phai
				vitri = 1;
				break;
		case ESC: // thoat vong lap
				return;
				break;
		}
		if (vitri == 0)
		{
			if (page == 1) continue;
			else
				--page;
		}
		if (vitri == 1)
		{
			if (page == numberofPage) continue;
			else
				++page;
		}
	}
}
void updateAStudentResult(ListCourses ds, int se, const SchoolYear& Y)
{
	cin.ignore();
	ViewListOfCourse(ds, se, Y);
	cout << "Nhap ma mon: ";
	int n = countNodeCourses(ds);
	int ViTrimon;
	char Mamon[50];
	int STT = 1;
	cin.get(Mamon, 50, '\n');
	int KT = Checkcourses(ds, Mamon);
	if (KT == 0) {
		cout << "Khong ton tai ma mon " << Mamon << endl;
		return;
	}
	else if (KT == -1) ViTrimon = 0;
	else ViTrimon = KT;
	NodeCourse* p = ds.head;
	for (int i = 0; i < ViTrimon; ++i)
	{
		p = p->next;
	}
	ListSV* Lsv_Of_Courses = findStudentOfCourses(ds, Mamon, se, Y);
	ListSV* k = Lsv_Of_Courses;
	if (Lsv_Of_Courses == NULL) {
		cout << "Chua co sinh vien nao trong mon" << endl;
		return;
	}
	char ID[10];
	cin.ignore();
	cout << "Nhap ID sinh vien: ";
	cin.get(ID, 10, '\n');
	bool check = false;
	for (ListSV* i = k; i != NULL; i = i->pNext)
	{
		if (i->info.ID== ID)
		{
			check = true;
			break;
		}
	}
	if (!check)
	{
		cout << "Khong ton tai sinh vien nay trong khoa hoc!" << endl;
		return;
	}
	ifstream file;
	ofstream file1;
	int count = 0;
	string mamon = Mamon;
	int t = YearPresent();
	string link = "ScoreBoard" + to_string(t - 1) + "_" + to_string(t) + "_" + to_string(se) + "_" + mamon + ".csv";
	file.open(link);
	if (!file.is_open())
	{
		cout << "Bang diem khoa hoc nay chua duoc nhap!" << endl;
		return;
	}
	file1.open("tam.csv");
	file1 << "STT,ID,First Name,Last Name,Total Mark, Final Mark, Midterm Mark, Other Mark" << endl;
	string s;
	getline(file, s);
	DiemMonHoc a;
	for (k; k != NULL; k = k->pNext)
	{
		file >> count;
		getline(file, k->info.ID, ',');				file.get();
		getline(file, k->info.FirstName, ',');		file.get();
		getline(file, k->info.LastName, ',');		file.get();
		file >> a.Total;
		file.ignore();
		file >> a.Final;
		file.ignore();
		file >> a.MidTerm;
		file.ignore();
		file >> a.Other;
		if (k->info.ID== ID)
		{
			cout << "Nhap diem cap nhat:" << endl;
			cout << "Total mark: ";
			cin >> a.Total;
			cout << "Final mark: ";
			cin >> a.Final;
			cout << "Midterm mark: ";
			cin >> a.MidTerm;
			cout << "Other mark: ";
			cin >> a.Other;
		}
		file1 << count << "," << k->info.ID << "," << k->info.FirstName << ","
			<< k->info.LastName << "," << a.Total << "," << a.Final << "," << a.MidTerm << "," << a.Other << endl;
	}
	file.close();
	file1.close();
	remove(link.c_str());
	rename("tam.csv", link.c_str());
}
void ViewScoreOfAClass(ListLop dsl, ListCourses dsm, int se, const SchoolYear& Y)
{
	int t = YearPresent();
	string malop;
	cout << "Nhap ma lop: ";
	getline(cin, malop);
	system("cls");
	ListCourses ds = ReadListCourses(se, Y);
	int VitriLop;
	int KT = CheckClass(dsl, malop, dsl.n);
	if (KT == 0)
	{
		cout << "Khong ton tai ma lop " << malop << endl;
		return;
	}
	if (KT == -1) VitriLop = 0;
	else
		VitriLop = KT;
	
	char key;
	int vitri = 0, n = 0;
	int numberofstudent = CountNodeStudent(dsl.l[VitriLop].pHead);
	int numberofPage = CountNodePage2(numberofstudent);
	int page = 1;
	while (true)
	{
		ListSV* k = dsl.l[VitriLop].pHead;
		int STT = 3 * (page - 1);
		for (int i = 1; i < page; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				k = k->pNext;
			}
		}
		int count = 0;
		int i = 0;
		system("cls");
		gotoxy(35, 3); cout << "\t\t---------------------------- " << malop << " ----------------------------";
		gotoxy(20, 4); cout << "+-------------------------------------------------------------------------------------------------------------+" << endl;
		gotoxy(20, 5); cout << char(124) << "  " << setw(4) << left << "STT" << char(124) << "  " << setw(14) << left << "     MSSV" << char(124) << "  " << setw(19) << left << "         Ho" << char(124) << "  " << setw(19) << "   Ten" << char(124) << setw(30) << "          Mon hoc" << char(124) << setw(10) << "   Diem" << char(124) << endl;
		gotoxy(20, 6); cout << "+-------------------------------------------------------------------------------------------------------------+" << endl;
		for (int j = 0; j < 3; j++)
		{
			if (k == NULL) break;

			double TotalMark = 0;
			int SoChi = 0;
			bool flat = false;
			for (NodeCourse* p = dsm.head; p->next != NULL; p = p->next)
			{
				ListSV* psv = findStudentOfCourses(dsm, p->course.ID, se, Y);
				for (psv; psv != NULL; psv = psv->pNext)
				{
					if (psv->info.ID == k->info.ID)
					{
						string link = "ScoreBoard" + to_string(t) + "_" + to_string(t + 1) + "_" + to_string(se) + "_" + string(p->course.ID) + ".csv";
						ifstream f(link);
						if (f.is_open()) {
							string s[8];
							getline(f, s[0]);
							while (!f.eof())
							{
								for (int i = 0; i < 7; i++) getline(f, s[i], ',');
								getline(f, s[7]);
								if (s[0] == "") break;
								if (s[1] == k->info.ID)
								{
									count++;
									SoChi += p->course.NumOfCredits;
									TotalMark += stoi(s[4]) * p->course.NumOfCredits;
									flat = true;
									gotoxy(20, 6 + count + 2 * j);
									cout << char(124) << " " << setw(5) << left << j + STT + 1 << char(124) << " " << setw(15) << left << s[1] << char(124) << " " << setw(20) << left << s[2] << char(124) << " " << setw(20) << s[3] << char(124) << setw(30) << p->course.Name << char(124) << setw(10) << s[4] << char(124) << endl; gotoxy(20, 7 + ++n + 1);
								}
							}
						}
						f.close();
					}
				}
			}
			i = count + 1;
			if (flat == true) {

				TextColor(225);
				gotoxy(20, 6 + i + 2 * j);
				i++;
				cout << char(124) << " " << setw(5) << left << "" << char(124) << " " << setw(15) << left << k->info.ID << char(124) << " " << setw(20) << left << k->info.FirstName << char(124) << " " << setw(20) << k->info.LastName << char(124) << setw(30) << "GPA ki" << char(124) << setw(10) << (TotalMark / SoChi) * 1.0 << char(124) << endl;
			}
			else {
				gotoxy(20, 6 + i + 2 * j);
				i++;
				cout << char(124) << " " << setw(5) << left << "" << char(124) << " " << setw(15) << left << k->info.ID << char(124) << " " << setw(20) << left << k->info.FirstName << char(124) << " " << setw(20) << k->info.LastName << char(124) << setw(30) << "GPA ki" << char(124) << setw(10) << 0.0 << char(124) << endl;
				TextColor(224);
			}
			TextColor(224);
			gotoxy(20, 6 + i + 2 * j); cout << "+-------------------------------------------------------------------------------------------------------------+" << endl;
			i++;
			k = k->pNext;
		}
		//gotoxy(5, 7 + count); cout << "+--------------------------------------------------------------------------------------------------------------+" << endl;
		gotoxy(50, 28); cout << page << "/" << numberofPage;
		key = GetKey();
		if (key == LEFT)// sang trai
		{
			vitri = 0;
		}
		if (key == RIGHT) // sang phai
		{
			vitri = 1;
		}
		if (key == ESC) // thoat vong lap
		{
			return;
		}
		if (vitri == 0)
		{
			if (page == 1) continue;
			else
				page--;
		}
		if (vitri == 1)
		{
			if (page == numberofPage) continue;
			else
				page++;
		}
	}
}
DiemMonHoc ReadfileCSVScore(const SinhVien& S, const SchoolYear& Y, int se, string mamon)
{
	int t = YearPresent();
	fstream f1;
	string link = "ScoreBoard" + to_string(t ) + "_" + to_string(t+1) + "_" + to_string(se) + "_" + mamon + ".csv";
	f1.open(link, ios::in);
	DiemMonHoc score;
	if (f1.is_open())
	{
		string line, word;
		vector<string> row;
		string ID;
		while (!f1.eof())
		{
			line = ""; word = "";
			getline(f1, line);
			if (line.size() == 0) break;
			stringstream s(line);
			row = {};
			while (getline(s, word, ','))
				row.push_back(word);
			ID = row[1];
			if (ID == S.ID)
			{
				score.Total = stod(row[4]);
				score.Final = stod(row[5]);
				score.MidTerm = stod(row[6]);
				score.Other = stod(row[7]);
				break;
			}
				
		}
	}
	f1.close();
	return score;
}
void PrintFileCSV(ListLop dsl, SchoolYear Y)
{
	string nameFile;
	ifstream fin;
	ofstream file;
	for (int i = 0;i < dsl.n; ++i)
	{
		nameFile = string(dsl.l[i].Ma) + "_" + FILECSV;
		fin.open(nameFile);
		if (!fin.is_open())
		{
			file.open(nameFile);
			file << "STT, ID, FirstName, LastName,Gender,DOB,SocialID";
			file.close();
		}
	}
}