// 12S25016-Loviska Astria Pasaribu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Untuk isdigit

#define MAX_TIKET 100
#define MAX_BUS_SEATS 30
#define MAX_NAMA 50
#define MAX_TANGGAL 11 // DD-MM-YYYY\0

// Struktur untuk data tiket
typedef struct {
    int id; // ID unik tiket
    char nama_penumpang[MAX_NAMA];
    char tujuan[MAX_NAMA];
    char tanggal_keberangkatan[MAX_TANGGAL];
    int nomor_kursi;
    char jenis_tiket[10]; // Ekonomi atau Eksekutif
    long harga_tiket;
    int status; // 0 = kosong, 1 = terisi
} Tiket;

// Array untuk menyimpan data tiket dan status kursi
Tiket daftar_tiket[MAX_TIKET];
int jumlah_tiket = 0;
int kursi_bus[MAX_BUS_SEATS + 1]; // Indeks 1-30

// Struktur untuk rute dan harga dasar ekonomi
typedef struct {
    char nama_rute[MAX_NAMA];
    long harga_ekonomi;
} Rute;

Rute daftar_rute[] = {
    {"Pematang Siantar", 50000},
    {"Parapat", 70000},
    {"Balige", 90000},
    {"Tarutung", 110000},
    {"Penyabungan", 150000}
};
int jumlah_rute = sizeof(daftar_rute) / sizeof(daftar_rute[0]);

// --- Prototype Fungsi ---
void inisialisasi_data();
void tampilkan_menu();
void pesan_tiket();
void lihat_daftar_tiket();
void cari_tiket();
void batalkan_tiket();
int get_available_seat();
void mark_seat(int seat_num, int status);
long hitung_harga(int id_rute, int jenis);
int is_valid_date(const char *date);
int get_rute_id(const char *tujuan);
void clear_input_buffer();
int validate_int_input();

// --- Implementasi Fungsi ---

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validate_int_input() {
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline

    for (int i = 0; buffer[i] != '\0'; i++) {
        if (!isdigit(buffer[i])) {
            printf("Input tidak valid. Harap masukkan angka.\n");
            return -1; // Indicate error
        }
    }
    return atoi(buffer);
}

void inisialisasi_data() {
    for (int i = 0; i < MAX_TIKET; i++) {
        daftar_tiket[i].id = 0; // Tandai tiket kosong
        daftar_tiket[i].status = 0; // Status 0 = kosong
    }
    for (int i = 1; i <= MAX_BUS_SEATS; i++) {
        kursi_bus[i] = 0; // 0 = kosong, 1 = terisi
    }
}

void tampilkan_menu() {
    printf("\n--- SISTEM TICKETING KOPERASI BINTANG TAPANULI (KBT) ---\n");
    printf("1. Pesan Tiket\n");
    printf("2. Lihat Daftar Tiket\n");
    printf("3. Cari Tiket Berdasarkan Nama\n");
    printf("4. Batalkan Tiket\n");
    printf("5. Keluar\n");
    printf("------------------------------------------------------\n");
    printf("Pilih menu: ");
}

int get_available_seat() {
    for (int i = 1; i <= MAX_BUS_SEATS; i++) {
        if (kursi_bus[i] == 0) {
            return i;
        }
    }
    return -1; // Tidak ada kursi kosong
}

void mark_seat(int seat_num, int status) {
    if (seat_num >= 1 && seat_num <= MAX_BUS_SEATS) {
        kursi_bus[seat_num] = status;
    }
}

long hitung_harga(int id_rute, int jenis) {
    long harga_dasar = daftar_rute[id_rute].harga_ekonomi;
    if (jenis == 1) { // Ekonomi
        return harga_dasar;
    } else if (jenis == 2) { // Eksekutif
        return harga_dasar + 50000;
    }
    return 0; // Error
}

int is_valid_date(const char *date) {
    // Format DD-MM-YYYY
    if (strlen(date) != 10) return 0;
    if (date[2] != '-' || date[5] != '-') return 0;

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(date[i])) return 0;
    }

    // Basic date validation (hari, bulan, tahun)
    int day = atoi(strndup(date, 2));
    int month = atoi(strndup(date + 3, 2));
    int year = atoi(strndup(date + 6, 4));

    if (year < 2024 || month < 1 || month > 12 || day < 1 || day > 31) return 0;
    // Add more complex date validation if needed (e.g., 30 hari di bulan X)
    return 1;
}

int get_rute_id(const char *tujuan) {
    for (int i = 0; i < jumlah_rute; i++) {
        if (strcmp(daftar_rute[i].nama_rute, tujuan) == 0) {
            return i;
        }
    }
    return -1; // Rute tidak ditemukan
}

