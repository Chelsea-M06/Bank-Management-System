//library import
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//jumlah max data akun
#define N 100

//tempat penampungan data yang diread dari database.txt
typedef struct {
	char id[5];
	char name[31];
	char num[13];
	char date[11];
	unsigned long balance;
}account;
account data[N];

//untuk menyimpan jumlah data yang berhasil diread dari file database.txt
int n;

//function declaration
void showMenu();
void inputData();
void inputfile(char[], char[], char[], char[], unsigned long);
void depo_wdraw(int);
void deleteData();
void scanData();
int search(char[]);


/*main() function created by Chelsea, Leon, and Vannes*/
/*main() function bertujuan untuk mengatur alur jalannya program sesuai pilihan aksi 
yang dipilih user*/

int main(){
	//variables
	int ind;
	
	//repetition agar program bisa terus berjalan hingga user memasukkan input 5
	while(1){
		//tampilin menunya
		showMenu();
		
		//user input
		scanf("%d", &ind);
		getchar();
		printf("\n");
		
		//menentukan function yang akan dijalankan sesuai user input
		switch(ind){
			case(1): inputData(); break;
			case(2): depo_wdraw(ind); break;
			case(3): depo_wdraw(ind); break;
			case(4): deleteData(); break;
			case(5): printf("<< Thank you! >>\n"); return 0; //sebagai stopping condition
			default: printf("<< Harap masukan angka yang terdapat dalam menu di atas! >>\n\n"); break;
		}
	}
}


/*showMenu() function created by Chelsea, Leon, and Vannes*/
/*showMenu() function bertujuan untuk menampilkan menu utama sebelum sesi pemilihan aksi 
pada main() function*/

void showMenu(){
	printf("------------ MENU UTAMA ------------\n");
	printf("====================================\n");
	printf("Harap masukkan nomor dari menu di bawah\n");
	printf("yang dipilih lalu tekan enter:\n");
	printf("1. Buat akun baru\n");
	printf("2. Penarikan tunai\n");
	printf("3. Deposit tunai\n");
	printf("4. Penutupan akun\n");
	printf("5. Exit\n");
	printf("----------------------------\n");
	printf("Input: ");
}


/*Function created by Chelsea*/
/*inputData() function bertujuan untuk menerima data dari akun baru yang ingin dibuat*/

void inputData(){
	//variables
	int i;
	unsigned long temp;
	char inp_id[5];
	char inp_name[31];
	char inp_num[13];
	char inp_date[11];
	unsigned long inp_balance;
	
	//user input
	printf("Harap masukkan data diri untuk pembuatan akun baru anda di bawah:\n");
	printf("-----------------------------------------------------------------\n");
	
	printf("ID                        : ");
	scanf("%[^\n]", inp_id);
	getchar();
	
    printf("Masukkan nama             : ");
    scanf("%[^\n]", inp_name);
    getchar();
    
    printf("Masukkan nomor telephone  : ");
    scanf("%[^\n]", inp_num);
    getchar();
    
    printf("Masukkan tanggal          : ");
    scanf("%[^\n]", inp_date);
    getchar();
    
    printf("Masukkan jumlah saldo     : Rp ");
    scanf("%d", &temp);
    getchar();
    
    // untuk pengecekan minimal deposit sebesar 100.000, bila tidak memenuhi, data tidak akan diproses dan pembuatan akun akan dibatalkan;
    if(temp<100000){ //biar ga dimasukin dulu ke datanya jadi pake temp dulu
    	printf("<< Maaf, pembuatan akun anda tidak dapat diproses. >>\n");
    	printf("<< Deposit anda tidak memenuhi ketentuan, minimal deposit adalah sebesar Rp 100.000,00. >>\n\n");
    	return;
    	
	}else if(temp>=100000){
		inp_balance = temp;//mindahin isi temp
    	inputfile(inp_id, inp_name, inp_num, inp_date, inp_balance);//buat masukin data ke database.txt
   		printf("\n<< Selamat, akun atas nama %s (ID akun: %s) telah dibuat. >>\n", inp_name, inp_id);
   		printf("<< Jumlah saldo awal anda: Rp %ld,00. >>\n\n", inp_balance);
   	}
   	
   	printf("\n");
}


/*inputfile() function created by Chelsea*/
/*inputfile() function bertujuan untuk menambahkan data dari function inputdata() ke dalam database.txt*/

