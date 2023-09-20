# SoftwareProjectHW1

[project_NMF.pdf](https://github.com/Frostswing/TheSoftwareProject/files/12523480/project_NMF.pdf)

[tests data](https://docs.google.com/spreadsheets/d/1-6MauHUQOco9jXE3B6dBwO4hImGQSyMZbodNBbdyvIw/edit)

To run python on vscode press "F5"- NOT the green arrow on the top right

## compile on nova:

python3 setup.py build_ext --inplace

## run on nova:

python3 symnmf.py 3 symnmf ./olddata/tests/input_1.txt

## run all commands together:

rm ./build/lib.linux-x86_64-3.6/symnmfC.cpython-36m-x86_64-linux-gnu.so ; python3 setup.py build_ext --inplace ; python3 symnmf.py 3 symnmf ./olddata/tests/input_1.txt

# TheSoftwareProject
