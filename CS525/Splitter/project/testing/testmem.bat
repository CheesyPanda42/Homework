make bcc_cg

rm *.cgl
bcc_cg.exe -s 50 -o chunkcg_ -i 120-byte-file
cat bcc_cg.cgl


rm *.cgl
bcc_cg.exe -s 50 -o chunkcg_ -i 200-byte-file
cat bcc_cg.cgl

rm *.cgl
bcc_cg.exe -j -o combinedcg -i chunkcg_0001 chunkcg_0002 chunkcg_0003
cat bcc_cg.cgl

rm *.cgl
bcc_cg.exe -j -o combinedcg -i chunkcg_0001 chunkcg_0002 chunkcg_0003
cat bcc_cg.cgl