#include "perpustakaan.h"
#include <vector>
#include <iostream>
using namespace std;
int main() {
    vector<Buku> daftarBuku;
    vector<Anggota> daftarAnggota;
    MuatData(daftarBuku, daftarAnggota);
    int menu;
    do {
        cout << "===== MENU PERPUSTAKAAN =====\n";
        cout << "1. Pinjam Buku\n";
        cout << "2. Kembalikan Buku\n";
        cout << "3. Lihat Daftar Anggota\n";
        cout << "4. Hapus Anggota\n";
        cout << "5. Cek Buku Hilang\n";
        cout << "6. Simpan Data & Keluar\n";
        cout << "=============================\n";
        cout << "Masukkan pilihan: ";
        if (!(cin >> menu) || cin.peek() != '\n') {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Input tidak valid! Pilih Angka 1-6\n";
            menu = 0;
            continue;
        }
        if (menu < 1 || menu > 6) {
            cout << "Pilihan hanya 1 sampai 6!\n";
            continue;
        }
        switch (menu) {
            case 1:
                PinjamBuku(daftarBuku, daftarAnggota);
                break;
            case 2:
                KembalikanBuku(daftarBuku, daftarAnggota);
                break;
            case 3:
                LihatDaftarAnggota(daftarAnggota, daftarBuku);
                break;
            case 4:
                HapusAnggota(daftarBuku, daftarAnggota);
                break;
            case 5:
                CekBukuHilang(daftarBuku, daftarAnggota);
                break;
            case 6:
                cout << "Menyimpan data...\n";
                SimpanData(daftarBuku, daftarAnggota);
                cout << "Terima kasih!\n";
                return 0; 
            default:
                cout << "Pilihan tidak ada! Coba lagi.\n";
                break;
        }
    } while (menu != 6); 
}