#!/bin/bash
reg add "/?"
reg query "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager"
reg query "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\SafeDllSearchMode" || true
echo "setting key"
reg add "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\" "//v" "SafeDllSearchMode" "//d" "0" "//t" "REG_DWORD"
echo "checking key"
reg query "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\" "//v" "SafeDllSearchMode" || true
echo "directory"
reg query "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager"
echo $PATH
echo "PATH = ${PATH}"
echo "pwd = ${PWD}"
export PATH=${PATH}:${PWD}/atl/install/Release/lib:${PWD}/atl/install/Release/bin:${PWD}/dill/install/Release/lib:${PWD}/dill/install/Release/bin:${PWD}/dill/install/lib:${PWD}/dill/install/bin:${PWD}/atl/install/lib:${PWD}/atl/install/bin:
echo "newpath = ${PATH}"
echo 'export PATH=Spuriousdir:${PATH}:${PWD}/atl/install/Release/lib:${PWD}/atl/install/Release/bin:${PWD}/dill/install/Release/lib:${PWD}/dill/install/Release/bin:' >> ~/.bashrc
