sudo ip -6 address add aaaa::1/64 dev usb0
sudo sysctl -w net.ipv6.conf.all.forwarding=1
sudo /etc/init.d/radvd stop
sudo cp /etc/radvd.conf-raven /etc/radvd.conf
sudo /etc/init.d/radvd start

