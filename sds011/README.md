# SDS011-driver

all code is written by CHATGPT/ Copilot so far

This project now lives in `embedded/sds011`. The commands below use
`~/github/embedded`; skip the clone if that checkout already exists. Existing
installations under `~/SDS011-driver` need their nginx links and logrotate paths
updated when moving to this layout. Update the checkout with
`git -C ~/github/embedded pull --ff-only`.

```
mkdir -p ~/github
git clone https://github.com/Kraftbar/embedded.git ~/github/embedded
cd ~/github/embedded/sds011
code . 
sudo apt-get install cmake
mkdir -p CMakeBuild
cd CMakeBuild
cmake .. && make 
# it will be ran later
```

```
cd ~/github/embedded/sds011
# plug the usb 
tmp1=$(ls /dev/) 
# unplug the usb 
tmp2=$(ls /dev/)
# find what tty the usb is attached to:
tmp3=$(diff <( echo "$tmp1" ) <( echo "$tmp2" ) )
# edit the correct stream into the driver file 
tmp4=$( echo $tmp3 | grep -o -E "([^[:space:]]*tty[^[:space:]]*)" )
sed -i "s/ttyUSB0/$tmp4/g" sds011_driver.c
``` 


```
cd ~/github/embedded/sds011/CMakeBuild
sudo apt-get install nginx
nohup ./sds011_driver |   while IFS= read -r line; do printf '[%s] %s <br>\n' "$(date '+%Y-%m-%d %H:%M:%S')" "$line"; done > output.txt &
sudo rm /var/www/html/index.nginx-debian.html
sudo  ln -s ~/github/embedded/sds011/CMakeBuild/output.txt /var/www/html/index.html.bk
sudo ln -s ~/github/embedded/sds011/index.html /var/www/html/index.html
``` 
```
sudo tee /etc/logrotate.d/mylogfile <<EOF
"$HOME/github/embedded/sds011/CMakeBuild/output.txt" {
    size 100M
    rotate 5
#    compress
#    delaycompress
    missingok
    notifempty
    create 0640 root root
}
EOF

sudo logrotate -d /etc/logrotate.d/mylogfile
```

```
cd ~/
scp -P 22 pi@raspberrypi:~/github/embedded/sds011/CMakeBuild/output.txt ~/output.txt
sed  -n  's/\[\([0-9-]* [0-9:]*\).*/\1/p' output.txt > date_time.txt
sed  -n 's/.* PM2\.5: \([0-9]*\).*/\1/p' output.txt > PM2.5.txt
sed  -n 's/.* PM10: \([0-9]*\).*/\1/p' output.txt > PM10.txt
paste -d","  date_time.txt PM2.5.txt PM10.txt > data_processed.txt
gnuplot -persist << EOF
set xdata time;set timefmt "%Y-%m-%d %H:%M:%S" ;set format x "%Y-%m-%d %H:%M:%S";
set xtics 3600;
set datafile separator ','
set xtics rotate by -30
set xtics font ", 5"
plot 'data_processed.txt' using 1:2 with line title "PM2.5" axes x1y1, '' using 1:3 with line title "PM10" axes x1y2;
EOF
``` 
