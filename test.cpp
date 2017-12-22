#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
#include "sqlite3.h"

using namespace std;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	const char* data = "";
	char sqlStr[256];	

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;


	// memanggil sql
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for(i = 0; i<argc; i++) {
		printf("%s", argv[i] ? argv[i] : "NULL"); // %s << isi dari variabel
	}
	return 0;
}

static int callback2(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for(i = 0; i<argc; i++) {
		printf("     %s\n", argv[i] ? argv[i] : "NULL"); // %s << isi dari variabel + enter
	}
	return 0;
}

static int callback3(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for(i = 0; i<argc; i++) {
		printf("\t   %s\n", argv[i] ? argv[i] : "NULL"); // %s << isi dari variabel + enter
	}
	return 0;
}

static int callback4(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for(i = 0; i<argc; i++) {
		printf("\t\t\t\t\t    %s\n", argv[i] ? argv[i] : "NULL"); // %s << isi dari variabel + enter
	}
	return 0;
}

static int callback5(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for(i = 0; i<argc; i++) {
		printf("\t\t\t\t\t\t\t\t%s\n", argv[i] ? argv[i] : "NULL"); // %s << isi dari variabel + enter
	}
	return 0;
}

	// main program
void title(int j);
void utama();
void daftar(int j);
void input(int j);
void hapus(int j);
void cari(int j);
void info(string b);
void update(int j, string i);
void exit();
void gotoxy(int x, int y);
void gotoxy(int x, int y, string text);
void draw(int j);

int main(int argc, char* argv[]) {
	char sqlStr[256];
	system ("CLS");
   rc = sqlite3_open("lib.db", &db);
   
   gotoxy(29,12);
   if( rc ) {
      fprintf(stderr, "Gagal membuka database: %s\n", sqlite3_errmsg(db));
   } else {
	  fprintf(stderr, "Sukses membuka database");
	  gotoxy(21,13,"Tekan tombol apapun untuk melanjutkan");
      getch();
      utama();
   }
}
   
void utama(){
	
	balek:
	sprintf( sqlStr, "vacuum");
	rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
	int j=0,x=0;
	system("cls");
	title(j);
	draw(j);
	gotoxy(4,7, "Selamat Datang di Perpustakaan Entah Dari Mana");
	gotoxy(4,8, "Silahkan pilih menu yang anda inginkan: ");
	gotoxy(4,10,"1.Melihat Daftar Buku");
	gotoxy(4,11,"2.Input Buku");
	gotoxy(4,12,"3.Hapus Buku");
	gotoxy(4,13,"4.Mencari Buku");
	gotoxy(4,19,"9.Keluar");
	gotoxy(44,8);
	gotoxy(4,22,"> ");
	cin >> j;
	
	if(j!=9){
		if(j>0&&j<5){
			switch (j){
				case 1:{
					daftar(j);
					break;
				}
				case 2:{
					input(j);
					break;
					}
				case 3:{
					hapus(j);
					break;
				}
				case 4:{
					cari(j);
					break;
				}
			}
		}
		goto balek;
	}
	
	gotoxy(4,19, "Apa anda yakin ingin keluar? (Y/N)");
	gotoxy(4,22, "                                  ");
	gotoxy(4,22, "> ");
	string erabe;
	cin >> erabe;
	if (erabe!="y"&&erabe!="Y"){
		goto balek;
	}
	
	exit();
}

void title(int j){ // Setting Judul
	
	int z=0;
	string tit[5]= {"LIBRARY IN THE MIDDLE OF NOWHERE","DAFTAR BUKU","INPUT BUKU","HAPUS BUKU","PENCARIAN BUKU"};
	if (j==1||j==4)
		z=1;
	gotoxy(40-tit[j].length()/2,3-z,tit[j]);

}