void pesan_tiket() {
    if (jumlah_tiket >= MAX_TIKET) {
        printf("Maaf, kapasitas tiket penuh.\n");
        return;
    }

    int idx_tiket_baru = -1;
    for (int i = 0; i < MAX_TIKET; i++) {
        if (daftar_tiket[i].status == 0) { // Cari slot kosong
            idx_tiket_baru = i;
            break;
        }
    }

    if (idx_tiket_baru == -1) {
        printf("Error: Tidak ada slot tiket kosong yang ditemukan (ini seharusnya tidak terjadi jika MAX_TIKET > jumlah_tiket).\n");
        return;
    }

    Tiket *new_tiket = &daftar_tiket[idx_tiket_baru];
    new_tiket->id = idx_tiket_baru + 1; // ID tiket dimulai dari 1

    printf("\n--- PESAN TIKET BARU ---\n");

    // 1. Nama Penumpang
    printf("Nama Penumpang: ");
    fgets(new_tiket->nama_penumpang, MAX_NAMA, stdin);
    new_tiket->nama_penumpang[strcspn(new_tiket->nama_penumpang, "\n")] = 0; // Hapus newline

    // Validasi nama
    if (strlen(new_tiket->nama_penumpang) == 0) {
        printf("Error: Nama penumpang tidak boleh kosong.\n");
        new_tiket->status = 0; // Reset status
        return;
    }

    // 2. Tujuan
    printf("Pilih Tujuan:\n");
    for (int i = 0; i < jumlah_rute; i++) {
        printf("%d. Medan - %s\n", i + 1, daftar_rute[i].nama_rute);
    }
    printf("Pilihan rute (angka): ");
    int pilihan_rute = validate_int_input();
    if (pilihan_rute < 1 || pilihan_rute > jumlah_rute) {
        printf("Error: Pilihan rute tidak valid.\n");
        new_tiket->status = 0;
        return;
    }
    strcpy(new_tiket->tujuan, daftar_rute[pilihan_rute - 1].nama_rute);

    // 3. Tanggal Keberangkatan
    while (1) {
        printf("Tanggal Keberangkatan (DD-MM-YYYY): ");
        fgets(new_tiket->tanggal_keberangkatan, MAX_TANGGAL, stdin);
        new_tiket->tanggal_keberangkatan[strcspn(new_tiket->tanggal_keberangkatan, "\n")] = 0; // Hapus newline
        if (is_valid_date(new_tiket->tanggal_keberangkatan)) {
            break;
        } else {
            printf("Error: Format tanggal tidak valid atau tanggal tidak masuk akal. Harap masukkan format DD-MM-YYYY (contoh: 25-06-2026).\n");
        }
    }

    // 4. Jenis Tiket
    printf("Pilih Jenis Tiket:\n");
    printf("1. Ekonomi\n");
    printf("2. Eksekutif\n");
    printf("Pilihan jenis tiket (angka): ");
    int pilihan_jenis = validate_int_input();
    if (pilihan_jenis < 1 || pilihan_jenis > 2) {
        printf("Error: Pilihan jenis tiket tidak valid.\n");
        new_tiket->status = 0;
        return;
    }
    if (pilihan_jenis == 1) {
        strcpy(new_tiket->jenis_tiket, "Ekonomi");
    } else {
        strcpy(new_tiket->jenis_tiket, "Eksekutif");
    }

    // 5. Nomor Kursi Otomatis
    int kursi_kosong = get_available_seat();
    if (kursi_kosong == -1) {
        printf("Maaf, semua kursi bus sudah terisi.\n");
        new_tiket->status = 0;
        return;
    }
    new_tiket->nomor_kursi = kursi_kosong;
    mark_seat(kursi_kosong, 1); // Tandai kursi terisi

    // Hitung Harga Tiket
    new_tiket->harga_tiket = hitung_harga(pilihan_rute - 1, pilihan_jenis);

    new_tiket->status = 1; // Tandai tiket terisi
    jumlah_tiket++;
    printf("\nTiket berhasil dipesan!\n");
    printf("ID Tiket: %d\n", new_tiket->id);
    printf("Nama: %s\n", new_tiket->nama_penumpang);
    printf("Tujuan: Medan - %s\n", new_tiket->tujuan);
    printf("Tanggal: %s\n", new_tiket->tanggal_keberangkatan);
    printf("Kursi: %d\n", new_tiket->nomor_kursi);
    printf("Jenis: %s\n", new_tiket->jenis_tiket);
    printf("Harga: Rp %ld\n", new_tiket->harga_tiket);
}

