#include <iostream>
#include <fstream>
#include <cstring>
#include <conio.h>
#include <cstdlib>
using namespace std;

struct GuvenlikGorevlisi {
    char ad[80];
    char soyad[80];
    char kimlik_no[12];
    char gorev[100];
};

void GorevliEkle();
void GorevliListele();
void GorevliAra();
void GorevliSil();
void GorevliDuzenle();

GuvenlikGorevlisi gorevli;

int main() {
	system("title Yakishi");
    setlocale(LC_ALL, "Turkish");
    char anamenu;
    do {
        system("cls");
        cout << "|---- Güvenlik Görevlisi Takip Sistemi ----|" << endl;
        cout << "|                                          |" << endl;
        cout << "|             Seçim Yapýnýz                |" << endl;
        cout << "|                                          |" << endl;
        cout << "|          1- Görevli Ekleme               |" << endl;
        cout << "|          2- Görevli Listeleme            |" << endl;
        cout << "|          3- Görevli Arama                |" << endl;
        cout << "|          4- Görevli Silme                |" << endl;
        cout << "|          5- Görevli Düzenleme            |" << endl;
        cout << "|                                          |" << endl;
        cout << "|------------------------------------------|" << endl;
        char secim;
        cin >> secim;
        switch (secim) {
            case '1':
                GorevliEkle();
                break;
            case '2':
                GorevliListele();
                break;
            case '3':
                GorevliAra();
                break;
            case '4':
                GorevliSil();
                break;
            case '5':
                GorevliDuzenle();
                break;
        }
        cout << "Ana menüye dönmek istiyor musunuz? (y/n)" << endl;
        anamenu = getche();
    } while (anamenu == 'y');
    return 0;
}

void GorevliEkle() {
    ofstream yaz("gorevliler.dat", ios::binary | ios::app);
    char secim;
    int adet = 0;
    do {
        cout << "Güvenlik Görevlisinin Adýný Giriniz: ";
        cin >> gorevli.ad;
        cout << "Güvenlik Görevlisinin Soyadýný Giriniz: ";
        cin >> gorevli.soyad;
        cout << "Güvenlik Görevlisinin Kimlik Numarasýný Giriniz: ";
        cin >> gorevli.kimlik_no;
        gorevli.kimlik_no[11] = '\0'; // Bu kýsýmda bir hatayla karþýlaþtýðým ve çözemediðim için biraz yardým aldým :)
        cin.ignore(); 
        cout << "Güvenlik Görevlisinin Görevini Giriniz: ";
        cin.getline(gorevli.gorev, 100);
        yaz.write((char*)&gorevli, sizeof(gorevli));
        adet++;
        cout << "Baþka kayýt eklemek istiyor musunuz? (y/n)";
        secim = getche();
        cout << endl;
    } while (secim == 'y');
    cout << adet << " adet güvenlik görevlisi eklendi." << endl;
    yaz.close();
}

void GorevliListele() {
    ifstream oku("gorevliler.dat", ios::binary);
    oku.seekg(0, ios::end);
    int kayitsayisi = oku.tellg() / sizeof(gorevli);
    cout << "Toplam Güvenlik Görevlisi Kayýt Sayýsý: " << kayitsayisi << endl;
    if (kayitsayisi > 0) {
        oku.seekg(0, ios::beg);
        for (int i = 0; i < kayitsayisi; i++) {
            oku.read((char*)&gorevli, sizeof(gorevli));
            cout << "\n" << i + 1 << ". Görevlinin Bilgileri:" << endl;
            cout << "Ad: " << gorevli.ad << endl;
            cout << "Soyad: " << gorevli.soyad << endl;
            cout << "Kimlik No: " << gorevli.kimlik_no << endl;
            cout << "Görev: " << gorevli.gorev << endl;
        }
    } else {
        cout << "Kayýt bulunamadý..." << endl;
    }
    oku.close();
}

