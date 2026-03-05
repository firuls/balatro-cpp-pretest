# Balatro CPP Pretest

Game ini adalah versi terminal (CLI) bergaya Balatro dengan sistem dadu, combo, shop, dan boss round.

## Cara membuka / menjalankan game

### Opsi 1: Langsung jalankan file `.exe`
1. Buka folder project ini.
2. Jalankan `DiceRunGame.exe` (double click), atau lewat terminal:

```powershell
.\DiceRunGame.exe
```

### Opsi 2: Compile dulu dari source
Jika ingin build ulang:

```powershell
g++ src/*.cpp -o DiceRunGame.exe
.\DiceRunGame.exe
```

## Cara bermain

1. Game berjalan per run, terdiri dari 4 round (round 4 adalah boss round).
2. Tiap round kamu punya:
   - `Commit Left`: jumlah kesempatan mengunci skor lemparan.
   - `Reroll Left`: jumlah kesempatan reroll dadu.
3. Di setiap giliran, pilih menu:
   - `1. Commit Dice` untuk ambil skor kombinasi dadu saat ini.
   - `2. Reroll Dice` untuk mengacak ulang (selama reroll masih ada).
   - `0. End Round` untuk mengakhiri round lebih cepat.
4. Capai target skor round agar bisa lanjut:
   - Round 1: `300`
   - Round 2: `500`
   - Round 3: `700`
   - Boss Round: default `900` (bisa berubah sesuai efek boss).
5. Setelah lolos round 1-3, kamu dapat coin dan masuk shop untuk beli upgrade/modifier.
6. Kalau skor round di bawah target, run langsung kalah.
7. Setelah run selesai, pilih `y/n` saat muncul `Play again?` untuk main lagi atau keluar.

## Catatan singkat combo

Di awal round normal, daftar combo dan syaratnya akan ditampilkan di game, contoh:
- `Doble Luck`: minimal 1 pasang angka sama.
- `Chain`: ada urutan 3 angka berurutan.
- `Golden Chain`: 5 dadu berurutan (straight penuh).

Gunakan preview score/multiplier di layar untuk memutuskan kapan commit atau reroll.