void lihat_daftar_tiket() {
    printf("\n--- DAFTAR TIKET YANG TERSEDIA ---\n");
    if (jumlah_tiket == 0) {
        printf("Belum ada tiket yang dipesan.\n");
        return;
    }

    printf("----------------------------------------------------------------------------------------------------\n");
    printf("| ID  | Nama Penumpang      | Tujuan            | Tanggal       | Kursi | Jenis    | Harga        |\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < MAX_TIKET; i++) {
        if (daftar_tiket[i].status == 1) {
            printf("| %-3d | %-19s | Medan - %-10s | %-13s | %-5d | %-8s | Rp %-10ld |\n",
                   daftar_tiket[i].id,
                   daftar_tiket[i].nama_penumpang,
                   daftar_tiket[i].tujuan,
                   daftar_tiket[i].tanggal_keberangkatan,
                   daftar_tiket[i].nomor_kursi,
                   daftar_tiket[i].jenis_tiket,
                   daftar_tiket[i].harga_tiket);
        }
    }
    printf("----------------------------------------------------------------------------------------------------\n");
}

void cari_tiket() {
    printf("\n--- CARI TIKET BERDASARKAN NAMA PENUMPANG ---\n");
    if (jumlah_tiket == 0) {
        printf("Belum ada tiket yang dipesan.\n");
        return;
    }

    char nama_cari[MAX_NAMA];
    printf("Masukkan nama penumpang yang dicari: ");
    fgets(nama_cari, MAX_NAMA, stdin);
    nama_cari[strcspn(nama_cari, "\n")] = 0; // Hapus newline

    int ditemukan = 0;
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("| ID  | Nama Penumpang      | Tujuan            | Tanggal       | Kursi | Jenis    | Harga        |\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < MAX_TIKET; i++) {
        if (daftar_tiket[i].status == 1 && strstr(daftar_tiket[i].nama_penumpang, nama_cari) != NULL) {
            printf("| %-3d | %-19s | Medan - %-10s | %-13s | %-5d | %-8s | Rp %-10ld |\n",
                   daftar_tiket[i].id,
                   daftar_tiket[i].nama_penumpang,
                   daftar_tiket[i].tujuan,
                   daftar_tiket[i].tanggal_keberangkatan,
                   daftar_tiket[i].nomor_kursi,
                   daftar_tiket[i].jenis_tiket,
                   daftar_tiket[i].harga_tiket);
            ditemukan = 1;
        }
    }
    printf("----------------------------------------------------------------------------------------------------\n");

    if (!ditemukan) {
        printf("Tidak ditemukan tiket untuk nama '%s'.\n", nama_cari);
    }
}

void batalkan_tiket() {
    printf("\n--- BATALKAN TIKET ---\n");
    if (jumlah_tiket == 0) {
        printf("Belum ada tiket yang dipesan.\n");
        return;
    }

    printf("Masukkan ID tiket yang ingin dibatalkan: ");
    int id_batal = validate_int_input();

    if (id_batal == -1) { // Input non-integer
        return;
    }

    if (id_batal < 1 || id_batal > MAX_TIKET) {
        printf("Error: ID tiket tidak valid.\n");
        return;
    }

    int idx = id_batal - 1; // Konversi ID ke indeks array

    if (daftar_tiket[idx].status == 1) {
        printf("Anda yakin ingin membatalkan tiket atas nama '%s' (ID: %d, Kursi: %d)? (y/n): ",
               daftar_tiket[idx].nama_penumpang, daftar_tiket[idx].id, daftar_tiket[idx].nomor_kursi);
        char konfirmasi;
        scanf(" %c", &konfirmasi);
        clear_input_buffer();

        if (konfirmasi == 'y' || konfirmasi == 'Y') {
            mark_seat(daftar_tiket[idx].nomor_kursi, 0); // Kosongkan kursi
            daftar_tiket[idx].status = 0; // Set status tiket menjadi kosong
            jumlah_tiket--;
            printf("Tiket ID %d berhasil dibatalkan.\n", id_batal);
        } else {
            printf("Pembatalan tiket dibatalkan.\n");
        }
    } else {
        printf("Tiket dengan ID %d tidak ditemukan atau sudah dibatalkan.\n", id_batal);
    }
}


int main() {
    inisialisasi_data();
    int pilihan;

    do {
        tampilkan_menu();
        pilihan = validate_int_input();

        if (pilihan == -1) { // Input non-integer
            continue;
        }

        switch (pilihan) {
            case 1:
                pesan_tiket();
                break;
            case 2:
                lihat_daftar_tiket();
                break;
            case 3:
                cari_tiket();
                break;
            case 4:
                batalkan_tiket();
                break;
            case 5:
                printf("Terima kasih telah menggunakan layanan KBT. Sampai jumpa!\n");
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                break;
        }
        printf("\nTekan ENTER untuk melanjutkan...");
        clear_input_buffer(); // Memastikan buffer kosong sebelum membaca input berikutnya
    } while (pilihan != 5);

    return 0;
}
