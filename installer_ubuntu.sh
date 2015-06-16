sudo add-apt-repository ppa:joseluisblancoc/mrpt
sudo apt-get update
sudo apt-get install libmrpt-dev mrpt-apps  
sudo apt-get install libsfml-dev
sudo apt-get build-dev libsfml

#wget http://www.sfml-dev.org/files/SFML-2.3-linux-gcc-64-bit.tar.gz
#tar zxfv SFML-2.3-linux-gcc-64-bit.tar.gz

git clone https://openspur.org/repos/yp-spur.git
cd yp-spur
./configure --prefix=/usr/local
make
sudo make install
sudo ldconfig


mkdir log