void daftar(int j){ // Setting Daftar
	
	int p=1;
	begin:
	int y=0,i=1+(11*(p-1));
	string c;
	
	system("cls");
	draw(j);
	title(j);
	
	for (i; i<(11*p)+1; i++){
		char id[2];
		int x=0;
		if (i>9){
			x=1;
		} else if (i>999){
			x=2;
		}
		sprintf(id, "%d", i); // convert
		gotoxy(6-x,7+y);	//ID
		cout << id;
		gotoxy(11,7+y);	//Title
		sprintf( sqlStr, "SELECT Title FROM book WHERE rowid = %s;", id);
		rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
		gotoxy(44,7+y);	//Author
		sprintf( sqlStr, "SELECT Author FROM book WHERE rowid = %s;", id);
		rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
		gotoxy(66,7+y);	//Ketersediaan
		sqlite3_stmt *stmt;
        sprintf( sqlStr, "SELECT Jumlah FROM book WHERE rowid = %s;", id);
		if (sqlite3_prepare_v2(db, sqlStr, -1, &stmt, NULL) != SQLITE_OK)
   			cout<<"Error";
		else {
   			for (;;) {
   				int rc = sqlite3_step(stmt);
   				if (rc == SQLITE_DONE)
   			    	break;
   				if (rc != SQLITE_ROW) {
       				cout<<"Error";
					break;
				}
				int stok = (sqlite3_column_int(stmt, 0));
   				if (stok > 0){
   					cout << "Tersedia";
				} else {
			   		cout << " Kosong";
				}
   			}
   			y++;
   		}
	}
	
	gotoxy(5,19,"1.Mencari Buku >< 2.Info Buku >< n/b = next/previous page >< 9.Kembali");
	gotoxy(4,22,"> ");
	cin >> c;
	if (c!="n"&&c!="b"&&c!="1"&&c!="2"&&c!="9"){
		goto begin;
	} else {
		if (c=="1"){
			cari(4);
			goto begin;
		} else if (c=="2"){
			string b;
			gotoxy(4,19,"Masukan ID Buku yang ingin anda lihat...                                ");
			gotoxy(4,22,">                                                                       ");
			gotoxy(6,22);
			cin >> b;
			info(b);
			goto begin;
		} else if (c=="n"){
			p++;
			goto begin;
		} else if (c=="b"){
			if (p!=1)
			p--;
			goto begin;
		}
	}
}

void input(int j){
	
	system("cls");
	draw(j);
	title(j);
	
	gotoxy(4,7, "Input data buku:");
	gotoxy(4,9, "Judul       :");
	gotoxy(4,10,"Pengarang   :");
	gotoxy(4,11,"Penerbit    :");
	gotoxy(4,12,"Tahun Terbit:");
	gotoxy(4,13,"Jumlah Buku :");
	
	string t,a,p,y,m;
	gotoxy(18,9);
	cin.ignore(256,'\n');
	getline(cin, t);
	const char * judul = t.c_str();
	gotoxy(18,10);
	getline(cin, a);
	const char * pengarang = a.c_str();
	gotoxy(18,11); // posisi input
	getline(cin, p);
	const char * penerbit = p.c_str();
	gotoxy(18,12);
	cin >> y;
	const char * tahun = y.c_str();
	gotoxy(18,13);
	cin >> m;
	const char * masuk = m.c_str();
	
//	gotoxy(40,18);
//	cout << judul <<endl<< pengarang << endl<< penerbit << endl<< tahun << endl<< masuk;
//	getch();

	sprintf( sqlStr, "INSERT INTO book (Title,Author,Publisher,Year,Jumlah) VALUES ('%s','%s','%s',%s,%s); ", judul, pengarang, penerbit,tahun,masuk);
	rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
	
	system("cls");
	draw(j);
	
	if( rc != SQLITE_OK ){
		gotoxy(26,12);
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		getch();
	} else {
		gotoxy(26,12);
		fprintf(stdout, "Records created successfully");
		getch();
	}
}

void hapus(int j){
	
	system("cls");
	draw(j);
	title(j);
	string in,erabe;
	
	gotoxy(4,7, "Menghapus buku dari database...");
	gotoxy(4,9, "Masukan ID buku: ");
	cin >> in;
	const char * id = in.c_str();
	gotoxy(4,10, "Apa anda yakin akan menghapus buku yang berjudul ");
	gotoxy(4,11,"'");
	sprintf( sqlStr, "SELECT Title FROM book WHERE rowid='%s' ", id);
	rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
	cout<<"'? (Y/N): ";
	cin.ignore(256,'\n');
	getline(cin, erabe);
	if (erabe=="y"||erabe=="Y"){
		sprintf( sqlStr, "DELETE FROM book WHERE rowid='%s' ", id);
		rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
		gotoxy(4,13, "Buku telah dihapus...");
		gotoxy(4,17,"Pengingat:");
		gotoxy(4,18);
		cout<<"Buku dengan ID lebih dari " << in << " akan digeser untuk menggantikan ID yang";
		gotoxy(4,19,"telah terhapus");
	} else {
		gotoxy(4,13,"Buku ");
		sprintf( sqlStr, "SELECT Title FROM book WHERE rowid='%s' ", id);
		rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
		cout<< " tidak jadi dihapus...";
	}
	getch();
}

