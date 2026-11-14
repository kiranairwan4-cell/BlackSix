#ifndef PERPUSTAKAAN_H
#define PERPUSTAKAAN_H
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
const int LAMA_PINJAM = 3;
const int DENDA_PER_HARI = 2000;
const int MAX_KETERLAMBATAN = 15;
const int BIAYA_GANTI_RUGI = 50000;
enum StatusBuku {
    DIKEMBALIKAN,
    DIPINJAM,
    HILANG
};

struct Buku {
    std::string kodeBuku;
    std::string judul;
    StatusBuku status;
    int denda;
    int gantiRugi;
};

struct Anggota {
    std::string idAnggota;
    std::string nama;
    std::string noTelp;
    bool statusAnggota;
    std::string kodeBuku;
    time_t tanggalPinjam;
    time_t tanggalJatuhTempo;
};

// Deklarasi fungsi
void MuatData(std::vector<Buku>& daftarBuku, std::vector<Anggota>& daftarAnggota);
void SimpanData(const std::vector<Buku>& daftarBuku, const std::vector<Anggota>& daftarAnggota);
void PinjamBuku(std::vector<Buku>& daftarBuku, std::vector<Anggota>& daftarAnggota);
void KembalikanBuku(std::vector<Buku>& daftarBuku, std::vector<Anggota>& daftarAnggota);
void LihatDaftarAnggota(std::vector<Anggota>& daftarAnggota, std::vector<Buku>& daftarBuku);
void HapusAnggota(std::vector<Buku>& daftarBuku, std::vector<Anggota>& daftarAnggota);
void CekBukuHilang(std::vector<Buku>& daftarBuku, std::vector<Anggota>& daftarAnggota);
#endif