void inputfile(char inp_id[5], char inp_name[31], char inp_num[13], char inp_date[11], unsigned long inp_balance){
	//Create buffer
	FILE *fp=fopen("database.txt","a");
	
	//openstream
	if(fp == NULL){
		printf("<< File cannot be found/opened! >>");
		exit(1);
	}
	
	//process data	
	fprintf(fp, "%s|%s|%s|%s|%ld\n", inp_id, inp_name, inp_num, inp_date, inp_balance);	
	fclose(fp);
}


/*depo_wdraw() function creared by Vannes and Leon*/
/*depo_wdraw() function bertujuan untuk memanipulasi data saldo dari sebuah akun
kemudian menyimpan kembali data yang telah diperbarui ke database.txt*/

void depo_wdraw(int mainfunc_selection){
	//variables
	char dwid[5];
	char confirm;
	int ind, i;
	unsigned long amount;
	
	//buat baca data dari database.txt sekalian nentuin jumlah datanya
	scanData();
	
	//user input
	if(mainfunc_selection == 2){
		printf("Masukkan ID akun untuk penarikan tunai:\n");
	} else if(mainfunc_selection == 3){
		printf("Masukkan ID akun untuk deposit tunai:\n");
	}
	scanf("%s", &dwid);
	getchar();
	printf("\n");
	
	//memastikan kalo akun tujuan deposit/withdraw memang ada
	ind = search(dwid);
	
	//kondisi berdasarkan hasil return search()
	if(ind == -1){
		//message
		printf("<< Akun tidak ditemukan. >>\n\n");
		
		//reset nilai n ke 0 untuk aksi yang bakal dipilih user selanjutnya dan exit fungsi
		n = 0; return;
		
	} else {
		//message
		printf("<< ID yang anda masukkan adalah ID akun atas nama %s. >>\n\n", data[ind].name);
		if(mainfunc_selection == 2){
			printf("Apakah anda yakin ingin melakukan penarikan tunai dari akun tersebut?\n");
		} else if(mainfunc_selection == 3){
			printf("Apakah anda yakin ingin melakukan deposit tunai ke akun tersebut?\n");
		}
		
		//confirmation
		printf("Silahkan masukan tombol apapun lalu enter untuk melanjutkan dan 'n' lalu enter untuk membatalkan:\n");
		scanf("%c", &confirm);
		if(confirm == 'n'){
			//message
			if(mainfunc_selection == 2){
				printf("<< Proses penarikan tunai dibatalkan. >>\n\n");
			} else if(mainfunc_selection == 3){
				printf("<< Proses deposit tunai dibatalkan. >>\n\n");
			}
			
			//reset nilai n ke 0 untuk aksi yang bakal dipilih user selanjutnya dan exit fungsi
			n = 0; return;
		}
		
		//user input
		printf("\nSaldo anda saat ini: Rp %ld,00.\n", data[ind].balance);
		if(mainfunc_selection == 2){
			printf("Masukkan jumlah penarikan tunai:\n");
		} else if(mainfunc_selection == 3){
			printf("Masukkan jumlah deposit tunai:\n");
		}
		printf("Rp "); scanf("%ld", &amount);
		getchar();
		printf("\n");
		
		//untuk deposit
		if(mainfunc_selection == 3){
			//deposit gabisa di bawah Rp 100k
			if(amount < 100000){
				//message
				printf("<< Jumlah deposit haruslah minimal Rp 100.000,00. >>\n\n");
			
				//reset nilai n ke 0 untuk aksi yang bakal dipilih user selanjutnya dan exit fungsi
				n = 0; return;
			}
		
			//manipulasi data
			data[ind].balance += amount;
			
		} else if(mainfunc_selection == 2){
			//withdraw gabisa lebih dari Rp 5jt
			if(amount > 5000000){
				//message
				printf("<< Jumlah penarikan maksimum adalah Rp 5.000.000,00. >>\n\n");
				
				//reset nilai n ke 0 untuk aksi yang bakal dipilih user selanjutnya dan exit fungsi
				n = 0; return;
			}
			
			//withdraw gabisa di bawah saldo awal
			if(amount > data[ind].balance){
				//message
				printf("<< Maaf, jumlah saldo tidak cukup untuk ditarik. >>\n\n");
			
				//reset nilai n ke 0 untuk aksi yang bakal dipilih user selanjutnya dan exit fungsi
				n = 0; return;
			}
			
			//manipulasi data
			data[ind].balance -= amount;
		}
		
		
		//manipulasi konten database.txt (tulis ulang/overwrite)
		FILE *fp;
		fp = fopen("database.txt", "w");
		
		for(i = 0; i < n; i++){
			fprintf(fp, "%s|%s|%s|%s|%ld\n", data[i].id, data[i].name, data[i].num, data[i].date, data[i].balance);
		}
		
		fclose(fp);
		
		//message
		if(mainfunc_selection == 2){
			printf("<< Selamat, Rp %ld,00 berhasil ditarik dari akun anda. >>\n", amount);
		} else if(mainfunc_selection == 3){
			printf("<< Selamat, Rp %ld,00 berhasil dideposit ke akun anda. >>\n", amount);
		}
		printf("<< Saldo anda sekarang: Rp %ld,00. >>\n\n", data[ind].balance);
		
		//reset nilai n ke 0 untuk aksi yang bakal dipilih user selanjutnya dan exit fungsi
		n = 0;
	}
	
	printf("\n");
}

