#include "perpustakaan.h"
#include <fstream>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;
// MUAT DATA
void MuatData(vector<Buku>& daftarBuku, vector<Anggota>& daftarAnggota) {
    daftarBuku.clear();
    daftarAnggota.clear();
    ifstream fileBuku("buku.txt");
    if (fileBuku) {
        string kode, judul;
        int status, denda, gantiRugi;
        while (getline(fileBuku, kode, '|') &&
               getline(fileBuku, judul, '|') &&
               (fileBuku >> status) && fileBuku.ignore(1) &&
               (fileBuku >> denda) && fileBuku.ignore(1) &&
               (fileBuku >> gantiRugi)) {
            fileBuku.ignore(1);
            daftarBuku.push_back({
                kode, judul,
                static_cast<StatusBuku>(status),
                denda, gantiRugi
            });
        }
    }
    ifstream fileAnggota("anggota.txt");
    if (fileAnggota) {
        string id, nama, noTelp, kodeBuku;
        bool statusAnggota;
        time_t tanggalPinjam, tanggalJatuhTempo;

        while (getline(fileAnggota, id, '|') &&
               getline(fileAnggota, nama, '|') &&
               getline(fileAnggota, noTelp, '|') &&
               (fileAnggota >> statusAnggota)) {
            fileAnggota.ignore(1);
            getline(fileAnggota, kodeBuku, '|');
            fileAnggota >> tanggalPinjam;
            fileAnggota.ignore(1);
            fileAnggota >> tanggalJatuhTempo;
            fileAnggota.ignore(1);
            daftarAnggota.push_back({
                id, nama, noTelp, statusAnggota,
                kodeBuku, tanggalPinjam, tanggalJatuhTempo
            });
        }
    }
}
// PINJAM BUKU
void PinjamBuku(vector<Buku>& daftarBuku, vector<Anggota>& daftarAnggota) {
    cout << "\n=== PINJAM BUKU ===\n";
    string idAnggota;
    int idxAnggota = -1;
    cin.ignore();
    while (true) {
        cout << "Masukkan ID Anggota (jika belum daftar ketik 'tidak'): ";
        getline(cin, idAnggota);
        if (idAnggota.empty()) {
            cout << " ID Anggota tidak boleh kosong!\n";
            continue;
        }
        if (idAnggota == "tidak" || idAnggota == "Tidak" || idAnggota == "TIDAK") {
            string jawab;
            while (true) {
                cout << "Apa ingin mendaftar sebagai anggota? (ya/tidak): ";
                getline(cin, jawab);
                if (jawab == "ya" || jawab == "Ya" || jawab == "YA")
                    break;
                if (jawab == "tidak" || jawab == "Tidak" || jawab == "TIDAK") {
                    cout << "Kembali ke menu utama.\n";
                    return;
                }
                cout << " Masukkan hanya ya atau tidak!\n";
            }
            cout << "\n--- Pendaftaran Anggota Baru ---\n";
            string nama, noTelp;
            while (true) {
                cout << "Nama Lengkap: ";
                getline(cin, nama);

                if (nama.empty())
                    cout << "Nama tidak boleh kosong!\n";
                else
                    break;
            }
            while (true) {
                cout << "No Telp: ";
                getline(cin, noTelp);
                if (noTelp.empty()) {
                    cout << "No Telp tidak boleh kosong!\n";
                    continue;
                }
                if (noTelp.length() < 11 || noTelp.length() > 13) {
                    cout << " Nomor telepon harus 11 - 13 digit!\n";
                    continue;
                }
                bool valid = true;
                for (char c : noTelp) {
                    if (!isdigit(c)) {
                        valid = false;
                        break;
                    }
                }
                if (!valid) {
                    cout << " Nomor telepon hanya boleh berisi angka!\n";
                    continue;
                }
                bool noTelpAda = false;
                for (const auto& anggota : daftarAnggota) {
                    if (anggota.noTelp == noTelp) {
                        noTelpAda = true;
                        break;
                    }
                }
                if (noTelpAda) {
                    cout << " Nomor telepon sudah dipakai anggota lain!\n";
                    continue;
                }
                break;
            } 
            int nomor = static_cast<int>(daftarAnggota.size()) + 1;
            string idBaru = "A";
            if (nomor < 10) idBaru += "00";
            else if (nomor < 100) idBaru += "0";
            idBaru += to_string(nomor);
            Anggota anggotaBaru = {idBaru, nama, noTelp, false, "", 0, 0};
            daftarAnggota.push_back(anggotaBaru);
            cout << "[BERHASIL] Terdaftar! ID Anda: " << idBaru << "\n";
            cout << "\nSilakan masukkan ID tersebut untuk meminjam: ";
            getline(cin, idAnggota);

        } 
        idxAnggota = -1;
        for (size_t i = 0; i < daftarAnggota.size(); i++) {
            if (daftarAnggota[i].idAnggota == idAnggota) {
                idxAnggota = static_cast<int>(i);
                break;
            }
        }
        if (idxAnggota == -1) {
            cout << " ID Anggota tidak terdaftar!\n";
            continue;
        }
        if (daftarAnggota[idxAnggota].statusAnggota) {
            cout << " Anggota sedang meminjam buku lain!\n";
            return;
        }
        break;
    }
    string kodeBuku;
    cout << "========================\n"
         << "      DAFTAR BUKU       \n"
         << "========================\n";
    for (const auto& b : daftarBuku) {
        if (b.status == 0) {
            cout << b.kodeBuku << " | " << b.judul << endl;
        }
    }
    while (true) {
        cout << "\nMasukkan Kode Buku: ";
        getline(cin, kodeBuku);

        if (kodeBuku.empty())
            cout << " Kode Buku tidak boleh kosong!\n";
        else
            break;
    }
    int idxBuku = -1;
    for (size_t i = 0; i < daftarBuku.size(); i++) {
        if (daftarBuku[i].kodeBuku == kodeBuku) {
            idxBuku = static_cast<int>(i);
            break;
        }
    }
    if (idxBuku == -1) {
        cout << " Kode buku tidak ditemukan! Pilih kode dari daftar di atas.";
        return;
    }
    // MEMERIKSA STATUS BUKU
    if (daftarBuku[idxBuku].status == HILANG) {
        cout << " Buku berstatus hilang!\n";
        return;
    }
    if (daftarBuku[idxBuku].status == DIPINJAM) {
        cout << " Buku sedang dipinjam!\n";
        return;
    }
    for (const auto& anggota : daftarAnggota) {
        if (anggota.statusAnggota &&
            anggota.kodeBuku == kodeBuku &&
            anggota.idAnggota != idAnggota) {

            cout << " Buku sedang dipinjam oleh Anggota Lain!\n";
            return;
        }
    }
    // MENCATAT TANGGAL PEMINJAMAN
    time_t tglPinjam = time(nullptr);
    tm* tglPinjamTmPtr = localtime(&tglPinjam);
    if (tglPinjamTmPtr == nullptr) {
        cout << " Gagal membaca tanggal peminjaman!\n";
        return;
    }
    tm tglPinjamTm = *tglPinjamTmPtr;
    tm tglJatuhTm = tglPinjamTm;
    tglJatuhTm.tm_mday += LAMA_PINJAM;
    time_t tglJatuhTempo = mktime(&tglJatuhTm);
    daftarAnggota[idxAnggota].statusAnggota = true;
    daftarAnggota[idxAnggota].kodeBuku = kodeBuku;
    daftarAnggota[idxAnggota].tanggalPinjam = tglPinjam;
    daftarAnggota[idxAnggota].tanggalJatuhTempo = tglJatuhTempo;
    daftarBuku[idxBuku].status = DIPINJAM;
    char strPinjam[20], strJatuh[20];
    strftime(strPinjam, sizeof(strPinjam), "%d-%m-%Y", &tglPinjamTm);
    strftime(strJatuh, sizeof(strJatuh), "%d-%m-%Y", &tglJatuhTm);
    cout << "\nKode Buku           : " << daftarBuku[idxBuku].kodeBuku << "\n";
    cout << "Judul Buku          : " << daftarBuku[idxBuku].judul << "\n";
    cout << "Tanggal Pinjam      : " << strPinjam << "\n";
    cout << "Tanggal Jatuh Tempo : " << strJatuh << "\n";

    cout << "\n[BERHASIL] Peminjaman tercatat!\n";
}
void KembalikanBuku(vector<Buku>& daftarBuku, vector<Anggota>& daftarAnggota) {
    cout << "\n=== KEMBALIKAN BUKU ===\n";
    string idAnggota, kodeBuku, inputTanggal;
    int tgl, bln, thn, tambahan;
    cin.ignore();
    while (true) {
        cout << "Masukkan ID Anggota: ";
        getline(cin, idAnggota);
        if (idAnggota.empty()) {
            cout << "ID anggota tidak boleh kosong!\n";
        }
        else if (idAnggota.find_first_not_of(
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
            != string::npos) {
            cout << "ID anggota tidak valid!\n";
        }
        else {
            break;
        }
    }
    while (true) {
        cout << "Masukkan Kode Buku: ";
        getline(cin, kodeBuku);

        if (kodeBuku.empty()) {
            cout << "Kode buku tidak boleh kosong!\n";
        }
        else if (kodeBuku.find_first_not_of(
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
            != string::npos) {
            cout << "Kode buku tidak valid!\n";
        }
        else {
            break;
        }
    }
    int idxAnggota = -1;
    for (size_t i = 0; i < daftarAnggota.size(); i++) {
        if (daftarAnggota[i].idAnggota == idAnggota &&
            daftarAnggota[i].kodeBuku == kodeBuku &&
            daftarAnggota[i].statusAnggota) {

            idxAnggota = static_cast<int>(i);
            break;
        }
    }
    if (idxAnggota == -1) {
        cout << "\n Data peminjaman tidak ditemukan!\n";
        return;
    }
    time_t dataPinjam = daftarAnggota[idxAnggota].tanggalPinjam;
    tm* pinjamTmPtr = localtime(&dataPinjam);
    if (pinjamTmPtr == nullptr) {
        cout << " Data tanggal peminjaman tidak valid!\n";
        return;
    }
    tm tanggalPinjam = {};
    tanggalPinjam.tm_mday = pinjamTmPtr->tm_mday;
    tanggalPinjam.tm_mon = pinjamTmPtr->tm_mon;
    tanggalPinjam.tm_year = pinjamTmPtr->tm_year;
    time_t tanggalPinjamNormal = mktime(&tanggalPinjam);
    tm kembaliTm = {};
    time_t dataKembali;
    while (true) {
        cout << "Masukkan Tanggal Pengembalian (tanggal bulan tahun): ";
        getline(cin, inputTanggal);
        if (inputTanggal.empty()) {
            cout << " Tanggal pengembalian tidak boleh kosong!\n";
            continue;
        }
        stringstream ss(inputTanggal);
        if (!(ss >> tgl >> bln >> thn) || (ss >> tambahan)) {
            cout << " Format tanggal harus (tanggal bulan tahun)\n";
            cout << "Contoh: 1 11 2026\n";
            continue;
        }
        kembaliTm = {};
        kembaliTm.tm_mday = tgl;
        kembaliTm.tm_mon = bln - 1;
        kembaliTm.tm_year = thn - 1900;
        dataKembali = mktime(&kembaliTm);
        tm* cekTanggal = localtime(&dataKembali);
        if (cekTanggal == nullptr ||
            cekTanggal->tm_mday != tgl ||
            cekTanggal->tm_mon != bln - 1 ||
            cekTanggal->tm_year != thn - 1900) {
            cout << " Tanggal Pengembalian Tidak Valid!\n";
            continue;
        }
        // DITAMBAHKAN: TANGGAL TIDAK BOLEH SEBELUM TANGGAL PINJAM
        if (dataKembali < tanggalPinjamNormal) {
            cout << " Tanggal pengembalian tidak boleh sebelum tanggal peminjaman.\n";
            continue;
        }
        break;
    }
    // MEMERIKSA TANGGAL JATUH TEMPO
    time_t dataJatuhTempo = daftarAnggota[idxAnggota].tanggalJatuhTempo;

    tm* jatuhTmPtr = localtime(&dataJatuhTempo);
    if (jatuhTmPtr == nullptr) {
        cout << " Data tanggal jatuh tempo tidak valid!\n";
        return;
    }
    tm tanggalJatuh = {};
    tanggalJatuh.tm_mday = jatuhTmPtr->tm_mday;
    tanggalJatuh.tm_mon = jatuhTmPtr->tm_mon;
    tanggalJatuh.tm_year = jatuhTmPtr->tm_year;
    time_t dataJatuh = mktime(&tanggalJatuh);
    // MENAMPILKAN TANGGAL
    char strJatuh[20], strKembali[20];
    strftime(strJatuh, sizeof(strJatuh), "%d-%m-%Y", &tanggalJatuh);
    strftime(strKembali, sizeof(strKembali), "%d-%m-%Y", &kembaliTm);
    cout << "\nJatuh Tempo   : " << strJatuh << "\n";
    cout << "Dikembalikan  : " << strKembali << "\n";
    // MENGHITUNG DENDA
    double selisih = difftime(dataKembali, dataJatuh);
    int hariTerlambat = static_cast<int>(selisih / (24 * 60 * 60));
    if (hariTerlambat < 0)
        hariTerlambat = 0;
    int totalDenda = hariTerlambat * DENDA_PER_HARI;
    if (hariTerlambat > 0) {
        cout << "Terlambat     : " << hariTerlambat << " hari\n";
        cout << "Denda         : Rp " << totalDenda << "\n";
    }
    else {
        cout << "Terlambat : " << hariTerlambat << endl;
        cout << "Denda     : " << totalDenda << endl;
    }
    // MEMPERBARUI STATUS BUKU
    for (auto& b : daftarBuku) {
        if (b.kodeBuku == kodeBuku) {
            b.status = DIKEMBALIKAN;
            b.denda = totalDenda;
            break;
        }
    }
    // MEMPERBARUI DATA ANGGOTA
    daftarAnggota[idxAnggota].statusAnggota = false;
    daftarAnggota[idxAnggota].kodeBuku = "";
    daftarAnggota[idxAnggota].tanggalPinjam = 0;
    daftarAnggota[idxAnggota].tanggalJatuhTempo = 0;
    cout << "\n[BERHASIL] Buku dikembalikan!\n";
}
// LIHAT DAFTAR ANGGOTA
void LihatDaftarAnggota(vector<Anggota>& daftarAnggota, vector<Buku>& daftarBuku) {
    cout << "\n=== DAFTAR ANGGOTA ===\n";
    if (daftarAnggota.empty()) {
        cout << "Belum ada anggota.\n";
        return;
    }
    cout << left;
    cout << setw(8) << "No"
         << setw(12) << "ID Anggota"
         << setw(18) << "Nama"
         << setw(15) << "No. Telp"
         << setw(12) << "Kode Buku"
         << setw(25) << "Judul Buku"
         << setw(18) << "Status"
         << setw(26) << "Tanggal Jatuh Tempo"
         << "\n";
    cout << string(127, '-') << "\n";
    for (size_t i = 0; i < daftarAnggota.size(); i++) {
        string status;
        string kodeBuku = "-";
        string judulBuku = "-";
        string tanggalTempo = "-";
        if (daftarAnggota[i].statusAnggota) {
            status = "Meminjam";
            kodeBuku = daftarAnggota[i].kodeBuku;
            for (const auto& buku : daftarBuku) {
                if (buku.kodeBuku == kodeBuku) {
                    judulBuku = buku.judul;
                    break;
                }
            }
            time_t waktu = daftarAnggota[i].tanggalJatuhTempo;
            tm* waktuTm = localtime(&waktu);
            if (waktuTm != nullptr) {
                char tanggal[20];
                strftime(
                    tanggal,
                    sizeof(tanggal),
                    "%d-%m-%Y",
                    waktuTm
                );
                tanggalTempo = tanggal;
            }
        } else {
            status = "Tidak Meminjam";
        }
        cout << setw(8) << i + 1
             << setw(12) << daftarAnggota[i].idAnggota
             << setw(18) << daftarAnggota[i].nama
             << setw(15) << daftarAnggota[i].noTelp
             << setw(12) << kodeBuku
             << setw(25) << judulBuku
             << setw(18) << status
             << setw(26) << tanggalTempo
             << "\n";
    }

    cout << "\nTotal: "
         << daftarAnggota.size()
         << " anggota\n";
}    
// HAPUS ANGGOTA
void HapusAnggota(vector<Buku>& daftarBuku, vector<Anggota>& daftarAnggota) {
    cout << "\n=== HAPUS ANGGOTA ===\n";
    if (daftarAnggota.empty()) {
        cout << "Belum ada anggota terdaftar.\n";
        return;
    }
    string idAnggota;
    cin.ignore();
    cout << "Masukkan ID Anggota yang akan dihapus: ";
    getline(cin, idAnggota);
    int idx = -1;
    for (size_t i = 0; i < daftarAnggota.size(); i++) {
        if (daftarAnggota[i].idAnggota == idAnggota) {
            idx = static_cast<int>(i);
            break;
        }
    }
    if (idx == -1) {
        cout << " ID Anggota tidak ditemukan!\n";
        return;
    }
    if (daftarAnggota[idx].statusAnggota) {
        cout << " Anggota ini sedang meminjam buku! Kembalikan buku dulu.\n";
        return;
    }
    daftarAnggota.erase(daftarAnggota.begin() + idx);
    cout << "\n[BERHASIL] Anggota dengan ID " << idAnggota<< " telah dihapus!\n";
}
// CEK BUKU TERLAMBAT / HILANG
void CekBukuHilang(vector<Buku>& daftarBuku, vector<Anggota>& daftarAnggota) {
    cout << "\n=== CEK BUKU TERLAMBAT / HILANG ===\n";
    if (daftarAnggota.empty()) {
        cout << "Belum ada anggota terdaftar.\n";
        return;
    }
    time_t sekarang = time(nullptr);
    int jumlahTerlambat = 0;
    cout << "\n=== DAFTAR ANGGOTA TERLAMBAT ===\n";
    cout << left
         << setw(12) << "ID Anggota"
         << setw(18) << "Nama"
         << setw(12) << "Kode Buku"
         << setw(12) << "Terlambat" << "\n";
    cout << string(54, '-') << "\n";
    for (const auto& a : daftarAnggota) {
        if (!a.statusAnggota)
            continue;
        double selisih = difftime(sekarang, a.tanggalJatuhTempo);
        int hariTerlambat = static_cast<int>( selisih / (24 * 60 * 60));
        if (hariTerlambat > 15) {
            cout << setw(12) << a.idAnggota
                 << setw(18) << a.nama
                 << setw(12) << a.kodeBuku
                 << hariTerlambat << " hari\n";
            jumlahTerlambat++;
        }
    }
    if (jumlahTerlambat == 0) {
        cout << "Tidak ada anggota yang terlambat lebih dari 15 hari.\n";
        return;
    }
    string idAnggota;
    cout << "\nMasukkan ID Anggota yang akan diproses: ";
    cin >> idAnggota;
    int idxAnggota = -1;
    for (size_t i = 0; i < daftarAnggota.size(); i++) {
        if (daftarAnggota[i].idAnggota == idAnggota) {
            idxAnggota = static_cast<int>(i);
            break;
        }
    }
    if (idxAnggota == -1) {
        cout << " ID Anggota tidak ditemukan!\n";
        return;
    }
    if (!daftarAnggota[idxAnggota].statusAnggota) {
        cout << " Anggota tidak sedang meminjam buku!\n";
        return;
    }
    int hariTerlambat = static_cast<int>(difftime(sekarang, daftarAnggota[idxAnggota].tanggalJatuhTempo) / (24 * 60 * 60));
    if (hariTerlambat <= 15) {
        cout << " Anggota belum terlambat lebih dari 15 hari.\n";
        return;
    }
    string kode = daftarAnggota[idxAnggota].kodeBuku;
    int idxBuku = -1;
    for (size_t i = 0; i < daftarBuku.size(); i++) {
        if (daftarBuku[i].kodeBuku == kode) {
            idxBuku = static_cast<int>(i);
            break;
        }
    }
    if (idxBuku == -1) {
        cout << " Data buku tidak ditemukan!\n";
        return;
    }
    cout << "\n=== DATA ANGGOTA ===\n";
    cout << "ID Anggota : " << daftarAnggota[idxAnggota].idAnggota << "\n";
    cout << "Nama       : " << daftarAnggota[idxAnggota].nama << "\n";
    cout << "No. Telp   : " << daftarAnggota[idxAnggota].noTelp << "\n";
    cout << "Kode Buku  : " << kode << "\n";
    cout << "Judul Buku : " << daftarBuku[idxBuku].judul << "\n";
    cout << "Terlambat  : " << hariTerlambat << " hari\n";

    cout << "\n[PETUGAS] Silakan hubungi anggota yang meminjam buku.\n";
    char jawab;
    while (true) {
        cout << "Apakah buku hilang? (y/t): ";
        cin >> jawab;

        if (jawab == 'y' || jawab == 'Y' ||
            jawab == 't' || jawab == 'T') {
            break;
        }

        cout << " Masukkan hanya y atau t!\n";
    }
    if (jawab == 'y' || jawab == 'Y') {

        int dendaTerlambat = hariTerlambat * DENDA_PER_HARI;
        int totalBayar = BIAYA_GANTI_RUGI + dendaTerlambat;

        cout << "\n=== RINCIAN GANTI RUGI ===\n";
        cout << "Biaya ganti rugi    : Rp " << BIAYA_GANTI_RUGI << "\n";
        cout << "Denda keterlambatan : Rp " << dendaTerlambat << "\n";
        cout << "TOTAL PEMBAYARAN    : Rp " << totalBayar << "\n";
        char bayar;

        while (true) {
            cout << "\nApakah denda sudah dibayar? (y/t): ";
            cin >> bayar;

            if (bayar == 'y' || bayar == 'Y' ||
                bayar == 't' || bayar == 'T') {
                break;
            }
            cout << " Masukkan hanya y atau t!\n";
        }
        if (bayar == 'y' || bayar == 'Y') {
            cout << " Pembayaran berhasil dikonfirmasi.\n";
            daftarBuku[idxBuku].status = HILANG;
            daftarBuku[idxBuku].denda = totalBayar;
            daftarBuku[idxBuku].gantiRugi = BIAYA_GANTI_RUGI;
            cout << " Status buku menjadi HILANG.\n";
        } else {
            cout << " Pembayaran belum dilakukan.\n";
            cout << " Data buku dan anggota tidak diperbarui.\n";
            return;
        }
    }
    else {
        int dendaTerlambat = hariTerlambat * DENDA_PER_HARI;
        cout << "\n=== DENDA KETERLAMBATAN ===\n";
        cout << "Denda keterlambatan : Rp " << dendaTerlambat << "\n";
        // MEMERIKSA PEMBAYARAN
        char bayar;
        while (true) {
            cout << "\nApakah denda sudah dibayar? (y/t): ";
            cin >> bayar;
            if (bayar == 'y' || bayar == 'Y' ||
                bayar == 't' || bayar == 'T') {
                break;
            }
            cout << " Masukkan hanya y atau t!\n";
        }
        if (bayar == 'y' || bayar == 'Y') {

            cout << " Pembayaran berhasil dikonfirmasi.\n";
            daftarBuku[idxBuku].status = DIKEMBALIKAN;
            daftarBuku[idxBuku].denda = dendaTerlambat;
            cout << " Status buku menjadi DIKEMBALIKAN.\n";
        } else {
            cout << " Pembayaran belum dilakukan.\n";
            cout << " Data buku dan anggota tidak diperbarui.\n";
            return;
        }
    }
    daftarAnggota[idxAnggota].statusAnggota = false;
    daftarAnggota[idxAnggota].kodeBuku = "";
    daftarAnggota[idxAnggota].tanggalPinjam = 0;
    daftarAnggota[idxAnggota].tanggalJatuhTempo = 0;
    cout << "\n[BERHASIL] Pembayaran dan pembaruan data berhasil.\n";
}

void SimpanData(const vector<Buku>& daftarBuku, const vector<Anggota>& daftarAnggota) {
    ofstream fileBuku("buku.txt");
    if (!fileBuku.is_open()) {
        cout << " Gagal membuka berkas buku.txt untuk menyimpan!\n";
        return;
    }
    for (const auto& b : daftarBuku) {
        int kodeStatus;
        if (b.status == DIKEMBALIKAN)
            kodeStatus = 0;
        else if (b.status == DIPINJAM) kodeStatus = 1;
        else if (b.status == HILANG) kodeStatus = 2;
        else kodeStatus = 0;
        fileBuku << b.kodeBuku << "|"
                 << b.judul << "|"
                 << kodeStatus << "|"
                 << b.denda << "|"
                 << b.gantiRugi << "\n";
    }
    fileBuku.close();
    cout << "[BERHASIL] Tersimpan " << daftarBuku.size() << " data buku.\n";
    ofstream fileAnggota("anggota.txt");
    if (!fileAnggota.is_open()) {
        cout << " Gagal membuka berkas anggota.txt untuk menyimpan!\n";
        return;
    }
    for (const auto& a : daftarAnggota) {
        fileAnggota << a.idAnggota << "|"<< a.nama << "|"
                    << a.noTelp << "|"
                    << a.statusAnggota << "|"
                    << a.kodeBuku << "|"
                    << a.tanggalPinjam << "|"
                    << a.tanggalJatuhTempo << "\n";
    }
    fileAnggota.close();
    cout << "[BERHASIL] Tersimpan " << daftarAnggota.size() << " data anggota.\n";
}