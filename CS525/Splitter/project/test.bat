make clean
make gcc_c
make gcc_c_cpp
make gcc_cpp
make msc_c
make msc_c_cpp
make msc_cpp
make bcc_c
make bcc_c_cpp
make bcc_cpp

./gcc0.exe -s -o chunk0_ -i 120-byte-file
./gcc1.exe -s -o chunk1_ -i 120-byte-file
./gcc2.exe -s -o chunk2_ -i 120-byte-file
./Femsc0.exe -s -o chunk3_ -i 120-byte-file
./Femsc1.exe -s -o chunk4_ -i 120-byte-file
./Femsc2.exe -s -o chunk5_ -i 120-byte-file
./Femsc3.exe -s -o chunk6_ -i 120-byte-file
./ebcc0.exe -s -o chunk7_ -i 120-byte-file
./ebcc1.exe -s -o chunk8_ -i 120-byte-file
./ebcc2.exe -s -o chunk8_ -i 120-byte-file
./gcc0.exe -s -o chunk9_ -i 200-byte-file
./gcc0.exe -s -o chunk10_ -i 200-byte-file
./gcc0.exe -s -o chunk11_ -i 200-byte-file
./Femsc0.exe -s -o chunk12_ -i 200-byte-file
./Femsc1.exe -s -o chunk13_ -i 200-byte-file
./Femsc2.exe -s -o chunk14_ -i 200-byte-file
./ebcc0.exe -s -o chunk15_ -i 200-byte-file
./ebcc1.exe -s -o chunk16_ -i 200-byte-file
./ebcc2.exe -s -o chunk17_ -i 200-byte-file

