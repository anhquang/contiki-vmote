sudo route add -6 aaaa::/64 tun0
sudo ip -6 address add aaaa::1/64 dev tun0
sudo sysctl -w net.ipv6.conf.all.forwarding=1
sudo /etc/init.d/radvd stop
sudo /etc/init.d/radvd start
#nc6 -u aaaa::206:98ff:fe00:232 161
sudo ip -6 route list table all root fe80::/10
