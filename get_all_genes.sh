#!/bin/bash

mkdir -p output

echo "HLA-A"; ./bin/main -f -g HLA-A --output output/HLA_A.fa
echo "HLA-B"; ./bin/main -f -g HLA-B --output output/HLA_B.fa
echo "HLA-C"; ./bin/main -f -g HLA-C --output output/HLA_C.fa
echo "HLA-DQA1"; ./bin/main -f -g HLA-DQA1 --output output/HLA_DQA1.fa
echo "HLA-DQB1"; ./bin/main -f -g HLA-DQB1 --output output/HLA_DQB1.fa
echo "HLA-DRB"; ./bin/main -f -g HLA-DRB --output output/HLA_DRB.fa
echo "HLA-DMA"; ./bin/main -f -g HLA-DMA --output output/HLA_DMA.fa
echo "HLA-DMB"; ./bin/main -f -g HLA-DMB --output output/HLA_DMB.fa
echo "HLA-DOA"; ./bin/main -f -g HLA-DOA --output output/HLA_DOA.fa
echo "HLA-DOB"; ./bin/main -f -g HLA-DOB --output output/HLA_DOB.fa
echo "HLA-DPA1"; ./bin/main -f -g HLA-DPA1 --output output/HLA_DPA1.fa
echo "HLA-DPB1"; ./bin/main -f -g HLA-DPB1 --output output/HLA_DPB1.fa
echo "HLA-DRA"; ./bin/main -f -g HLA-DRA --output output/HLA_DRA.fa
echo "HLA-E"; ./bin/main -f -g HLA-E --output output/HLA_E.fa
echo "HLA-F"; ./bin/main -f -g HLA-F --output output/HLA_F.fa
echo "HLA-G"; ./bin/main -f -g HLA-G --output output/HLA_G.fa