void cari(int j){ // Setting Pencarian
	
	system("cls");
	draw(j);
	title(j);
	
	gotoxy(4,19, "Masukan judul yang ingin anda cari...");
	gotoxy(4,22,"> ");
	string what;
	cin.ignore(256,'\n');
	getline(cin, what);
	const char * search = what.c_str();
	gotoxy(0,7);
	sprintf( sqlStr, "SELECT Publisher FROM book WHERE Title LIKE '%%%s%%';", search);
	rc = sqlite3_exec(db, sqlStr, callback5, 0, &zErrMsg);
	gotoxy(0,7);
	sprintf( sqlStr, "SELECT Author FROM book WHERE Title LIKE '%%%s%%';", search);
	rc = sqlite3_exec(db, sqlStr, callback4, 0, &zErrMsg);
	gotoxy(0,7);
	sprintf( sqlStr, "SELECT Title FROM book WHERE Title LIKE '%%%s%%';", search);
	rc = sqlite3_exec(db, sqlStr, callback3, 0, &zErrMsg);
	gotoxy(0,7);
	sprintf( sqlStr, "SELECT rowid FROM book WHERE Title LIKE '%%%s%%';", search);
	rc = sqlite3_exec(db, sqlStr, callback2, 0, &zErrMsg);
	draw(j);
	gotoxy(4,19, "Tekan tombol sembarang untuk kembali...");
	gotoxy(4,22, "                                                                       ");
	gotoxy(4,22, "> ");
	getch();
}

void info(string b){
	
	begin:
	system("cls");
	int j=0;
	string what;
	string temp = b;
	draw(j);
	title(j);
	const char * id = b.c_str();
	gotoxy(4,7);
	cout<< "Info buku ";
	sprintf( sqlStr, "SELECT Title FROM book WHERE rowid=%s;", id);
	rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
	gotoxy(4,9, "ID          : ");
	cout<<id;
	gotoxy(4,10,"Judul       : ");
	sprintf( sqlStr, "SELECT Title FROM book WHERE rowid=%s;", id);
	rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
	gotoxy(4,11,"Pengarang   : ");
	sprintf( sqlStr, "SELECT Author FROM book WHERE rowid=%s;", id);
	rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
	gotoxy(4,12,"Penerbit    : ");
	sprintf( sqlStr, "SELECT Publisher FROM book WHERE rowid=%s;", id);
	rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
	gotoxy(4,13,"Tahun Terbit: ");
	sprintf( sqlStr, "SELECT Year FROM book WHERE rowid=%s;", id);
	rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
	gotoxy(4,14,"Jumlah Buku : ");
	sprintf( sqlStr, "SELECT Jumlah FROM book WHERE rowid=%s;", id);
	rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
	
	gotoxy(4,19, "1.Edit info buku >< 9.Kembali");
	gotoxy(4,22, "> ");
	cin >> what;
	if (what!="1"&&what!="9"){
		goto begin;
	} else if (what=="1"){
		gotoxy(4,17, "Apa yang ingin anda rubah?");	
		gotoxy(4,18, "1.Judul >< 2.Pengarang >< 3.Penerbit >< 4.Tahun Terbit >< 5.Jumlah Buku");
		gotoxy(4,19, "6.Semua >< 9. Kembali                                                 ");
		gotoxy(4,22, "                                                                       ");
		gotoxy(4,22, "> ");
		int pil;
		cin >> pil;
		if(pil!=9){
			if (pil>0&&pil<7){
				update(pil, temp);
			}
			goto begin;
		}
	}
}

