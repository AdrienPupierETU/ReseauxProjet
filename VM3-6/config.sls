# Configuration eth1
# RAPPEL: eth0 est à vagrant, ne pas y toucher

## Désactivation de network-manager
NetworkManager:
  service:
    - dead
    - enable: False
    
## Suppression de la passerelle par défaut
ip route del default:
  cmd:
    - run

##Configuration de VM3
eth1:
  network.managed:
    - enabled: True
    - type: eth
    - proto: none
    - enable_ipv4: false
    - ipv6proto: static
    - enable_ipv6: true
    - ipv6_autoconf: no
    - ipv6ipaddr: fc00:1234:2::36
    - ipv6netmask: 64

eth2:
  network.managed:
    - enabled: True
    - type: eth
    - proto: none
    - ipaddr: 172.16.2.186
    - netmask: 28

## Configuration de la route vers LAN2 via VM2
routes:
  network.routes:
    - name: eth1
    - routes:
      - name: LAN1-6
        ipaddr: fc00:1234:1::/64
        gateway: fc00:1234:2::26
      - name: LAN3
        ipaddr: 172.16.2.144/28
        gateway: 172.16.2.2

'nohup /mnt/partage/launch_tunnel_3-6.sh':
  cmd.run

net.ipv4.ip_forward:
  sysctl:
    - present
    - value: 1

