cd /home/lm/vms/stage/webcam;
mplayer -vo png -frames 1 tv://;
vagrant ssh -c "cd /var/www/arduino_demo; bash node.sh";