void update(int b, string i){
	
	const char * id = i.c_str();
	gotoxy(4,17, "                          ");	
	gotoxy(4,18, "                                                                       ");
	gotoxy(4,19, "                                                                       ");
	gotoxy(4,22, "                                                                       ");
	
		switch (b){
			case 1:{ // Judul
				gotoxy(4,19, "Masukan judul buku yang baru:");
				gotoxy(4,22, "> ");
				string what;
				cin.ignore(256,'\n');
				getline(cin, what);
				const char * anyar = what.c_str();
				sprintf( sqlStr, "UPDATE book SET Title = '%s' WHERE rowid=%s;", anyar, id);
				rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
				break;
			}
			case 2:{ //
				gotoxy(4,19, "Masukan nama pengarang yang baru:");
				gotoxy(4,22, "> ");
				string what;
				cin.ignore(256,'\n');
				getline(cin, what);
				const char * anyar = what.c_str();
				sprintf( sqlStr, "UPDATE book SET Author = '%s' WHERE rowid=%s;", anyar, id);
				rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
				break;
			}
			case 3:{
				gotoxy(4,19, "Masukan nama penerbit yang baru:");
				gotoxy(4,22, "> ");
				string what;
				cin.ignore(256,'\n');
				getline(cin, what);
				const char * anyar = what.c_str();
				sprintf( sqlStr, "UPDATE book SET Publisher = '%s' WHERE rowid=%s;", anyar, id);
				rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
				break;
			}
			case 4:{
				gotoxy(4,19, "Masukan tahun penerbitan buku yang baru:");
				gotoxy(4,22, "> ");
				string what;
				cin.ignore(256,'\n');
				getline(cin, what);
				const char * anyar = what.c_str();
				sprintf( sqlStr, "UPDATE book SET Year = '%s' WHERE rowid=%s;", anyar, id);
				rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
				break;
			}
			case 5:{
				gotoxy(4,19, "Masukan jumlah stok buku yang baru");
				gotoxy(4,22, "> ");
				string what;
				cin.ignore(256,'\n');
				getline(cin, what);
				const char * anyar = what.c_str();
				sprintf( sqlStr, "UPDATE book SET Jumlah = '%s' WHERE rowid=%s;", anyar, id);
				rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
				break;
			}
			case 6:{
				gotoxy(4,7, "Merubah data buku:                                                 ");
				gotoxy(4,10, "Judul       :                                                      ");
				gotoxy(4,11,"Pengarang   :                                                      ");
				gotoxy(4,12,"Penerbit    :                                                      ");
				gotoxy(4,13,"Tahun Terbit:                                                      ");
				gotoxy(4,14,"Jumlah Buku :                                                      ");
				
				string t,a,p,y,m;
				gotoxy(18,10);
				cin.ignore(256,'\n');
				getline(cin, t);
				const char * judul = t.c_str();
				gotoxy(18,11);
				getline(cin, a);
				const char * pengarang = a.c_str();
				gotoxy(18,12); // posisi input
				getline(cin, p);
				const char * penerbit = p.c_str();
				gotoxy(18,13);
				cin >> y;
				const char * tahun = y.c_str();
				gotoxy(18,14);
				cin >> m;
				const char * masuk = m.c_str();
				
				sprintf( sqlStr, "UPDATE book SET (Title,Author,Publisher,Year,Jumlah) VALUES ('%s','%s','%s',%s,%s); ", judul, pengarang, penerbit,tahun,masuk);
				rc = sqlite3_exec(db, sqlStr, callback, 0, &zErrMsg);
			}
		}
		
	if( rc != SQLITE_OK ){
		gotoxy(4,19);
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		getch();
	} else {
		gotoxy(4,19, "                                                                      ");
		gotoxy(4,19);
		fprintf(stdout, "Data telah diubah!!");
		getch();
	}

}

void exit(){ // tampilan keluar program
	
	system("cls");
	string ty,bye;
	ty	= "Terima kasih";
	bye	= "Tekan tombol sembarang untuk keluar";
	gotoxy(40-ty.length()/2,12);
	cout << ty;
	gotoxy(40-bye.length()/2,13);
	cout << bye;
	gotoxy(0,0);
	getch();
}

void gotoxy(int x, int y){ // perpindahan kursor
	
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console,CursorPosition);
}

void gotoxy(int x, int y, string text){ // perpindahan kursor+teks
	
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console,CursorPosition);
	cout << text;
}

void draw(int j){ // bingkai
	
	int a;
	
//	for (a=0; a<16; a++){ // penggaris
//		int b=a*5;
//		gotoxy(0,b);
//		cout << b;
//		gotoxy(b,0);
//		cout << b;
//	}
	
	char tl,tr,bl,br,ver,hor,ct,cb,cl,cr,c; // border
	tl	= '\xC9'; // top-left
	tr	= '\xBB'; // top-right
	bl	= '\xC8'; // bottom-left
	br	= '\xBC'; // bottom-right
	ver	= '\xBA'; // vertical-line
	hor	= '\xCD'; // horizontal-line
	ct	= '\xCB'; // connector-top
	cb	= '\xCA'; // connector-bottom
	cl	= '\xCC'; // connector-left
	cr	= '\xB9'; // connector-right
	c	= '\xCE'; // cross
	
	string line(74,hor);
	
	gotoxy(2,1);
	cout << tl << line << tr;
	for (a=1;a<22;a++){
		gotoxy(2,a+1);
		cout << ver;
		gotoxy(77,a+1);
		cout << ver;
	}
	gotoxy(2,23);
	cout << bl << line << br;
	gotoxy(2,5);
	cout << cl << line << cr;
	gotoxy(2,21);
	cout << cl << line << cr;
	
	if (j==1||j==4){ //daftar buku
		gotoxy(2,3);	
		cout << cl << line << cr;
		// ID
		gotoxy(5,4,"ID");
		gotoxy(9,3);
		cout << ct;
		gotoxy(9,4);
		cout << ver;
		gotoxy(9,5);
		cout << cb;
		// Judul
		gotoxy(21,4,"Judul Buku");
		gotoxy(42,3);
		cout << ct;
		gotoxy(42,4);
		cout << ver;
		gotoxy(42,5);
		cout << cb;
		// Pengarang
		gotoxy(48,4,"Pengarang");
		gotoxy(62,3);
		cout << ct;
		gotoxy(62,4);
		cout << ver;
		gotoxy(62,5);
		cout << cb;
		// Ketersediaan
		if (j==1)
		gotoxy(64,4,"Ketersediaan");
		else 
		gotoxy(66,4,"Penerbit");
	}
}