void GorevliAra() {
    ifstream oku("gorevliler.dat", ios::binary);
    oku.seekg(0, ios::end);
    int kayitsayisi = oku.tellg() / sizeof(gorevli);
    cout << "Aranan güvenlik görevlisinin kimlik numarasýný giriniz: ";
    char kimlik_no[12];
    cin >> kimlik_no;
    bool bulundu = false;
    if (kayitsayisi > 0) {
        oku.seekg(0, ios::beg);
        for (int i = 0; i < kayitsayisi; i++) {
            oku.read((char*)&gorevli, sizeof(gorevli));
            if (strcmp(gorevli.kimlik_no, kimlik_no) == 0) {
                cout << "Güvenlik Görevlisi Bulundu:" << endl;
                cout << "Ad: " << gorevli.ad << endl;
                cout << "Soyad: " << gorevli.soyad << endl;
                cout << "Kimlik No: " << gorevli.kimlik_no << endl;
                cout << "Görev: " << gorevli.gorev << endl;
                bulundu = true;
                break;
            }
        }
    }
    if (!bulundu) {
        cout << "Kayýt bulunamadý..." << endl;
    }
    oku.close();
}

void GorevliSil() {
    char kimlik_no[12];
    cout << "Silmek istediðiniz güvenlik görevlisinin kimlik numarasýný giriniz: ";
    cin >> kimlik_no;
    ifstream oku("gorevliler.dat", ios::binary);
    ofstream yaz("yedek.dat", ios::binary);
    bool bulundu = false;
    oku.seekg(0, ios::end);
    int kayitsayisi = oku.tellg() / sizeof(gorevli);
    if (kayitsayisi > 0) {
        oku.seekg(0, ios::beg);
        for (int i = 0; i < kayitsayisi; i++) {
            oku.read((char*)&gorevli, sizeof(gorevli));
            if (strcmp(gorevli.kimlik_no, kimlik_no) != 0) {
                yaz.write((char*)&gorevli, sizeof(gorevli));
            } else {
                bulundu = true;
            }
        }
    }
    oku.close();
    yaz.close();
    if (bulundu) {
        remove("gorevliler.dat");
        rename("yedek.dat", "gorevliler.dat");
        cout << "Kayýt baþarýyla silindi." << endl;
    } else {
        remove("yedek.dat");
        cout << "Kayýt bulunamadý..." << endl;
    }
}

void GorevliDuzenle() {
    char kimlik_no[12];
    cout << "Düzenlemek istediðiniz güvenlik görevlisinin kimlik numarasýný giriniz: ";
    cin >> kimlik_no;
    ifstream oku("gorevliler.dat", ios::binary);
    ofstream yaz("yedek.dat", ios::binary);
    bool bulundu = false;
    oku.seekg(0, ios::end);
    int kayitsayisi = oku.tellg() / sizeof(gorevli);
    if (kayitsayisi > 0) {
        oku.seekg(0, ios::beg);
        for (int i = 0; i < kayitsayisi; i++) {
            oku.read((char*)&gorevli, sizeof(gorevli));
            if (strcmp(gorevli.kimlik_no, kimlik_no) == 0) {
                cout << "Yeni bilgileri giriniz:" << endl;
                cout << "Ad: ";
                cin >> gorevli.ad;
                cout << "Soyad: ";
                cin >> gorevli.soyad;
                cout << "Görev: ";
                cin.ignore();
                cin.getline(gorevli.gorev, 100);
                bulundu = true;
            }
            yaz.write((char*)&gorevli, sizeof(gorevli));
        }
    }
    oku.close();
    yaz.close();
    if (bulundu) {
        remove("gorevliler.dat");
        rename("yedek.dat", "gorevliler.dat");
        cout << "Kayýt baþarýyla düzenlendi." << endl;
    } else {
        remove("yedek.dat");
        cout << "Kayýt bulunamadý..." << endl;
    }
}

