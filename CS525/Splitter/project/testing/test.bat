rm chunk*
gcc0.exe -s 50 -o chunk0_ -i 120-byte-file
gcc1.exe -s 50 -o chunk1_ -i 120-byte-file
gcc2.exe -s 50 -o chunk2_ -i 120-byte-file
msc0.exe -s 50 -o chunk3_ -i 120-byte-file
msc1.exe -s 50 -o chunk4_ -i 120-byte-file
msc2.exe -s 50 -o chunk5_ -i 120-byte-file
bcc0.exe -s 50 -o chunk7_ -i 120-byte-file
bcc1.exe -s 50 -o chunk8_ -i 120-byte-file
bcc2.exe -s 50 -o chunk9_ -i 120-byte-file
gcc0.exe -s 50 -o chunk10_ -i 200-byte-file
gcc0.exe -s 50 -o chunk11_ -i 200-byte-file
gcc0.exe -s 50 -o chunk12_ -i 200-byte-file
msc0.exe -s 50 -o chunk13_ -i 200-byte-file
msc1.exe -s 50 -o chunk14_ -i 200-byte-file
msc2.exe -s 50 -o chunk15_ -i 200-byte-file
bcc0.exe -s 50 -o chunk16_ -i 200-byte-file
bcc1.exe -s 50 -o chunk17_ -i 200-byte-file
bcc2.exe -s 50 -o chunk18_ -i 200-byte-file

rm combined*
gcc0.exe -j -o combined0 -i chunk0_0001 chunk0_0002 chunk0_0003
gcc1.exe -j -o combined1 -i chunk1_0001 chunk1_0002 chunk1_0003
gcc2.exe -j -o combined2 -i chunk2_0001 chunk2_0002 chunk2_0003
msc0.exe -j -o combined3 -i chunk3_0001 chunk3_0002 chunk3_0003
msc1.exe -j -o combined4 -i chunk4_0001 chunk4_0002 chunk4_0003
msc2.exe -j -o combined5 -i chunk5_0001 chunk5_0002 chunk5_0003
bcc0.exe -j -o combined7 -i chunk7_0001 chunk7_0002 chunk7_0003
bcc1.exe -j -o combined8 -i chunk8_0001 chunk8_0002 chunk8_0003
bcc2.exe -j -o combined9 -i chunk9_0001 chunk9_0002 chunk9_0003
gcc0.exe -j -o combined10 -i chunk10_0001 chunk10_0002 chunk10_0003 chunk10_0004
gcc1.exe -j -o combined11 -i chunk11_0001 chunk11_0002 chunk11_0003 chunk11_0004
gcc2.exe -j -o combined12 -i chunk12_0001 chunk12_0002 chunk12_0003 chunk12_0004
msc0.exe -j -o combined13 -i chunk13_0001 chunk13_0002 chunk13_0003 chunk13_0004
msc1.exe -j -o combined14 -i chunk14_0001 chunk14_0002 chunk14_0003 chunk14_0004
msc2.exe -j -o combined15 -i chunk15_0001 chunk15_0002 chunk15_0003 chunk15_0004
bcc0.exe -j -o combined17 -i chunk16_0001 chunk16_0002 chunk16_0003 chunk16_0004
bcc1.exe -j -o combined18 -i chunk17_0001 chunk17_0002 chunk17_0003 chunk17_0004
bcc2.exe -j -o combined19 -i chunk18_0001 chunk18_0002 chunk18_0003 chunk18_0004


diff combined0 120-byte-file
diff combined1 120-byte-file
diff combined2 120-byte-file
diff combined3 120-byte-file
diff combined4 120-byte-file
diff combined5 120-byte-file
diff combined7 120-byte-file
diff combined8 120-byte-file
diff combined9 120-byte-file
diff combined10 200-byte-file
diff combined11 200-byte-file
diff combined12 200-byte-file
diff combined13 200-byte-file
diff combined14 200-byte-file
diff combined15 200-byte-file
diff combined17 200-byte-file
diff combined18 200-byte-file
diff combined19 200-byte-file