/*deleteData() created by Vannes */
/*deleteData() bertujuan untuk menghapus/menutup sebuah akun bank*/

void deleteData(){
	//variables
	int ind, i;
	char confirm;
	char del_id[5];
	
	//buat baca data dari database.txt sekalian nentuin jumlah datanya
	scanData();
	
	//user input
	printf("Masukkan ID akun yang ingin ditutup:\n");
	scanf("%s", &del_id);
	getchar();
	printf("\n");
	
	//memastikan kalo akun yang mau didelete memang ada
	ind = search(del_id);
	
	//kondisi berdasarkan hasil return search()
	if(ind == -1){
		//message
		printf("<< Akun tidak ditemukan. >>\n\n");
		
		//reset nilai n ke 0 untuk aksi yang bakal dipilih user selanjutnya dan exit fungsi
		n = 0; return;
		
	} else {
		//message
		printf("<< ID yang anda masukkan adalah ID akun atas nama %s. >>\n\n", data[ind].name);
		printf("Apakah anda yakin ingin menutup akun tersebut?\n");
		
		//confirmation
		printf("Silahkan masukan tombol apapun lalu enter untuk melanjutkan dan 'n' lalu enter untuk membatalkan:\n");
		scanf("%c", &confirm);
		if(confirm == 'n'){
			printf("<< Proses penutupan akun dibatalkan. >>\n\n");
			
			//reset nilai n ke 0 untuk aksi yang bakal dipilih user selanjutnya dan exit fungsi
			n = 0; return;
		}
		
		//create buffer + open file
		FILE *fp;
		fp = fopen("database.txt", "w");
		
		//rewrite data tanpa memasukkan akun yang ingin didelete
		for(i = 0; i < n; i++){
			if(i == ind){
				continue;
			} else {
				fprintf(fp, "%s|%s|%s|%s|%ld\n", data[i].id, data[i].name, data[i].num, data[i].date, data[i].balance);
			}
		}
		
		//close buffer + file
		fclose(fp);
		
		//message
		printf("\n<< Akun atas nama %s (ID akun: %s) berhasil ditutup. >>\n\n", data[ind].name, data[ind].id);
		
		//reset nilai n ke 0 untuk aksi yang bakal dipilih user selanjutnya dan exit fungsi
		n = 0;
	}
	
	printf("\n");
}


/*scanData() function created by Vannes*/
/*scanData() function bertujuan untuk membaca data dari database.txt dan disimpan ke
dalam struct*/

void scanData(){
	//variables
	int i = 0;
	
	//create buffer + open file for read
	FILE *fp;
	fp = fopen("database.txt", "r");
	
	//read data from database.txt
	while(!feof(fp)){
		//kode utama untuk baca data
		fscanf(fp, "%[^|]|%[^|]|%[^|]|%[^|]|%ld\n", &data[i].id, &data[i].name, &data[i].num, &data[i].date, &data[i].balance);
		
		//biar tau ada berapa data di dalam file database.txt
		i++; n = i;
	}
	
	//close buffer + file
	fclose(fp);
}


/*search() function created by Vannes*/
/*search() function bertujuan untuk melakukan pencarian sebuah data akun pada database.txt 
untuk memastikan bahwa akun yang ingin dideposit/ditarik/ditutup memanglah ada*/

int search(char key[5]){
	/*pake algoritma linear search*/
	//variables
	int i;
	
	//searching
	for(i = 0; i < n; i++){
		if(strcmp(key, data[i].id) == 0){
			return i;
		}
	}
	
	//kalo datanya kagak ada sama sekali
	return -1;
}
